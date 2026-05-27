/*
   RoboCar: WebServer + DRV8833
*/

#include <WiFi.h>

char ssid[] = "TCFSTWIFI.ALL";    // your network SSID (name)
char pass[] = "035623116";        // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;     // Indicator of Wifi status

WiFiServer server(80);

String commandState = "stop";

// nSLEEP pin connected to Vcc
// nFAULT pin kept floating
#define IN1_PIN     5 // IN1 pin GPIO
#define IN2_PIN     6 // IN2 pin PWM
#define IN3_PIN     7 // IN3 pin GPIO
#define IN4_PIN     8 // IN4 pin PWM

void setup()
{
    Serial.begin(115200);        // initialize serial communication
    pinMode(LED_BUILTIN, OUTPUT);    // set the LED pin mode
    digitalWrite(LED_BUILTIN, HIGH);

    // set 4 GPIOs to OUTPUT mode
    pinMode(IN1_PIN, OUTPUT);
    pinMode(IN2_PIN, OUTPUT);
    pinMode(IN3_PIN, OUTPUT);
    pinMode(IN4_PIN, OUTPUT);

    Serial.println("Motor0 & 1 Stop...");
    digitalWrite(IN1_PIN, LOW); 
    digitalWrite(IN2_PIN, LOW);
    digitalWrite(IN3_PIN, LOW); 
    digitalWrite(IN4_PIN, LOW);
    delay(1000);

    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to Network named: ");
        Serial.println(ssid);    // print the network name (SSID);

        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);
        // wait 10 seconds for connection:
        delay(10000);
    }
    server.begin();       // start the web server on port 80

}


void cmd_B() {
  Serial.println("Motor0 Rotating Clockwise...");
  digitalWrite(IN1_PIN, HIGH); 
  analogWrite(IN2_PIN, 5); // dutycycle 0~255
  Serial.println("Motor1 Rotating Clockwise...");
  digitalWrite(IN3_PIN, HIGH); 
  analogWrite(IN4_PIN, 5); // dutycycle 0~255
  delay(1000);
}

void cmd_F() {
  Serial.println("Motor0 Rotating CounterClockwise...");
  digitalWrite(IN1_PIN, LOW); 
  analogWrite(IN2_PIN, 250); // dutycycle 0~255
  Serial.println("Motor1 Rotating CounterClockwise...");
  digitalWrite(IN3_PIN, LOW); 
  analogWrite(IN4_PIN, 250); // dutycycle 0~255
  delay(1000);
}

void cmd_L() {
  Serial.println("Motor0 Rotating Clockwise...");
  digitalWrite(IN1_PIN, HIGH); 
  analogWrite(IN2_PIN, 5); // dutycycle 0~255
  Serial.println("Motor1 Rotating CounterClockwise...");
  digitalWrite(IN3_PIN, LOW); 
  analogWrite(IN4_PIN, 250); // dutycycle 0~255
  delay(1000);
}

void cmd_R() {
  Serial.println("Motor0 Rotating CounterClockwise...");
  digitalWrite(IN1_PIN, LOW); 
  analogWrite(IN2_PIN, 250); // dutycycle 0~255
  Serial.println("Motor1 Rotating Clockwise...");
  digitalWrite(IN3_PIN, HIGH); 
  analogWrite(IN4_PIN, 5); // dutycycle 0~255  
  delay(1000);
}

void cmd_S() {
  Serial.println("Motor0 Stop...");
  digitalWrite(IN1_PIN, LOW); 
  analogWrite(IN2_PIN, 5);
  Serial.println("Motor1 Stop...");
  digitalWrite(IN3_PIN, LOW); 
  analogWrite(IN4_PIN, 5); // dutycycle 0~255  
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
                        // web header
                        client.println("<body><h1>AMB82-mini Web Server</h1>");
                        // Display current state  
                        client.println("<p>RoboCar : " + commandState + "</p>");                        

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
                if (currentLine.endsWith("GET /F")) {
                    cmd_F();
                    commandState="FORWARD";
                }
                if (currentLine.endsWith("GET /B")) {
                    cmd_B();
                    commandState="BACKWARD";
                }
                if (currentLine.endsWith("GET /L")) {
                    cmd_L();
                    commandState="LEFT";
                }
                if (currentLine.endsWith("GET /R")) {
                    cmd_R();
                    commandState="RIGHT";
                }           
                if (currentLine.endsWith("GET /S")) {
                    cmd_S();
                    commandState="STOP";
                }                          
            }
        }
        // close the connection:
        client.stop();
        Serial.println("client disconnected");
    }
}
