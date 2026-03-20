/*
 Example guide:
 https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/ameba_pro2/amb82-mini/Example_Guides/NTP/Retrieve%20Universal%20Time%20%28UTC%29%20by%20NTPClient%20library.html
*/
#include "OTA.h"
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>

char ssid[] = "TCFSTWIFI.ALL";
char pass[] = "035623116";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

int port = 3000;
char* server = "192.168.0.22";

OTA ota;

void setup()
{
    Serial.begin(115200);

    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    timeClient.begin();
    // Set up the threads
    ota.start_OTA_threads(port, server);
}

void loop()
{
    timeClient.update();

    Serial.println(timeClient.getFormattedTime());

    delay(1000);
}
