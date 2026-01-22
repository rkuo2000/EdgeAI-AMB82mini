/*

This sketch shows the example of speech-to-text using Gemini API.

Gemini platform -
server: generativelanguage.googleapis.com
endpoint: /v1beta/models
model: gemini
https://cloud.google.com/vertex-ai/generative-ai/docs/multimodal/audio-understanding

Example Guide: https://ameba-arduino-doc.readthedocs.io/en/latest/amebapro2/Example_Guides/Neural%20Network/Generative%20AI%20Speech.html

Credit : ChungYi Fu (Kaohsiung, Taiwan)

*/

#include <WiFi.h>
#include <WiFiUdp.h>
#include "StreamIO.h"
#include "AudioStream.h"
#include "AudioEncoder.h"
#include "MP4Recording.h"
#include "GenAI.h"
#include "AmebaFatFS.h"

WiFiSSLClient client;
GenAI llm;
GenAI tts;
AudioSetting configA(1);
Audio audio;
AAC aac;
MP4Recording mp4;
StreamIO audioStreamer1(1, 1);
StreamIO audioStreamer2(1, 1);

AmebaFatFS fs;
String mp3Filename = "test_play_google_tts.mp3";

#include "Base64.h"
#include <ArduinoJson.h>

char ssid[] = "TCFSTWIFI.ALL";    // your network SSID (name)
char pass[] = "035623116";        // your network password

String apiKey = "xxxxxx"; // Visit Google AI Studio to get key
String fileName = "gemini_speech";
int recordSeconds = 5;    // 5s
const int buttonPin = 1;          // the number of the pushbutton pin

void initWiFi()
{

    for (int i = 0; i < 2; i++) {
        WiFi.begin(ssid, pass);

        delay(1000);
        Serial.println("");
        Serial.print("Connecting to ");
        Serial.println(ssid);

        long int StartTime = millis();
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            if ((StartTime + 5000) < millis()) {
                break;
            }
        }

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("");
            Serial.println("STAIP address: ");
            Serial.println(WiFi.localIP());
            Serial.println("");

            break;
        }
    }
}

void setup()
{
    Serial.begin(115200);
    initWiFi();

    audio.configAudio(configA);
    audio.begin();
    aac.configAudio(configA);
    aac.begin();
    mp4.configAudio(configA, CODEC_AAC);
    mp4.setRecordingDuration(recordSeconds);
    mp4.setRecordingFileCount(1);
    mp4.setRecordingFileName(fileName);
    mp4.setRecordingDataType(STORAGE_AUDIO);
    audioStreamer1.registerInput(audio);
    audioStreamer1.registerOutput(aac);
    if (audioStreamer1.begin() != 0) {
        Serial.println("StreamIO link start failed");
    }
    audioStreamer2.registerInput(aac);
    audioStreamer2.registerOutput(mp4);
    if (audioStreamer2.begin() != 0) {
        Serial.println("StreamIO link start failed");
    }
    audio.printInfo();

    pinMode(buttonPin, INPUT);
    pinMode(LED_B, OUTPUT);
    pinMode(LED_G, OUTPUT);
}

void loop()
{
    if ((digitalRead(buttonPin)) == 1) {
        // Start MP4 recording after 3 seconds of blinking
        for (int count = 0; count < 3; count++) {
            digitalWrite(LED_B, HIGH);
            delay(500);
            digitalWrite(LED_B, LOW);
            delay(500);
        }

        // ASR 
        String text = llm.geminiaudio(apiKey, fileName, "gemini-3-flash-preview", mp4, "Please transcribe the audio into text", client);
        Serial.println(text);

        // LLM Text
        String prompt_msg = text + ", 請簡短回答";
        String answer = llm.geminitext(apiKey, "gemini-3-flash-preview", prompt_msg, client);   

        // Text-to-Speech
        tts.googletts(mp3Filename, answer, "zh-TW"); // en-US
        delay(500);
        sdPlayMP3(mp3Filename);       
    }
}

void sdPlayMP3(String filename)
{
    fs.begin();
    String filepath = String(fs.getRootPath()) + filename;
    File file = fs.open(filepath, MP3);
    file.setMp3DigitalVol(120);
    file.playMp3();
    file.close();
    fs.end();
}
