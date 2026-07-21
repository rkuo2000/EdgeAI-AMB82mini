/*
   RoboCar_VLA : Vision-Language-Action with Ollama Gemma4
   -------------------------------------------------------
   Merges: DRV8833 motor + MPU6050 IMU + VL53L0X IR + Camera + Ollama Gemma4

   User Interface:
     Web Server (port 80) - open http://<IP> in browser
     Serial Monitor (115200 baud) also accepts text commands

   Web UI Endpoints:
     /              - main dashboard
     /F, /B, /L, /R, /S  - motor control
     /SPEED?val=N   - set speed (0-255)
     /AUTO          - toggle autonomous mode
     /VLA?q=<prompt> - VLA query with camera
     /NAV?q=<goal>  - set navigation goal
     /NAVCLR        - clear navigation goal
     /ASK?q=<text>  - text-only Ollama query
     /STATUS        - JSON status
     /CAMERA        - JPEG snapshot
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
#define SENSOR_INTERVAL 1000000

// Motor pins (DRV8833)
#define IN1_PIN  5
#define IN2_PIN  6
#define IN3_PIN  7
#define IN4_PIN  8

// Speed levels
#define SPEED_SLOW   80
#define SPEED_NORMAL 180
#define SPEED_FAST   250

// Obstacle threshold (mm)
#define OBSTACLE_THRESHOLD 200

// Camera
#define CAM_CHANNEL 0

// ===================== WiFi / OLLAMA =====================
char ssid[] = "TCFSTWIFI.ALL";
char pass[] = "035623116";
int  status = WL_IDLE_STATUS;

const char* ollamaHost = "192.168.3.83";
const int   ollamaPort = 11434;
String      ollamaModel = "gemma4:e2b";
String      ollamaKey   = "";

// Camera image buffer
uint32_t img_addr = 0;
uint32_t img_len  = 0;

// ===================== OBJECTS =====================
WiFiClient client;
WiFiServer server(80);
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
int    currentHeading = 0;

// IR
int currentDistance = 999;

// Motor
String currentMode    = "manual";
String motorState     = "stop";
int    motorSpeed     = SPEED_NORMAL;
bool   obstacleStop   = false;

// Navigation goal
String navGoal = "";
int    navHeading = -1;

// Sensor flags
volatile bool flag_readIR  = false;
volatile bool flag_readIMU = false;
int sensorCounter = 0;

// VLA response (for web UI polling)
String lastVLAResponse = "";
String lastVLAReason   = "";

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
String ollamaChat(String prompt, bool withImage) {
    String feedback = "";

    if (!client.connect(ollamaHost, ollamaPort)) {
        Serial.println("[VLA] Ollama connection failed");
        return "";
    }

    String imageB64 = "";
    if (withImage) {
        Serial.println("[VLA] Capturing image...");
        imageB64 = captureImageBase64();
        Serial.println("[VLA] Image captured, size=" + String(imageB64.length()));
    }

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

    client.println("POST /v1/chat/completions HTTP/1.1");
    client.println("Host: " + String(ollamaHost));
    if (ollamaKey.length() > 0) {
        client.println("Authorization: Bearer " + ollamaKey);
    }
    client.println("Content-Type: application/json; charset=utf-8");
    client.println("Content-Length: " + String(payload.length()));
    client.println("Connection: close");
    client.println();

    for (unsigned int i = 0; i < payload.length(); i += 1024) {
        client.print(payload.substring(i, i + 1024));
    }

    Serial.println("[VLA] Waiting for response...");
    delay(3000);

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

            lastVLAResponse = action;
            lastVLAReason   = reason;
            Serial.println("[VLA] Action: " + action + " | Reason: " + reason);

            if (action == "forward") {
                int spd = (distance > 50) ? SPEED_FAST : SPEED_NORMAL;
                motorForward(spd);
                if (distance > 0) {
                    delay(min(distance * 30, 3000));
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

    String prompt = "I am a robot car trying to reach: " + navGoal + ". " +
                    "Current heading: " + String(currentHeading) + " degrees. " +
                    "Distance to nearest obstacle: " + String(currentDistance) + "mm. " +
                    "Describe what you see and tell me which direction to go. "
                    "Reply with JSON: {\"action\":\"forward\" or \"turn_left\" or \"turn_right\" or \"stop\", "
                    "\"distance\":NN, \"reason\":\"...\"}";

    String response = ollamaChat(prompt, true);
    handleVLAResponse(response);
}

// ===================== WEB SERVER =====================
void handleClient(WiFiClient client) {
    String currentLine = "";
    String requestPath = "";
    String queryString = "";
    bool pathDone = false;

    while (client.connected()) {
        if (client.available()) {
            char c = client.read();
            if (c == '\n') {
                if (currentLine.length() == 0) {
                    break;
                }
                // Parse request line
                if (currentLine.startsWith("GET ")) {
                    int pathStart = 4;
                    int pathEnd = currentLine.indexOf(' ', pathStart);
                    String fullPath = currentLine.substring(pathStart, pathEnd);

                    int qIdx = fullPath.indexOf('?');
                    if (qIdx >= 0) {
                        requestPath = fullPath.substring(0, qIdx);
                        queryString = fullPath.substring(qIdx + 1);
                    } else {
                        requestPath = fullPath;
                        queryString = "";
                    }
                }
                currentLine = "";
            } else if (c != '\r') {
                currentLine += c;
            }
        } else {
            break;
        }
    }

    // --- Parse query parameters ---
    String qValue = "";
    int qpIdx = queryString.indexOf("q=");
    if (qpIdx >= 0) {
        qValue = queryString.substring(qpIdx + 2);
        int ampIdx = qValue.indexOf('&');
        if (ampIdx >= 0) qValue = qValue.substring(0, ampIdx);
        qValue.replace("+", " ");
        qValue.replace("%20", " ");
        qValue.replace("%22", "\"");
    }
    String valValue = "";
    int vpIdx = queryString.indexOf("val=");
    if (vpIdx >= 0) {
        valValue = queryString.substring(vpIdx + 4);
        int ampIdx = valValue.indexOf('&');
        if (ampIdx >= 0) valValue = valValue.substring(0, ampIdx);
    }

    // --- Process commands ---
    if (requestPath == "/F") {
        currentMode = "manual"; motorForward(motorSpeed);
    } else if (requestPath == "/B") {
        currentMode = "manual"; motorBackward(motorSpeed);
    } else if (requestPath == "/L") {
        currentMode = "manual"; motorLeft(motorSpeed);
    } else if (requestPath == "/R") {
        currentMode = "manual"; motorRight(motorSpeed);
    } else if (requestPath == "/S") {
        currentMode = "manual"; motorStop();
    } else if (requestPath == "/SPEED") {
        if (valValue.length() > 0) motorSpeed = constrain(valValue.toInt(), 0, 255);
    } else if (requestPath == "/AUTO") {
        if (currentMode == "auto") { currentMode = "manual"; motorStop(); }
        else { currentMode = "auto"; }
    } else if (requestPath == "/VLA") {
        if (qValue.length() > 0) {
            currentMode = "manual"; motorStop();
            String response = ollamaChat(qValue, true);
            handleVLAResponse(response);
        }
    } else if (requestPath == "/NAV") {
        if (qValue.length() > 0) {
            navGoal = qValue; currentMode = "nav";
        }
    } else if (requestPath == "/NAVCLR") {
        navGoal = ""; currentMode = "manual"; motorStop();
    } else if (requestPath == "/ASK") {
        if (qValue.length() > 0) {
            String response = ollamaChat(qValue, false);
            lastVLAResponse = "ask";
            lastVLAReason   = response;
        }
    } else if (requestPath == "/STATUS") {
        String json = "{";
        json += "\"mode\":\"" + currentMode + "\",";
        json += "\"motor\":\"" + motorState + "\",";
        json += "\"speed\":" + String(motorSpeed) + ",";
        json += "\"heading\":" + String(currentHeading) + ",";
        json += "\"distance\":" + String(currentDistance) + ",";
        json += "\"navGoal\":\"" + navGoal + "\",";
        json += "\"vlaResponse\":\"" + lastVLAResponse + "\",";
        json += "\"vlaReason\":\"" + lastVLAReason + "\"";
        json += "}";
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: application/json");
        client.println("Connection: close");
        client.println();
        client.println(json);
        client.stop();
        return;
    } else if (requestPath == "/CAMERA") {
        Camera.getImage(CAM_CHANNEL, &img_addr, &img_len);
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: image/jpeg");
        client.println("Content-Length: " + String(img_len));
        client.println("Connection: close");
        client.println();
        uint8_t* ptr = (uint8_t*)img_addr;
        for (uint32_t i = 0; i < img_len; i++) {
            client.write(ptr[i]);
        }
        client.stop();
        return;
    }

    // --- Serve HTML dashboard ---
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html; charset=utf-8");
    client.println("Connection: close");
    client.println();

    String modeLabel = (currentMode == "auto") ? "AUTO" :
                       (currentMode == "nav")  ? "NAV"  :
                       (currentMode == "vla")  ? "VLA"  : "MANUAL";

    client.println("<!DOCTYPE html><html>");
    client.println("<head>");
    client.println("<meta name=\"viewport\" content=\"width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no\">");
    client.println("<title>RoboCar VLA</title>");
    client.println("<style>");
    client.println("*{box-sizing:border-box;margin:0;padding:0}");
    client.println("body{font-family:Arial,sans-serif;background:#1a1a2e;color:#eee;text-align:center;padding:10px}");
    client.println("h1{color:#e94560;font-size:1.5em;margin:10px 0}");
    client.println(".card{background:#16213e;border-radius:12px;padding:12px;margin:8px auto;max-width:500px}");
    client.println(".card h2{color:#0f3460;font-size:1em;margin-bottom:8px;color:#e94560}");
    client.println(".grid{display:grid;grid-template-columns:1fr 1fr 1fr;gap:6px;max-width:300px;margin:auto}");
    client.println(".btn{display:block;padding:14px;border:none;border-radius:8px;font-size:1.1em;font-weight:bold;cursor:pointer;color:#fff;text-decoration:none;text-align:center}");
    client.println(".btn-f{background:#4ecca3}");
    client.println(".btn-b{background:#e94560}");
    client.println(".btn-l{background:#f5a623}");
    client.println(".btn-r{background:#f5a623}");
    client.println(".btn-s{background:#555}");
    client.println(".btn-auto{background:#0f3460;margin:6px auto}");
    client.println(".btn-auto.on{background:#4ecca3}");
    client.println(".btn-vla{background:#533483}");
    client.println(".btn-nav{background:#e94560}");
    client.println("input[type=text]{width:100%;padding:10px;border:none;border-radius:6px;margin:4px 0;font-size:1em}");
    client.println("input[type=range]{width:100%;margin:6px 0}");
    client.println(".status{font-size:0.85em;color:#aaa;margin:4px 0}");
    client.println(".status span{color:#4ecca3;font-weight:bold}");
    client.println("img{width:100%;max-width:480px;border-radius:8px;margin:6px 0}");
    client.println("</style></head><body>");

    client.println("<h1>RoboCar VLA</h1>");

    // Status bar
    client.println("<div class=\"card\">");
    client.println("<div class=\"status\">Mode: <span id=\"mode\">" + modeLabel + "</span> | ");
    client.println("Motor: <span id=\"motor\">" + motorState + "</span> | ");
    client.println("Speed: <span id=\"speed\">" + String(motorSpeed) + "</span></div>");
    client.println("<div class=\"status\">Heading: <span id=\"heading\">" + String(currentHeading) + "</span>&deg; | ");
    client.println("Distance: <span id=\"dist\">" + String(currentDistance) + "</span> mm</div>");
    client.println("</div>");

    // D-pad
    client.println("<div class=\"card\"><h2>Drive</h2>");
    client.println("<div class=\"grid\">");
    client.println("<div></div><a href=\"/F\" class=\"btn btn-f\">&#9650;</a><div></div>");
    client.println("<a href=\"/L\" class=\"btn btn-l\">&#9664;</a><a href=\"/S\" class=\"btn btn-s\">STOP</a><a href=\"/R\" class=\"btn btn-r\">&#9654;</a>");
    client.println("<div></div><a href=\"/B\" class=\"btn btn-b\">&#9660;</a><div></div>");
    client.println("</div></div>");

    // Speed
    client.println("<div class=\"card\"><h2>Speed</h2>");
    client.println("<input type=\"range\" id=\"speedSlider\" min=\"0\" max=\"255\" value=\"" + String(motorSpeed) + "\" onchange=\"window.location='/SPEED?val='+this.value\">");
    client.println("<div class=\"status\">Value: <span id=\"speedVal\">" + String(motorSpeed) + "</span></div>");
    client.println("</div>");

    // Auto mode
    String autoBtnClass = (currentMode == "auto") ? "btn btn-auto on" : "btn btn-auto";
    String autoLabel = (currentMode == "auto") ? "AUTO ON" : "AUTO OFF";
    client.println("<div class=\"card\"><a href=\"/AUTO\" class=\"" + autoBtnClass + "\">" + autoLabel + "</a></div>");

    // VLA
    client.println("<div class=\"card\"><h2>VLA Query</h2>");
    client.println("<form action=\"/VLA\" method=\"get\">");
    client.println("<input type=\"text\" name=\"q\" placeholder=\"e.g. go to the red cup\">");
    client.println("<input type=\"submit\" value=\"Send\" class=\"btn btn-vla\" style=\"width:100%\">");
    client.println("</form></div>");

    // Navigation
    client.println("<div class=\"card\"><h2>Navigation Goal</h2>");
    client.println("<form action=\"/NAV\" method=\"get\">");
    client.println("<input type=\"text\" name=\"q\" placeholder=\"e.g. the blue chair\">");
    client.println("<input type=\"submit\" value=\"Set Goal\" class=\"btn btn-nav\" style=\"width:100%\">");
    client.println("</form>");
    if (navGoal.length() > 0) {
        client.println("<div class=\"status\">Goal: <span>" + navGoal + "</span> | <a href=\"/NAVCLR\" style=\"color:#e94560\">Clear</a></div>");
    }
    client.println("</div>");

    // Text-only Ollama
    client.println("<div class=\"card\"><h2>Ask Ollama</h2>");
    client.println("<form action=\"/ASK\" method=\"get\">");
    client.println("<input type=\"text\" name=\"q\" placeholder=\"Ask a question...\">");
    client.println("<input type=\"submit\" value=\"Ask\" class=\"btn btn-vla\" style=\"width:100%\">");
    client.println("</form></div>");

    // Camera
    client.println("<div class=\"card\"><h2>Camera</h2>");
    client.println("<img src=\"/CAMERA\" alt=\"Camera feed\" id=\"cameraImg\">");
    client.println("</div>");

    // VLA response
    if (lastVLAResponse.length() > 0) {
        client.println("<div class=\"card\"><h2>Last VLA Response</h2>");
        client.println("<div class=\"status\">Action: <span>" + lastVLAResponse + "</span></div>");
        if (lastVLAReason.length() > 0) {
            client.println("<div class=\"status\">Reason: <span>" + lastVLAReason + "</span></div>");
        }
        client.println("</div>");
    }

    // Auto-refresh script
    client.println("<script>");
    client.println("setInterval(function(){");
    client.println("var x=new XMLHttpRequest();");
    client.println("x.onload=function(){");
    client.println("var d=JSON.parse(x.responseText);");
    client.println("document.getElementById('mode').textContent=d.mode;");
    client.println("document.getElementById('motor').textContent=d.motor;");
    client.println("document.getElementById('speed').textContent=d.speed;");
    client.println("document.getElementById('heading').textContent=d.heading;");
    client.println("document.getElementById('dist').textContent=d.distance;");
    client.println("document.getElementById('speedVal').textContent=d.speed;");
    client.println("document.getElementById('speedSlider').value=d.speed;");
    client.println("};");
    client.println("x.open('GET','/STATUS',true);");
    client.println("x.send();");
    client.println("},2000);");

    // Refresh camera every 5 seconds
    client.println("setInterval(function(){");
    client.println("document.getElementById('cameraImg').src='/CAMERA?'+Date.now();");
    client.println("},5000);");
    client.println("</script>");

    client.println("</body></html>");
    client.stop();
}

// ===================== SERIAL COMMAND PARSER =====================
void processSerialCommand(String cmd) {
    cmd.trim();
    if (cmd.length() == 0) return;

    int spaceIdx = cmd.indexOf(' ');
    String verb = (spaceIdx > 0) ? cmd.substring(0, spaceIdx) : cmd;
    String arg  = (spaceIdx > 0) ? cmd.substring(spaceIdx + 1) : "";
    verb.toLowerCase();

    if (verb == "f" || verb == "forward") {
        currentMode = "manual"; motorForward(motorSpeed);
        Serial.println("[CMD] Forward");
    } else if (verb == "b" || verb == "back") {
        currentMode = "manual"; motorBackward(motorSpeed);
        Serial.println("[CMD] Backward");
    } else if (verb == "l" || verb == "left") {
        currentMode = "manual"; motorLeft(motorSpeed);
        Serial.println("[CMD] Left");
    } else if (verb == "r" || verb == "right") {
        currentMode = "manual"; motorRight(motorSpeed);
        Serial.println("[CMD] Right");
    } else if (verb == "s" || verb == "stop") {
        currentMode = "manual"; motorStop();
        Serial.println("[CMD] Stop");
    } else if (verb == "speed") {
        if (arg.length() > 0) {
            motorSpeed = constrain(arg.toInt(), 0, 255);
            Serial.println("[CMD] Speed set to " + String(motorSpeed));
        }
    } else if (verb == "auto") {
        if (currentMode == "auto") { currentMode = "manual"; motorStop(); Serial.println("[CMD] Auto mode OFF"); }
        else { currentMode = "auto"; Serial.println("[CMD] Auto mode ON"); }
    } else if (verb == "vla") {
        if (arg.length() == 0) { Serial.println("[CMD] Usage: vla <prompt>"); return; }
        currentMode = "manual"; motorStop();
        Serial.println("[CMD] VLA query: " + arg);
        String response = ollamaChat(arg, true);
        handleVLAResponse(response);
    } else if (verb == "nav") {
        if (arg.length() == 0) { navGoal = ""; currentMode = "manual"; motorStop(); Serial.println("[CMD] Navigation cleared"); }
        else { navGoal = arg; currentMode = "nav"; Serial.println("[CMD] Navigation goal: " + navGoal); }
    } else if (verb == "ask") {
        if (arg.length() == 0) { Serial.println("[CMD] Usage: ask <prompt>"); return; }
        Serial.println("[CMD] Ask: " + arg);
        String response = ollamaChat(arg, false);
        Serial.println("[VLA] Answer: " + response);
    } else if (verb == "status") {
        Serial.println("=== RoboCar Status ===");
        Serial.println("  Mode:     " + currentMode);
        Serial.println("  Motor:    " + motorState);
        Serial.println("  Speed:    " + String(motorSpeed));
        Serial.println("  Heading:  " + String(currentHeading) + " deg");
        Serial.println("  Distance: " + String(currentDistance) + " mm");
        if (navGoal.length() > 0) Serial.println("  NavGoal:  " + navGoal);
        Serial.println("======================");
    } else if (verb == "help") {
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
    } else {
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
    Serial.println("[INIT] WiFi connected: " + String(WiFi.localIP()));
    digitalWrite(LED_BUILTIN, HIGH);

    // Start web server
    server.begin();
    Serial.println("[INIT] Web server started at http://" + String(WiFi.localIP()));

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
    Serial.println("[INIT] Ready! Open browser to http://" + String(WiFi.localIP()));
    Serial.println("[INIT] Serial commands also available (type 'help')");
    Serial.println();
}

// ===================== LOOP =====================
void loop() {
    // --- Handle web client ---
    WiFiClient webClient = server.available();
    if (webClient) {
        handleClient(webClient);
    }

    // --- Read serial commands ---
    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        processSerialCommand(cmd);
    }

    // --- Read sensors ---
    if (flag_readIR) {
        readIR();
        flag_readIR = false;
    }
    if (flag_readIMU) {
        readIMU();
        flag_readIMU = false;
    }

    // --- Safety stop ---
    if (currentMode == "manual" && currentDistance < OBSTACLE_THRESHOLD) {
        if (motorState != "stop") {
            motorStop();
            Serial.println("[SAFE] Obstacle too close - emergency stop");
        }
    }

    // --- Autonomous mode ---
    if (currentMode == "auto") {
        static unsigned long lastAutoStep = 0;
        if (millis() - lastAutoStep > 2000) {
            autonomousStep();
            lastAutoStep = millis();
        }
    }

    // --- Navigation mode ---
    if (currentMode == "nav") {
        static unsigned long lastNavStep = 0;
        if (millis() - lastNavStep > 3000) {
            navigationStep();
            lastNavStep = millis();
        }
    }

    delay(50);
}
