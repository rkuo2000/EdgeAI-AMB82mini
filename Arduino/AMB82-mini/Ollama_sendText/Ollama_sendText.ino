/*
This example demonstrates how to capture an image from the camera and send it to Ollama (Gemma4).

Credit: geofrancis
*/
#include <ArduinoJson.h> 
#include <WiFi.h>
JsonDocument doc;
WiFiClient client;

uint32_t img_addr = 0;
uint32_t img_len = 0;

char ssid[] = "TCFSTWIFI.ALL";    // your network SSID (name)
char pass[] = "035623116";        // your network password
int status = WL_IDLE_STATUS;     // Indicator of Wifi status

const char *myDomain = "192.168.0.22"; // Ollama Server IP address
String model = "gemma4:e2b";
String ollama_key = "";

int send_count = 0;

void setup()
{
    Serial.begin(115200);

    // Attempt to connect to WiFi network
    while (status != WL_CONNECTED) {
        Serial.print("\r\nAttempting to connect to SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);

        // wait 10 seconds for connection:
        delay(10000);
    }
}

void loop()
{
    if (send_count<3) {
        Serial.println("Send #"+String(send_count));
        if (send_count==0)
            send_Ollama("Say I love you");
        else if (send_count==1)
            send_Ollama("Why is the sky blue?");
        else if (send_count==2)
            send_Ollama("What is the meaning of life?");
        send_count++;
    }
}

void send_Ollama(String prompt)
{
    String getResponse = "", Feedback = "";
    Serial.println("Connect to " + String(myDomain));
    if (client.connect(myDomain, 11434)) {
        Serial.println("Connection successful");

        //String Data = "{\"model\": \"" + model + "\", \"messages\": [{\"role\": \"user\",\"content\": \"" + prompt + "\"}] }";
        String Data = "{\"model\": \"" + model + "\", \"messages\": [{\"role\": \"user\",\"content\": [{ \"type\": \"text\", \"text\": \"" + prompt + "\"}]}]}";
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
