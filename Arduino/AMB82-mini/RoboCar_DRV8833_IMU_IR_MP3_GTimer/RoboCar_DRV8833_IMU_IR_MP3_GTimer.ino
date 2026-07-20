/*
   RoboCar : WebServer + DRV8833 + MPU6050 + VL53L0X + MP3 + GTimer
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
#include <GTimer.h>

#define TIMER_ID 0
#define INTERVAL 1000

// nSLEEP pin connected to Vcc
// nFAULT pin kept floating
#define IN1_PIN     5 // IN1 pin GPIO
#define IN2_PIN     6 // IN2 pin PWM
#define IN3_PIN     7 // IN3 pin GPIO
#define IN4_PIN     8 // IN4 pin PWM

AmebaFatFS fs;
String mp3file = "robocar_ready.mp3";

VL53L0X sensor;

char ssid[] = "TCFSTWIFI.ALL";    // your network SSID (name)
char pass[] = "035623116";        // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;     // Indicator of Wifi status

WiFiClient client;

MPU6050 mpu;
//MPU6050 mpu(0x69); // <-- use for AD0 high

// MPU control/status vars
bool dmpReady = false;      // set true if DMP init was successful
uint8_t mpuIntStatus;       // holds actual interrupt status byte from MPU
uint8_t devStatus;          // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;        // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;         // count of all bytes currently in FIFO
uint8_t fifoBuffer[128];    // FIFO storage buffer'

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
int Heading;
int Distance;

String commandState = "stop";

int counter = 0;
bool flag_readIR = false;
bool flag_readIMU = false;

void myhandler(uint32_t data)
{
    data = data;
    counter++;
    Serial.println();
    Serial.print("counter: ");
    Serial.println(counter);

    flag_readIR = true;
    flag_readIMU = true;
}

void init_ir()
{
    // initialize VL53L0X
    sensor.setTimeout(500);
    if (!sensor.init()) {
        Serial.println("Failed to detect and initialize IR sensor!");
        while (1) {
        }
    }

#if defined LONG_RANGE
    // lower the return signal rate limit (default is 0.25 MCPS)
    sensor.setSignalRateLimit(0.1);
    // increase laser pulse periods (defaults are 14 and 10 PCLKs)
    sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
    sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
#endif

#if defined HIGH_SPEED
    // reduce timing budget to 20 ms (default is about 33 ms)
    sensor.setMeasurementTimingBudget(20000);
#elif defined HIGH_ACCURACY
    // increase timing budget to 200 ms
    sensor.setMeasurementTimingBudget(200000);
#endif
}

void init_imu()
{
    Serial.println("initializing MPU...");
    mpu.initialize();

    // verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    // load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(51);
    mpu.setYGyroOffset(8);
    mpu.setZGyroOffset(21);
    mpu.setXAccelOffset(1150);
    mpu.setYAccelOffset(-50);
    mpu.setZAccelOffset(1060);

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // Calibration Time: generate offsets and calibrate our MPU6050
        mpu.CalibrateAccel(6);
        mpu.CalibrateGyro(6);
        Serial.println();
        mpu.PrintActiveOffsets();
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        mpuIntStatus = mpu.getIntStatus();
        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }
}

void GetHeading()                                                                                                                                                   
{
    if (!dmpReady) {
        return;
    }
    // read a packet from FIFO
    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {    // Get the Latest packet
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        Serial.print("Yaw = ");
        Serial.println(int((ypr[0] * 180/M_PI)));
        Heading = int((ypr[0] * 180/M_PI)) + 180;
    }
}


void setup()
{
    Serial.begin(115200);        // initialize serial communication
    Wire.begin();                // configure I2C interface 
    Wire.setClock(400000); 

    pinMode(LED_BUILTIN, OUTPUT);    // set the LED pin mode
    digitalWrite(LED_BUILTIN, LOW);

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

    init_ir();

    init_imu();

    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to Network named: ");
        Serial.println(ssid);    // print the network name (SSID);

        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);
        // wait 10 seconds for connection:
        delay(10000);
    }

    digitalWrite(LED_BUILTIN, HIGH); // web server & mpu are ready
    mp3file = "robocar_ready.mp3";
    playMP3();

    // timer id 0, period 100ms, invoke myhander
    GTimer.begin(TIMER_ID, (1 * 100 * 1000), myhandler);
}


void cmd_B() 
{
  digitalWrite(IN1_PIN, HIGH); 
  analogWrite(IN2_PIN, 5); // dutycycle 0~255
  digitalWrite(IN3_PIN, HIGH); 
  analogWrite(IN4_PIN, 5); // dutycycle 0~255
}

void cmd_F() 
{
  digitalWrite(IN1_PIN, LOW); 
  analogWrite(IN2_PIN, 250); // dutycycle 0~255
  digitalWrite(IN3_PIN, LOW); 
  analogWrite(IN4_PIN, 250); // dutycycle 0~255
}

void cmd_L() 
{
  digitalWrite(IN1_PIN, HIGH); 
  analogWrite(IN2_PIN, 5); // dutycycle 0~255
  digitalWrite(IN3_PIN, LOW); 
  analogWrite(IN4_PIN, 250); // dutycycle 0~255
}

void cmd_R() 
{
  digitalWrite(IN1_PIN, LOW); 
  analogWrite(IN2_PIN, 250); // dutycycle 0~255
  digitalWrite(IN3_PIN, HIGH); 
  analogWrite(IN4_PIN, 5); // dutycycle 0~255  
}

void cmd_S() 
{
  digitalWrite(IN1_PIN, LOW); 
  analogWrite(IN2_PIN, 5);
  digitalWrite(IN3_PIN, LOW); 
  analogWrite(IN4_PIN, 5); // dutycycle 0~255  
}

void IRranger()
{
    Distance = sensor.readRangeSingleMillimeters();
    Serial.print("D = ");
    Serial.println(Distance);
    if (sensor.timeoutOccurred()) {
        Serial.print(" TIMEOUT");
    }
}

void loop()
{
    if (flag_readIR) {
        IRranger();
        flag_readIR = false;
    }

    if (flag_readIMU) {
        GetHeading();
        flag_readIMU = false;
    }
    
    if (Distance < 100) {
        cmd_S();
        commandState="STOP";       
    }    
}

void playMP3()
{
    fs.begin();
    File file = fs.open(String(fs.getRootPath()) + mp3file, MP3);
    file.setMp3DigitalVol(128);     
    file.playMp3();
    file.close();
}
