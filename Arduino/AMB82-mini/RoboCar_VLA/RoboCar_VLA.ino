/*
   RoboCar_VLA : Vision-Language-Action with Ollama Gemma4
   -------------------------------------------------------
   Merges: DRV8833 motor + MPU6050 IMU + VL53L0X IR + Camera + Ollama Gemma4

   User Interface:
     Serial Monitor (115200 baud) accepts text commands:
       F / B / L / R / S       - manual motor control
       auto                     - toggle autonomous obstacle avoidance
       vla <prompt>             - send camera image + prompt to Gemma4, execute action
       nav <prompt>             - set navigation goal (e.g. "go to the red cup")
       status                   - print heading, distance, mode
       help                     - print command list

   VLA Pipeline:
     1. Capture camera frame (JPEG)
     2. Base64-encode and send to Ollama Gemma4 with a structured prompt
     3. Parse JSON response for action commands
     4. Execute motor action (forward/back/turn/stop)

   Gemma4 responds with JSON like:
     {"action":"forward","distance":30,"reason":"path clear"}
     {"action":"stop","reason":"obstacle detected"}
     {"action":"turn_left","angle":45,"reason":"target is left"}
*/

#include "I2Cdev.h"
#include <MPU6050_IMU_libraries/MPU6050_6Axis_MotionApps612.h>
#include "Wire.h"
#include <VL53L0X_IR_libraries/VL53L0X.h>
#include "StreamIO.h"
#include "AudioStream.h"
#include "AudioEncoder.h"
#include "MP4Recording.h"
#include "VideoStream.h"
#include "WiFi.h"
#include "AmebaFatFS.h"
#include "Base64.h"
#include <ArduinoJson.h>
#include <GTimer.h>

// ===================== CONFIGURATION =====================
#define TIMER_ID        0
#define SENSOR_INTERVAL 1000000   // 1 second in microseconds

// Motor pins (DRV8833)
#define IN1_PIN  5
#define IN2_PIN  6   // PWM
#define IN3_PIN  7
#define IN4_PIN  8   // PWM

// Speed levels
#define SPEED_SLOW   80
#define SPEED_NORMAL 180
#define SPEED_FAST   250

// Obstacle threshold (mm)
#define OBSTACLE_THRESHOLD 200

// Camera
#define CAM_CHANNEL 0

// ===================== WiFi / OLLAMA =====================
char ssid[] = "HITRON-DF90-2.4G";
char pass[] = "0972211921";
int  status = WL_IDLE_STATUS;

const char* ollamaHost = "192.168.0.22";
const int   ollamaPort = 11434;
String      ollamaModel = "gemma4:e2b";
String      ollamaKey   = "";

// Camera image buffer
uint32_t img_addr = 0;
uint32_t img_len  = 0;

// ===================== OBJECTS =====================
WiFiClient client;
MPU6050    mpu;
VL53L0X    sensor;
AmebaFatFS fs;
VideoSetting config(768, 768, CAM_FPS, VIDEO_JPEG, 1);

// ===================== STATE =====================
// IMU
bool    dmpReady = false;
uint8_t mpuIntStatus, devStatus;
uint16_t packetSize, fifoCount;
uint8_t  fifoBuffer[128];
Quaternion q;
VectorFloat gravity;
float ypr[3];
int    currentHeading = 0;   // 0-359 degrees

// IR
int currentDistance = 999;    // mm

// Motor
String currentMode    = "manual";   // "manual" | "auto" | "vla" | "nav"
String motorState     = "stop";
int    motorSpeed     = SPEED_NORMAL;
bool   obstacleStop   = false;

// Navigation goal
String navGoal = "";
int    navHeading = -1;   // target heading, -1 = none

// Sensor flags (set by GTimer ISR)
volatile bool flag_readIR  = false;
volatile bool flag_readIMU = false;
int sensorCounter = 0;

// ===================== TIMER ISR =====================
void sensorTimerHandler(uint32_t data) {
    sensorCounter++;
    flag_readIR  = true;
    flag_readIMU = true;
}

// ===================== MOTOR CONTROL =====================
void motorStop() {
    digitalWrite(IN1_PIN, LOW);
    analogWrite(IN2_PIN, 0);
    digitalWrite(IN3_PIN, LOW);
    analogWrite(IN4_PIN, 0);
    motorState = "stop";
}

