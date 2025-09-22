/*

AMB82-mini working with Ollama

Created by Richard Kuo on 2025/09/23

*/

char wifi_ssid[] = "TCFSTWIFI.ALL";    // your network SSID (name)
char wifi_pass[] = "035623116";        // your network password

#include <WiFi.h>
#include "GenAI.h"

WiFiClient client;
GenAI llm;

const int buttonPin  = 1;

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
            delay(200);
            digitalWrite(LED_B, LOW);
            delay(200);
        }

    // Gemini text
        String prompt_msg = "What model are you? please give a brief answer.";
        Serial.println(prompt_msg);
        String text = llm.ollamatext("gpt-oss", prompt_msg, client);
        Serial.println(text);
    }
}
