/*
This example demonstrates how to capture an image from the camera and send it to Ollama (a local server for Llava Vision).

Credit: geofrancis
*/

#include <ArduinoJson.h> 
#include <WiFi.h>
WiFiClient client;

JsonDocument doc;

char ssid[] = "TCFSTWIFI.ALL";    // your network SSID (name)
char pass[] = "035623116";        // your network password
int status = WL_IDLE_STATUS;      // Indicator of Wifi status

const char *myDomain = "192.168.0.22"; // Ollama Server IP address
String model = "gemma4:e2b";
String ollama_key = "";
String prompt = "Say Hello to me";

boolean state = false;
boolean markState = false;
String getResponse = "";
String Feedback = "";
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
    Serial.println();
}

 
void loop()
{
    if (send_count<3 && !markState) {
        Serial.println("Send #"+String(send_count));
        if      (send_count==0) sendOllama("Say I Love you");
        else if (send_count==1) sendOllama("Why is the sky blue?");
        else if (send_count==2) sendOllama("What is the meaning of life?");
        send_count++;
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
        Serial.println(Feedback);
        deserializeJson(doc, Feedback);
        String content = doc["choices"][0]["message"]["content"];
        Serial.println(content);

        Feedback="";
        state=false; markState=false;

        Serial.println("Client Stop!");
        client.stop();
    }
}

void sendOllama(String prompt)
{
    Serial.println("Connect to " + String(myDomain));
    if (client.connect(myDomain, 11434)) {
        Serial.println();
        Serial.println("Connection successful");

        String Data = "{\"model\": \"" + model + "\", \"messages\": [{\"role\": \"user\",\"content\": \"" + prompt + "\"}] }";
        Serial.println("POST");
        client.println("POST /v1/chat/completions HTTP/1.1");
        client.println("Host: " + String(myDomain));
        client.println("Authorization: Bearer " + ollama_key);
        client.println("Content-Type: application/json; charset=utf-8");
        client.println("Content-Length: " + String(Data.length()));
        //client.println("Connection: keep-alive");
        client.println("Connection: close");
        Serial.println("CLOSE");
        client.println();
        client.println(Data);
        Serial.println("Data Sent");       
    }
}
