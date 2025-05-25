
#include <WiFi.h>
#include <WiFiUdp.h>
#include "GenAI.h"
#include "VideoStream.h"
#include "AmebaFatFS.h"
#include "MP3_Player.h"
#include <section_config.h>

#define touchPin A0

String Gemini_key = "AIzaSyACcTG88dK9mxdIJzu-xe-9EWCmsSuuN-o";               // paste your generated Gemini API key here
char wifi_ssid[] = "HITRON-DF90-5G";    // your network SSID (name)
char wifi_pass[] = "0972211921";        // your network password

WiFiSSLClient client;
GenAI llm;
GenAI tts;

AmebaFatFS fs;
char mp3Filename[] = "mp3/Google_TTS_.mp3";

VideoSetting config(768, 768, CAM_FPS, VIDEO_JPEG, 1);
#define CHANNEL 0

uint32_t img_addr = 0;
uint32_t img_len = 0;

//String prompt_msg = "What type and name of the recyclables in the picture?";
String prompt_msg = "Please describe the image?";

void initWiFi()
{
    for (int i = 0; i < 2; i++) {
        WiFi.begin(wifi_ssid, wifi_pass);

        delay(1000);
        Serial.println("");
        Serial.print("Connecting to ");
        Serial.println(wifi_ssid);

        uint32_t StartTime = millis();
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

    config.setRotation(0);
    Camera.configVideoChannel(CHANNEL, config);
    Camera.videoInit();
    Camera.channelBegin(CHANNEL);
    Camera.printInfo();

    pinMode(LED_BUILTIN, OUTPUT); 
}

void loop()
{
    int touchValue = analogRead(touchPin);
    if (touchValue >64) {        
        // Vision prompt using same taken image
        Camera.getImage(0, &img_addr, &img_len);
        strcpy(mp3Filename,"mp3/CameraClick.mp3");
        playMP3();

        // Gemini vision prompt        
        String text = llm.geminivision(Gemini_key, "gemini-2.0-flash", prompt_msg, img_addr, img_len, client);

        // Google TTS
        strcpy(mp3Filename,"mp3/Google_TTS_.mp3");
        tts.googletts(mp3Filename, text, "en-US");
        playMP3();
    }
}

void playMP3() {
    char path[128];
    bool b_result;
    b_result = fs.begin();

    if (b_result) {
        sprintf(path, "%s%s", fs.getRootPath(), mp3Filename);
        File file = fs.open(path);
        int size = file.available();
        mp3_data_len = size;
        file.seek(0);
        mp3_data = new unsigned char[mp3_data_len];
        file.read(mp3_data, mp3_data_len);
        file.close();
        fs.end();
        
        parseMP3();
        setOutputGain(0x80); // The value must be in [0x0~0xAF]
        audio_helix_mp3();
    } else {
        Serial.println("==== SD init failed ====");
    }
}
