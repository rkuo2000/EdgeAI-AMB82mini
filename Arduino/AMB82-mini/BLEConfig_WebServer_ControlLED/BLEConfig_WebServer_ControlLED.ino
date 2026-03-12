/*

 Example guide:
 https://www.amebaiot.com/en/amebapro2-arduino-ameba-web-server/
 */
#include "BLEDevice.h"
#include "BLEWifiConfigService.h"

BLEWifiConfigService configService;

#include <WiFi.h>
//char ssid[] = "TCFSTWIFI.ALL";  // your network SSID (Home WiFi or Smartphone Hotspot)
//char pass[] = "035623116";   // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;     // Indicator of Wifi status

WiFiServer server(80);

#define LED_PIN LED_BUILTIN

String buttonState = "off";

void setup()
{
    Serial.begin(115200);        // initialize serial communication
    pinMode(LED_PIN, OUTPUT);    // set the LED pin mode

    // BLE Config WiFi
    BLE.init();
    BLE.configServer(1);
    configService.addService();
    configService.begin();

    // Wifi config service requires a specific advertisement format to be recognised by the app
    // The advertisement needs the local BT address, which can only be obtained after starting peripheral mode
    // Thus, we stop advertising to update the advert data, wait for advertising to stop, then restart advertising with new data
    BLE.beginPeripheral();
    BLE.configAdvert()->stopAdv();
    BLE.configAdvert()->setAdvData(configService.advData());
    BLE.configAdvert()->updateAdvertParams();
    delay(100);
    BLE.configAdvert()->startAdv();

    server.begin();       // start the web server on port 80
}


void loop()
{
    WiFiClient client = server.available();    // listen for incoming clients

    if (client) {                          // if you get a client,
        Serial.println("new client");      // print a message out the serial port
        String currentLine = "";           // make a String to hold incoming data from the client
        while (client.connected()) {       // loop while the client's connected
            if (client.available()) {      // if there's bytes to read from the client,
                char c = client.read();    // read a byte, then
                Serial.write(c);           // print it out the serial monitor
                if (c == '\n') {           // if the byte is a newline character
                    // if the current line is blank, you got two newline characters in a row.
                    // that's the end of the client HTTP request, so send a response:
                    if (currentLine.length() == 0) {
                        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                        // and a content-type so the client knows what's coming, then a blank line:
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");                        
                        client.println();
                        // Display the HTML web page
                        client.println("<!DOCTYPE html><html>");
                        client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                        client.println("<link rel=\"icon\" href=\"data:,\">");
                        // CSS style
                        client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
                        client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; border-radius: 16px}");
                        client.println(".button2 {background-color: #555555;} </style></head>");
                        // web header
                        client.println("<body><h1>AMB82-mini Web Server</h1>");

                        // Display current state, and ON/OFF buttons for GPIO 26  
                        client.println("<p>LED : " + buttonState + "</p>");
                        // If the output26State is off, it displays the ON button       
                        if (buttonState=="off") {
                          client.println("<p><a href=\"/H\"><button class=\"button\">ON</button></a></p>");
                        } else {
                          client.println("<p><a href=\"/L\"><button class=\"button button2\">OFF</button></a></p>");
                        }

                        // The HTTP response ends with another blank line:
                        client.println();
                        // break out of the while loop:
                        break;
                    } else {    // if you got a newline, then clear currentLine:
                        currentLine = "";
                    }
                } else if (c != '\r') {    // if you got anything else but a carriage return character,
                    currentLine += c;      // add it to the end of the currentLine
                }

                // Check to see if the client request was "GET /H" or "GET /L":
                if (currentLine.endsWith("GET /H")) {
                    digitalWrite(LED_PIN, HIGH);    // GET /H turns the LED on
                    buttonState = "on";
                }
                if (currentLine.endsWith("GET /L")) {
                    digitalWrite(LED_PIN, LOW);    // GET /L turns the LED off
                    buttonState = "off";
                }
            }
        }
        // close the connection:
        client.stop();
        Serial.println("client disconnected");
    }
}