void motorForward(int speed) {
    digitalWrite(IN1_PIN, LOW);
    analogWrite(IN2_PIN, speed);
    digitalWrite(IN3_PIN, LOW);
    analogWrite(IN4_PIN, speed);
    motorState = "forward";
}

void motorBackward(int speed) {
    digitalWrite(IN1_PIN, HIGH);
    analogWrite(IN2_PIN, 255 - speed);
    digitalWrite(IN3_PIN, HIGH);
    analogWrite(IN4_PIN, 255 - speed);
    motorState = "backward";
}

void motorLeft(int speed) {
    digitalWrite(IN1_PIN, HIGH);
    analogWrite(IN2_PIN, 255 - speed);
    digitalWrite(IN3_PIN, LOW);
    analogWrite(IN4_PIN, speed);
    motorState = "left";
}

void motorRight(int speed) {
    digitalWrite(IN1_PIN, LOW);
    analogWrite(IN2_PIN, speed);
    digitalWrite(IN3_PIN, HIGH);
    analogWrite(IN4_PIN, 255 - speed);
    motorState = "right";
}

void executeAction(String action, int speed) {
    if (action == "forward")     motorForward(speed);
    else if (action == "back")   motorBackward(speed);
    else if (action == "left")   motorLeft(speed);
    else if (action == "right")  motorRight(speed);
    else                         motorStop();
}

// ===================== IMU =====================
void readIMU() {
    if (!dmpReady) return;
    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        currentHeading = int(ypr[0] * 180.0 / M_PI) + 180;
    }
}

// ===================== IR SENSOR =====================
void readIR() {
    currentDistance = sensor.readRangeSingleMillimeters();
    if (sensor.timeoutOccurred()) {
        currentDistance = 9999;
    }
}

// ===================== CAMERA =====================
String captureImageBase64() {
    Camera.getImage(CAM_CHANNEL, &img_addr, &img_len);
    uint8_t* fbBuf = (uint8_t*)img_addr;
    uint32_t fbLen = img_len;

    char* input  = (char*)fbBuf;
    char  output[base64_enc_len(3)];
    String b64 = "data:image/jpeg;base64,";
    for (uint32_t i = 0; i < fbLen; i++) {
        base64_encode(output, (input++), 3);
        if (i % 3 == 0) {
            b64 += String(output);
        }
    }
    return b64;
}

// ===================== OLLAMA COMMUNICATION =====================
// Sends a prompt (text-only or with image) and returns the content string.
// If withImage is true, attaches the current camera frame.
String ollamaChat(String prompt, bool withImage) {
    String feedback = "";

    if (!client.connect(ollamaHost, ollamaPort)) {
        Serial.println("[VLA] Ollama connection failed");
        return "";
    }

    // Build JSON payload
    String imageB64 = "";
    if (withImage) {
        Serial.println("[VLA] Capturing image...");
        imageB64 = captureImageBase64();
        Serial.println("[VLA] Image captured, size=" + String(imageB64.length()));
    }

    // Construct messages array
    String msgContent;
    if (withImage) {
        msgContent = "["
            "{\"type\":\"text\",\"text\":\"" + prompt + "\"},"
            "{\"type\":\"image_url\",\"image_url\":{\"url\":\"" + imageB64 + "\"}}"
            "]";
    } else {
        msgContent = "["
            "{\"type\":\"text\",\"text\":\"" + prompt + "\"}"
            "]";
    }

    String payload = "{"
        "\"model\":\"" + ollamaModel + "\","
        "\"messages\":[{\"role\":\"user\",\"content\":" + msgContent + "}],"
        "\"stream\":false"
        "}";

    // Send HTTP request
    client.println("POST /v1/chat/completions HTTP/1.1");
    client.println("Host: " + String(ollamaHost));
    if (ollamaKey.length() > 0) {
        client.println("Authorization: Bearer " + ollamaKey);
    }
    client.println("Content-Type: application/json; charset=utf-8");
    client.println("Content-Length: " + String(payload.length()));
    client.println("Connection: close");
    client.println();

    // Send body in chunks
    for (unsigned int i = 0; i < payload.length(); i += 1024) {
        client.print(payload.substring(i, i + 1024));
    }

    // Wait for response
    Serial.println("[VLA] Waiting for response...");
    delay(3000);

    // Read response
    String httpHeader = "";
    bool headerDone = false;
    String jsonBody = "";

    while (client.available()) {
        char c = client.read();
        if (!headerDone) {
            httpHeader += c;
            if (httpHeader.indexOf("\r\n\r\n") >= 0) {
                headerDone = true;
            }
        } else {
            jsonBody += c;
        }
    }
    client.stop();

    // Parse JSON response
    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, jsonBody);
    if (err) {
        Serial.print("[VLA] JSON parse error: ");
        Serial.println(err.c_str());
        return "";
    }

    feedback = doc["choices"][0]["message"]["content"].as<String>();
    return feedback;
}

