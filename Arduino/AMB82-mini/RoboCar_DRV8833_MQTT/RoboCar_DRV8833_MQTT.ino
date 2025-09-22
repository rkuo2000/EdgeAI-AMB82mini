/*
   RoboCar controlled by MQTT
*/
#include <WiFi.h>
#include <PubSubClient.h>

char ssid[] = "TCFSTWIFI.ALL";    // your network SSID (name)
char pass[] = "035623116";        // your network password
int status = WL_IDLE_STATUS;     // Indicator of Wifi status

char mqttServer[] = "test.mosquitto.org";
char clientId[] = "amebaClient";
char publishTopic[] = "outTopic";
char publishPayload[] = "hello world";
char subscribeTopic[] = "TCFST/Robot1";

// nSLEEP pin connected to Vcc
// nFAULT pin kept floating
#define IN1_PIN     5 // IN1 pin GPIO
#define IN2_PIN     6 // IN2 pin PWM
#define IN3_PIN     7 // IN3 pin GPIO
#define IN4_PIN     8 // IN4 pin PWM

#define FULLSPEED 255
#define HALFSPEED 128
#define STOPSPEED 0

char buffer[128];
String command;

void callback(char* topic, byte* payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (unsigned int i = 0; i < length; i++) {
        Serial.print((char)(payload[i]));
        buffer[i]=(char)(payload[i]);
    }
    Serial.println();

    // command
    command = (char*)buffer;
    Serial.println(command);
    if (command=="forward") {
      forward();
      delay(100);
      stop();
    } else if (command=="backward") {
      backward();
      delay(100);
      stop();
    } else if (command=="turnleft") {
      turnleft();
      delay(100);
      stop();
    } else if (command=="turnright") {
      turnright();
      delay(100);
      stop();
    } else if (command=="stop") {
      stop();
    }
    for (unsigned int i = 0; i < 128; i++) buffer[i] = 0;
}

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void reconnect()
{
    // Loop until we're reconnected
    while (!(client.connected())) {
        Serial.print("\r\nAttempting MQTT connection...");
        // Attempt to connect
        if (client.connect(clientId)) {
            Serial.println("connected");
            // Once connected, publish an announcement and resubscribe
            client.publish(publishTopic, publishPayload);
            client.subscribe(subscribeTopic);
        } else {
            Serial.println("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void setup()
{
    Serial.begin(115200);
    // set 4 GPIOs to OUTPUT mode
    pinMode(IN1_PIN, OUTPUT);
    pinMode(IN2_PIN, OUTPUT);
    pinMode(IN3_PIN, OUTPUT);
    pinMode(IN4_PIN, OUTPUT);    

    // Initialize serial and wait for port to open:
    Serial.begin(115200);
    // wait for serial port to connect.
    while (!Serial) {
        ;
    }

    // Attempt to connect to WiFi network
    while (status != WL_CONNECTED) {
        Serial.print("\r\nAttempting to connect to SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);

        // wait 10 seconds for connection:
        delay(10000);
    }

    wifiClient.setNonBlockingMode();
    client.setServer(mqttServer, 1883);
    client.setCallback(callback);

    // Allow Hardware to sort itself out
    delay(1500);
}

void backward(){
  digitalWrite(IN1_PIN, HIGH);
  analogWrite(IN2_PIN, STOPSPEED);

  digitalWrite(IN3_PIN, HIGH);
  analogWrite(IN4_PIN, STOPSPEED);  
}

void forward(){
  digitalWrite(IN1_PIN, LOW);
  analogWrite(IN2_PIN, FULLSPEED);

  digitalWrite(IN3_PIN, LOW);
  analogWrite(IN4_PIN, FULLSPEED);
}

void turnright(){
  digitalWrite(IN1_PIN, HIGH);
  analogWrite(IN2_PIN, STOPSPEED);

  digitalWrite(IN3_PIN, LOW);
  analogWrite(IN4_PIN, FULLSPEED);
}

void turnleft(){
  digitalWrite(IN1_PIN, LOW);
  analogWrite(IN2_PIN, FULLSPEED);

  digitalWrite(IN3_PIN, HIGH);
  analogWrite(IN4_PIN, STOPSPEED);
}

void stop(){
  digitalWrite(IN1_PIN, LOW);
  analogWrite(IN2_PIN, STOPSPEED);

  digitalWrite(IN3_PIN, LOW);
  analogWrite(IN4_PIN, STOPSPEED);
}

void loop()
{
    if (!(client.connected())) {
        reconnect();
    }
    client.loop();
}
