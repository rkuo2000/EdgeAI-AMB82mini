/*
This example demonstrates how to capture an image from the camera and send it to Ollama (a local server for Llava Vision).

Credit: geofrancis
*/
#include "StreamIO.h"
#include <ArduinoJson.h> 
#include "AudioStream.h"
#include "AudioEncoder.h"
#include "MP4Recording.h"
#include "WiFi.h"
#include "AmebaFatFS.h"
#include "Base64.h"

#define FILENAME "TestRecordingAudioOnly.mp4"

JsonDocument doc;
WiFiClient client;
AmebaFatFS fs;

AudioSetting configA(0);
Audio audio;
AAC aac;
MP4Recording mp4;
StreamIO audioStreamer1(1, 1);    // 1 Input Audio -> 1 Output AAC
StreamIO audioStreamer2(1, 1);    // 1 Input AAC -> 1 Output MP4
int recordSeconds = 5;

char ssid[] = "TCFSTWIFI.ALL";    // your network SSID (name)
char pass[] = "035623116";        // your network password
int status = WL_IDLE_STATUS;     // Indicator of Wifi status

const char *myDomain = "192.168.0.16"; // Ollama Server IP address
String model = "gemma4:e2b";
String ollama_key = "";
String prompt = "Transcribe the audio";

char buf[512];
char *p;
int recordingstate = 0;
int previousRecordingState = 0;

void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);

    // Attempt to connect to WiFi network
    while (status != WL_CONNECTED) {
        Serial.print("\r\nAttempting to connect to SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);

        // wait 10 seconds for connection:
        delay(2000);
    }

    // Configure audio peripheral for audio data output
    audio.configAudio(configA);
    audio.begin();

    // Configure AAC audio encoder
    aac.configAudio(configA);
    aac.begin();

    // Configure MP4 recording settings
    mp4.configAudio(configA, CODEC_AAC);
    mp4.setRecordingDuration(recordSeconds);
    mp4.setRecordingFileCount(1);
    mp4.setRecordingFileName(String(FILENAME));
    mp4.setRecordingDataType(STORAGE_AUDIO);    // Set MP4 to record audio only

    // Configure StreamIO object to stream data from audio channel to AAC encoder
    audioStreamer1.registerInput(audio);
    audioStreamer1.registerOutput(aac);
    if (audioStreamer1.begin() != 0) {
        Serial.println("StreamIO link start failed");
    }

    // Configure StreamIO object to stream data from AAC encoder to MP4
    audioStreamer2.registerInput(aac);
    audioStreamer2.registerOutput(mp4);
    if (audioStreamer2.begin() != 0) {
        Serial.println("StreamIO link start failed");
    }

    //recordingstate = (int)(mp4.getRecordingState());
    //if (recordingstate == 1) {
    //    digitalWrite(LED_BUILTIN, HIGH);
    //} else {
        mp4.begin(); // Start recording MP4 data to SD card
        Serial.println("Recording");
        delay(recordSeconds * 1000);
    //}
    
/*
    if (recordingstate == 1 && previousRecordingState == 0) {
        // Change from 0 to 1
        digitalWrite(LED_BUILTIN, HIGH);
    } else if (recordingstate == 0 && previousRecordingState == 1) {
        // Change from 1 to 0
        digitalWrite(LED_BUILTIN, LOW);
        send_Ollama();
        delay(1000);
    }  
*/
    // list files under root directory
    fs.begin();

    memset(buf, 0, sizeof(buf));
    fs.readDir(fs.getRootPath(), buf, sizeof(buf));
    String filepath = String(fs.getRootPath()) + String(FILENAME);
    /* the filenames are separated with '\0', so we scan one by one */
    p = buf;
    while (strlen(p) > 0) {
        /* list out file name image will be saved as "TestRecordingAudioOnly.mp4"*/
        if (strstr(p, FILENAME) != NULL) {
            Serial.println("[INFO] Found 'TestRecordingAudioOnly.mp4' in the string.");
            Serial.println("[INFO] Processing file...");
        } else {
            // Serial.println("Substring 'image.jpg' not found in the
            // string.");
        }
        p += strlen(p) + 1;
    }

    uint8_t *fileinput;
    File file = fs.open(filepath);
    unsigned int fileSize = file.size();
    fileinput = (uint8_t *)malloc(fileSize + 1);
    file.read(fileinput, fileSize);
    fileinput[fileSize] = '\0';
    file.close();
    fs.end();

    // Encode the file data as Base64
    int encodedLen = base64_enc_len(fileSize);
    char *encodedData = (char *)malloc(encodedLen);
    base64_encode(encodedData, (char *)fileinput, fileSize);   

    send_Ollama(encodedData);
}

void loop()
{
    // do nothing
}

void send_Ollama(char *encodedData)
{
    String getResponse = "", Feedback = "";
    Serial.println("Connect to " + String(myDomain));
    if (client.connect(myDomain, 11434)) {
        Serial.println("Connection successful");

        String Data = "{\"model\": \"" + model + "\", \"messages\": [{\"role\": \"user\",\"content\": [{ \"type\": \"text\", \"text\": \"" + prompt + "\"},{\"type\": \"input_audio\", \"input_audio\": {\"data\": \"" + encodedData + "\", \"format\": \"mp4\" }}]}]}";
        Serial.println("POST");
        client.println("POST /v1/chat/completions HTTP/1.1");
        client.println("Host: " + String(myDomain));
        client.println("Authorization: Bearer " + ollama_key);
        client.println("Content-Type: application/json; charset=utf-8");
        client.println("Content-Length: " + String(Data.length()));
        client.println("Connection: close");
        Serial.println("CLOSE");
        client.println();

        unsigned int Index;
        for (Index = 0; Index < Data.length(); Index = Index + 1024) {
            client.print(Data.substring(Index, Index + 1024));
        }

        Serial.println("Receive");
        uint32_t waitTime = 10000;
        uint32_t startTime = millis();
        boolean state = false;
        boolean markState = false;

        while ((startTime + waitTime) > millis()) {
            Serial.print(".");
            delay(1000);
        }

        while (client.available()) {
            char c = client.read();
            if (String(c) == "{") {
                markState = true;
            }
            if (state == true && markState == true) {
                Feedback += String(c);
            }
            if (c == '\n') {
                if (getResponse.length() == 0) {
                    state = true;
                }
                getResponse = "";
            } else if (c != '\r') {
                getResponse += String(c);
            }
        }

        if (Feedback.length() > 0) {
            deserializeJson(doc, Feedback);
            String content = doc["choices"][0]["message"]["content"];
            Serial.println(content);           
        }
        Serial.println();
        client.stop();
    }
}
