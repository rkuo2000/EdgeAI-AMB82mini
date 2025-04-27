/*

This sketch shows the example of image prompts using APIs, plus text-to-speech using Google Translate API.

openAI platform - openAI vision
https://platform.openai.com/docs/guides/vision

Google AI Studio - Gemini vision
https://ai.google.dev/gemini-api/docs/vision

GroqCloud - Llama vision
https://console.groq.com/docs/overview

Example Guide: https://ameba-arduino-doc.readthedocs.io/en/latest/amebapro2/Example_Guides/Neural%20Network/Generative%20AI%20Vision.html
Example Guide: https://ameba-arduino-doc.readthedocs.io/en/latest/amebapro2/Example_Guides/Neural%20Network/Text-to-Speech.html

Credit : ChungYi Fu (Kaohsiung, Taiwan)

2025/04/25 讀英文字卡 created by Richard Kuo, NTOU/EE
*/

String openAI_key = "";               // paste your generated openAI API key here
String Gemini_key = "";               // paste your generated Gemini API key here
String Llama_key = "";                // paste your generated Llama API key here
//char wifi_ssid[] = "TCFSTWIFI.ALL";    // your network SSID (name)
//char wifi_pass[] = "035623116";        // your network password
char wifi_ssid[] = "HITRON-DF90-5G";    // your network SSID (name)
char wifi_pass[] = "0972211921";        // your network password

#include <WiFi.h>
#include <WiFiUdp.h>
#include "GenAI.h"
#include "VideoStream.h"
#include "SPI.h"
#include "AmebaILI9341.h"
#include "TJpg_Decoder.h" // Include the jpeg decoder library
#include "AmebaFatFS.h"

WiFiSSLClient client;
GenAI llm;
GenAI tts;

AmebaFatFS fs;
String mp3Filename = "test_play_google_tts.mp3";

VideoSetting config(768, 768, CAM_FPS, VIDEO_JPEG, 1);
#define CHANNEL 0

uint32_t img_addr = 0;
uint32_t img_len = 0;
const int buttonPin = 1;          // the number of the pushbutton pin

//String prompt_msg = "What type and name of the recyclables in the picture?";
//String prompt_msg = "請問這個回收物是什麼?請用中文回答";
String prompt_msg = "Just spell, then say the word in the picture?";

#define TFT_RESET 5
#define TFT_DC    4
#define TFT_CS    SPI_SS

AmebaILI9341 tft = AmebaILI9341(TFT_CS, TFT_DC, TFT_RESET);

#define ILI9341_SPI_FREQUENCY 20000000

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap)
{
    tft.drawBitmap(x, y, w, h, bitmap);

    // Return 1 to decode next block
    return 1;
}

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

void init_tft()
{
    tft.begin();
    tft.setRotation(0);

    tft.clr();
    tft.setCursor(0, 0);

    tft.setForeground(ILI9341_GREEN);
    tft.setFontSize(2);
}

void setup()
{
    Serial.begin(115200);

    SPI.setDefaultFrequency(ILI9341_SPI_FREQUENCY);
    initWiFi();

    config.setRotation(2);
    Camera.configVideoChannel(CHANNEL, config);
    Camera.videoInit();
    Camera.channelBegin(CHANNEL);
    Camera.printInfo();
    
    pinMode(buttonPin, INPUT);
    pinMode(LED_B, OUTPUT);

    init_tft();
    tft.println("GenAIVision_TTS_LCD");

    TJpgDec.setJpgScale(2); // The jpeg image can be scaled by a factor of 1, 2, 4, or 8    
    TJpgDec.setCallback(tft_output);

    tft.println("Press Button");
}

void blink_blue(int count)
{
    for (int i = 0; i < count; i++) {
        digitalWrite(LED_B, HIGH);
        delay(500);
        digitalWrite(LED_B, LOW);
        delay(500);
    }
}

void loop()
{
     if ((digitalRead(buttonPin)) == 1) {
        tft.setCursor(0,0);        
        tft.println("Capture Image");
        blink_blue(2); // blink
    // Camera take image
        Camera.getImage(0, &img_addr, &img_len); 

    // JPEG decode image & display
        TJpgDec.getJpgSize(0, 0, (uint8_t *)img_addr, img_len);
        TJpgDec.drawJpg(0, 0, (uint8_t *)img_addr, img_len);

    // LLM Vision
        String text = llm.geminivision(Gemini_key, "gemini-2.0-flash", prompt_msg, img_addr, img_len, client);
        Serial.println(text);
        tft.setCursor(0,0);
        tft.println(text);

    // Text-To-Speech & play mp3 file
        tts.googletts(mp3Filename, text, "en-US");
        //tts.googletts(mp3Filename, text, "zh-TW");
        delay(500);
        sdPlayMP3(mp3Filename);

        tft.setCursor(0,0);        
        tft.println("Press Button");
    }
}

void sdPlayMP3(String filename)
{
    fs.begin();
    String filepath = String(fs.getRootPath()) + filename;
    File file = fs.open(filepath, MP3);
    file.setMp3DigitalVol(128);
    file.playMp3();
    file.close();
    fs.end();
}