// ===================== VLA ACTION PARSER =====================
void handleVLAResponse(String response) {
    Serial.println("[VLA] Response: " + response);

    // Try to extract JSON action block from response
    int jsonStart = response.indexOf('{');
    int jsonEnd   = response.lastIndexOf('}');

    if (jsonStart >= 0 && jsonEnd > jsonStart) {
        String jsonStr = response.substring(jsonStart, jsonEnd + 1);
        JsonDocument actionDoc;
        DeserializationError err = deserializeJson(actionDoc, jsonStr);

        if (!err) {
            String action = actionDoc["action"].as<String>();
            int distance   = actionDoc["distance"].as<int>();
            String reason  = actionDoc["reason"].as<String>();

            Serial.println("[VLA] Action: " + action + " | Reason: " + reason);

            if (action == "forward") {
                int spd = (distance > 50) ? SPEED_FAST : SPEED_NORMAL;
                motorForward(spd);
                if (distance > 0) {
                    delay(min(distance * 30, 3000));  // rough time estimate
                    motorStop();
                }
            } else if (action == "back") {
                motorBackward(SPEED_NORMAL);
                delay(1500);
                motorStop();
            } else if (action == "turn_left") {
                motorLeft(SPEED_NORMAL);
                delay(1000);
                motorStop();
            } else if (action == "turn_right") {
                motorRight(SPEED_NORMAL);
                delay(1000);
                motorStop();
            } else if (action == "stop") {
                motorStop();
            } else {
                Serial.println("[VLA] Unknown action: " + action);
                motorStop();
            }
            return;
        }
    }

    // Fallback: scan response keywords for simple commands
    response.toLowerCase();
    if (response.indexOf("forward") >= 0 || response.indexOf("ahead") >= 0) {
        motorForward(SPEED_NORMAL);
        delay(2000);
        motorStop();
    } else if (response.indexOf("back") >= 0 || response.indexOf("reverse") >= 0) {
        motorBackward(SPEED_NORMAL);
        delay(1500);
        motorStop();
    } else if (response.indexOf("left") >= 0) {
        motorLeft(SPEED_NORMAL);
        delay(1000);
        motorStop();
    } else if (response.indexOf("right") >= 0) {
        motorRight(SPEED_NORMAL);
        delay(1000);
        motorStop();
    } else if (response.indexOf("stop") >= 0) {
        motorStop();
    } else {
        Serial.println("[VLA] No recognized action in response");
        motorStop();
    }
}

// ===================== AUTONOMOUS MODE =====================
void autonomousStep() {
    if (currentDistance < OBSTACLE_THRESHOLD) {
        motorStop();
        Serial.println("[AUTO] Obstacle at " + String(currentDistance) + "mm - stopped");

        // Ask Gemma4 what to do
        String prompt = "I am a robot car. There is an obstacle " +
                        String(currentDistance) + "mm ahead. " +
                        "Heading is " + String(currentHeading) + " degrees. " +
                        "Should I turn left, turn right, or go back? "
                        "Reply with JSON: {\"action\":\"turn_left\" or \"turn_right\" or \"back\", \"reason\":\"...\"}";

        String response = ollamaChat(prompt, true);
        handleVLAResponse(response);
    } else {
        motorForward(SPEED_NORMAL);
    }
}

// ===================== NAVIGATION MODE =====================
void navigationStep() {
    if (navGoal.length() == 0) return;

    // Ask Gemma4 where the goal is relative to the robot
    String prompt = "I am a robot car trying to reach: " + navGoal + ". " +
                    "Current heading: " + String(currentHeading) + " degrees. " +
                    "Distance to nearest obstacle: " + String(currentDistance) + "mm. " +
                    "Describe what you see and tell me which direction to go. "
                    "Reply with JSON: {\"action\":\"forward\" or \"turn_left\" or \"turn_right\" or \"stop\", "
                    "\"distance\":NN, \"reason\":\"...\"}";

    String response = ollamaChat(prompt, true);
    handleVLAResponse(response);
}

// ===================== SERIAL COMMAND PARSER =====================
void processSerialCommand(String cmd) {
    cmd.trim();
    if (cmd.length() == 0) return;

    // Extract command word and optional argument
    int spaceIdx = cmd.indexOf(' ');
    String verb = (spaceIdx > 0) ? cmd.substring(0, spaceIdx) : cmd;
    String arg  = (spaceIdx > 0) ? cmd.substring(spaceIdx + 1) : "";
    verb.toLowerCase();

    // --- Manual motor commands ---
    if (verb == "f" || verb == "forward") {
        currentMode = "manual";
        motorForward(motorSpeed);
        Serial.println("[CMD] Forward");
    }
    else if (verb == "b" || verb == "back") {
        currentMode = "manual";
        motorBackward(motorSpeed);
        Serial.println("[CMD] Backward");
    }
    else if (verb == "l" || verb == "left") {
        currentMode = "manual";
        motorLeft(motorSpeed);
        Serial.println("[CMD] Left");
    }
    else if (verb == "r" || verb == "right") {
        currentMode = "manual";
        motorRight(motorSpeed);
        Serial.println("[CMD] Right");
    }
    else if (verb == "s" || verb == "stop") {
        currentMode = "manual";
        motorStop();
        Serial.println("[CMD] Stop");
    }
    // --- Speed ---
    else if (verb == "speed") {
        if (arg.length() > 0) {
            motorSpeed = constrain(arg.toInt(), 0, 255);
            Serial.println("[CMD] Speed set to " + String(motorSpeed));
        }
    }
    // --- Auto mode ---
    else if (verb == "auto") {
        if (currentMode == "auto") {
            currentMode = "manual";
            motorStop();
            Serial.println("[CMD] Auto mode OFF");
        } else {
            currentMode = "auto";
            Serial.println("[CMD] Auto mode ON");
        }
    }
    // --- VLA query ---
    else if (verb == "vla") {
        if (arg.length() == 0) {
            Serial.println("[CMD] Usage: vla <prompt>");
            return;
        }
        currentMode = "manual";
        motorStop();
        Serial.println("[CMD] VLA query: " + arg);
        String response = ollamaChat(arg, true);
        handleVLAResponse(response);
    }
    // --- Navigation goal ---
    else if (verb == "nav") {
        if (arg.length() == 0) {
            navGoal = "";
            currentMode = "manual";
            motorStop();
            Serial.println("[CMD] Navigation cleared");
        } else {
            navGoal = arg;
            currentMode = "nav";
            Serial.println("[CMD] Navigation goal: " + navGoal);
        }
    }
    // --- Text-only Ollama query (no camera) ---
    else if (verb == "ask") {
        if (arg.length() == 0) {
            Serial.println("[CMD] Usage: ask <prompt>");
            return;
        }
        Serial.println("[CMD] Ask: " + arg);
        String response = ollamaChat(arg, false);
        Serial.println("[VLA] Answer: " + response);
    }
    // --- Status ---
    else if (verb == "status") {
        Serial.println("=== RoboCar Status ===");
        Serial.println("  Mode:     " + currentMode);
        Serial.println("  Motor:    " + motorState);
        Serial.println("  Speed:    " + String(motorSpeed));
        Serial.println("  Heading:  " + String(currentHeading) + " deg");
        Serial.println("  Distance: " + String(currentDistance) + " mm");
        if (navGoal.length() > 0) {
            Serial.println("  NavGoal:  " + navGoal);
        }
        Serial.println("======================");
    }
    // --- Help ---
    else if (verb == "help") {
        Serial.println("=== RoboCar VLA Commands ===");
        Serial.println("  f / forward   - move forward");
        Serial.println("  b / back      - move backward");
        Serial.println("  l / left      - turn left");
        Serial.println("  r / right     - turn right");
        Serial.println("  s / stop      - stop motors");
        Serial.println("  speed <0-255> - set motor speed");
        Serial.println("  auto          - toggle autonomous mode");
        Serial.println("  vla <prompt>  - VLA: camera + prompt -> action");
        Serial.println("  nav <goal>    - set navigation goal");
        Serial.println("  ask <prompt>  - text-only Ollama query");
        Serial.println("  status        - show current status");
        Serial.println("  help          - this help");
        Serial.println("============================");
    }
    else {
        Serial.println("[CMD] Unknown command: " + verb + " (type 'help')");
    }
}

// ===================== SETUP =====================
void setup() {
    Serial.begin(115200);
    Wire.begin();
    Wire.setClock(400000);

    Serial.println();
    Serial.println("===========================================");
    Serial.println("  RoboCar VLA - Vision-Language-Action");
    Serial.println("  Gemma4 Model: " + ollamaModel);
    Serial.println("===========================================");

    // LED
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    // Motor pins
    pinMode(IN1_PIN, OUTPUT);
    pinMode(IN2_PIN, OUTPUT);
    pinMode(IN3_PIN, OUTPUT);
    pinMode(IN4_PIN, OUTPUT);
    motorStop();

    // IR sensor
    Serial.println("[INIT] VL53L0X IR sensor...");
    sensor.setTimeout(500);
    if (!sensor.init()) {
        Serial.println("[INIT] IR sensor FAILED!");
        while (1);
    }
    sensor.setMeasurementTimingBudget(20000);

    // IMU
    Serial.println("[INIT] MPU6050 IMU...");
    mpu.initialize();
    Serial.println(mpu.testConnection() ? "[INIT] MPU6050 OK" : "[INIT] MPU6050 FAILED");

    devStatus = mpu.dmpInitialize();
    mpu.setXGyroOffset(51);
    mpu.setYGyroOffset(8);
    mpu.setZGyroOffset(21);
    mpu.setXAccelOffset(1150);
    mpu.setYAccelOffset(-50);
    mpu.setZAccelOffset(1060);

    if (devStatus == 0) {
        mpu.CalibrateAccel(6);
        mpu.CalibrateGyro(6);
        mpu.setDMPEnabled(true);
        mpuIntStatus = mpu.getIntStatus();
        dmpReady = true;
        packetSize = mpu.dmpGetFIFOPacketSize();
        Serial.println("[INIT] DMP ready");
    } else {
        Serial.println("[INIT] DMP init failed code " + String(devStatus));
    }

    // Camera
    Serial.println("[INIT] Camera...");
    config.setRotation(0);
    Camera.configVideoChannel(CAM_CHANNEL, config);
    Camera.videoInit();
    Camera.channelBegin(CAM_CHANNEL);
    Camera.printInfo();
    delay(2000);

    // WiFi
    Serial.println("[INIT] Connecting to WiFi: " + String(ssid));
    while (status != WL_CONNECTED) {
        status = WiFi.begin(ssid, pass);
        delay(10000);
    }
    Serial.println("[INIT] WiFi connected: " + WiFi.localIP().toString());
    digitalWrite(LED_BUILTIN, HIGH);

    // Ready tone
    fs.begin();
    File mp3file = fs.open(String(fs.getRootPath()) + "robocar_ready.mp3", MP3);
    if (mp3file) {
        mp3file.setMp3DigitalVol(128);
        mp3file.playMp3();
        mp3file.close();
    }

    // Sensor timer (1 Hz)
    GTimer.begin(TIMER_ID, SENSOR_INTERVAL, sensorTimerHandler);

    Serial.println();
    Serial.println("[INIT] Ready! Type 'help' for commands.");
    Serial.println();
}

// ===================== LOOP =====================
void loop() {
    // --- Read serial commands ---
    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        processSerialCommand(cmd);
    }

    // --- Read sensors (from timer flags) ---
    if (flag_readIR) {
        readIR();
        flag_readIR = false;
    }
    if (flag_readIMU) {
        readIMU();
        flag_readIMU = false;
    }

    // --- Safety: always stop on close obstacle in manual mode ---
    if (currentMode == "manual" && currentDistance < OBSTACLE_THRESHOLD) {
        if (motorState != "stop") {
            motorStop();
            Serial.println("[SAFE] Obstacle too close - emergency stop");
        }
    }

    // --- Autonomous mode loop ---
    if (currentMode == "auto") {
        static unsigned long lastAutoStep = 0;
        if (millis() - lastAutoStep > 2000) {  // step every 2 seconds
            autonomousStep();
            lastAutoStep = millis();
        }
    }

    // --- Navigation mode loop ---
    if (currentMode == "nav") {
        static unsigned long lastNavStep = 0;
        if (millis() - lastNavStep > 3000) {  // step every 3 seconds
            navigationStep();
            lastNavStep = millis();
        }
    }

    delay(50);
}
