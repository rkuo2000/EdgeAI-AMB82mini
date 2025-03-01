## IMU - Inertial Measurement Unit (慣性量測單元) 
慣性測量單元是測量物體三軸姿態角以及加速度的裝置。<br>
一般一個IMU內會裝有三軸的陀螺儀和三個方向的加速度計，來測量物體在三維空間中的角速度和加速度，並以此解算出物體的姿態。<br>

### Accelerometer（加速計）
An accelerometer is a device that measures proper acceleration ("gravity force").<br>
an accelerometer at rest on the surface of the Earth will measure an acceleration g= 9.81 m/s2 straight upwards (測量加速度的裝置)<br>

**[ADXL335 3-axis Accelerometer](https://www.taydakits.com/articles/featured-product-adxl335-3-axis-accelerometer)**
![](https://www.taydakits.com/ckeditor_assets/pictures/314/content_capacitorusage.png)
![](https://www.taydakits.com/ckeditor_assets/pictures/315/content_accelmems.jpg)

---
### Gyroscope （陀螺儀）
A gyroscope is a device for measuring or maintaining orientation, based on the principles of angular momentum, it is to measure the rate of changes of the angles (deg/s)
(一種感測與維持方向的裝置，量測角速度)<br>
![](https://upload.wikimedia.org/wikipedia/commons/thumb/d/d5/Gyroscope_operation.gif/220px-Gyroscope_operation.gif)
**MEMS Gyroscope**
![](http://memsjournal.typepad.com/.a/6a00d8345225f869e20148c7d54d63970c-pi)

---
### Magnetic Field Sensor （電子羅盤）
[Resonant Magnetic Field Sensors Based On MEMS Technology](https://www.mdpi.com/1424-8220/9/10/7785/htm)
![](https://www.mdpi.com/sensors/sensors-09-07785/article_deploy/html/images/sensors-09-07785f13-1024.png)

---
### 9-axis IMU（九軸慣性感測器）
![](https://www.systemplus.fr/wp-content/uploads/2019/01/SP19443-IMU-MEMS-Sensor_3.png)
![](https://github.com/rkuo2000/MCU-course/blob/main/images/9-axis_IMUs.png?raw=true)

---
### [Comparing Gyroscopes](https://learn.adafruit.com/adafruit-sensorlab-gyroscope-calibration/comparing-gyroscopes)
**ST ISM330DHC**<br>
Uncalibrated offsets are low, the one we grabbed had 0.006 rad/s (0.35 deg/s) max offset.<br>
Datasheet's Angular rate zero-rate level is typical ±1 deg/s.<br>
No-motion observed noise was an incredibly low  ±0.002 rad/s (±0.06 deg/sec) when running at 104 Hz and no filters on. Check the datasheet for more details!
![](https://cdn-learn.adafruit.com/assets/assets/000/087/544/medium800/sensors_image.png)

**LSM6DSOX**<br>
Uncalibrated offsets are low, the one we grabbed had 0.007 rad/s (0.42 deg/s) max offset.<br>
Datasheet's Angular rate zero-rate level is typical ±1 deg/s.<br>
No-motion observed noise was an incredibly low  ±0.003 rad/s (±0.17 deg/sec) when running at 104 Hz and no filters on. Check the datasheet for more details!
![](https://cdn-learn.adafruit.com/assets/assets/000/087/552/medium800/sensors_image.png?1580015078)

**LSM6DS33**<br>
Uncalibrated offsets are fair, the one we grabbed had 0.034 rad/s (2 deg/s) max offset.<br>
Datasheet's Angular rate zero-rate level is typical ±10 deg/s!<br>
No-motion observed noise was ±0.015 rad/s (±0.85 deg/sec) when running at 104 Hz and no filters on. Check the datasheet for more details!
![](https://cdn-learn.adafruit.com/assets/assets/000/087/546/medium800/sensors_image.png?1579996592)

**LSM9DS1**<br>
Uncalibrated offsets are pretty good, the one we grabbed had 0.02 rad/s (1.2 deg/s) max offset.<br>
Datasheet's Angular rate zero-rate level is typical ±30 deg/s!<br>
However, we noticed spikes of gyro data well outside the expected range. When those spikes are ignore, the no-motion observed noise was +- 0.007 rad/s (±0.4 deg/sec) at 1 KHz with the 408 Hz bandwidth filter on. Check the datasheet for more details!
![](https://cdn-learn.adafruit.com/assets/assets/000/087/580/medium800/sensors_image.png?1580073474)
![](https://cdn-learn.adafruit.com/assets/assets/000/087/579/medium800/sensors_image.png?1580073025)

**MPU-6050**<bR>
Uncalibrated offsets are fair, the one we grabbed had 0.04 rad/s (2.3 deg/s) max offset.<br>
Datasheet's Angular rate zero-rate level is typical ±20 deg/s!<br>
No-motion observed noise was +- 0.05 rad/s (±0.29 deg/sec) with the 260 Hz bandwidth filter on. Check the datasheet for more details!
![](https://cdn-learn.adafruit.com/assets/assets/000/087/545/medium800/sensors_image.png?1579996083)

**NXP FXAS21002**<br>
Uncalibrated offsets are pretty good - the one we grabbed had 0.01 rad/s (0.57 deg/s) max offset.<br>
Datasheet's Angular rate zero-rate level is typical post-mount ±50 LSB (NOT deg/s) - at 250 deg/s rate, that translates to ±0.4 deg/s<br>
No-motion observed noise was +- 0.01 rad/s (±0.55 deg/sec) at 100 Hz output. Check the datasheet for more details!
![](https://cdn-learn.adafruit.com/assets/assets/000/087/547/medium800/sensors_image.png?1579997018)

**ICM-20649**<br>
Uncalibrated offsets are not bad, the one we grabbed had 0.023 rad/s (1.3 deg/s) max offset.<br>
Datasheet's Angular rate zero-rate level is typical ±5 deg/s<br>
No-motion observed noise was +- 0.015 rad/s (±0.86 deg/sec) at 1.1KHz. Check the datasheet for more details!
![](https://cdn-learn.adafruit.com/assets/assets/000/087/553/medium800/sensors_image.png?1580015410)

---
## AHRS (Attitude and Heading Reference System)

### Euler Angles
![](https://upload.wikimedia.org/wikipedia/commons/thumb/a/a1/Eulerangles.svg/300px-Eulerangles.svg.png)

---
### Yaw Pitch Roll (YPR)
![](https://www.researchgate.net/publication/329603549/figure/fig1/AS:703246263926788@1544678383831/An-illustration-of-the-three-angles-yaw-pitch-and-roll-returned-by-the-Hyper-IMU.png)

---
### The coordinate system and rotation conventions 
![](https://danceswithcode.net/engineeringnotes/rotations_in_3d/images/fig01.png)
![](https://github.com/rkuo2000/MCU-course/blob/main/images/XYZ-YPR-Euler.png?raw=true)

---
### InvenSense MPU6050 & MPU9250
* **MPU6050**<br>
[MPU-6000 and MPU-6050 Product Specification Revision 3.4](https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf)<br>
[MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2](https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf)<br>
![](https://github.com/rkuo2000/MCU-course/blob/main/images/MPU6050.jpeg?raw=true)
* **MPU9250** [Datasheet](https://invensense.tdk.com/download-pdf/mpu-9250-datasheet/)
![](https://github.com/rkuo2000/MCU-course/blob/main/images/MPU9250.jpeg?raw=true)
* **BNO080** [Datasheet](https://cdn.sparkfun.com/assets/1/3/4/5/9/BNO080_Datasheet_v1.3.pdf)
![](https://github.com/rkuo2000/MCU-course/blob/main/images/BNO080.png?raw=true)

---
## Arduino Library: MPU6050
![](https://github.com/rkuo2000/MCU-course/blob/main/images/Arduino_Library_MPU6050.png?raw=true)
![](https://github.com/rkuo2000/MCU-course/blob/main/images/Example_ESP32_MPU6050.jpg?raw=true)

* Download [Processing](https://processing.org/download) & Install
  - Tools> add Tool> Library> Toxi

---  
### Sketchbook> IMU> MPU6050_DMP6_plane
* #define OUTPUT_TEAPOT
![](https://github.com/rkuo2000/MCU-course/blob/main/images/Examples_MPU6050_DMP6_using_DMP_V6v12.png?raw=true)
* click ./MPUplane/MPUPlane.pde
* modify port name: `String portName = "COM3";`
* press Run
![](https://github.com/rkuo2000/MCU-course/blob/main/images/Examples_MPU6050_DMP6_using_DMP_V6v12_pde.png?raw=true)

---
### Sketchbook> IMU> MPU6050_DMP_Teapot
![](https://github.com/rkuo2000/MCU-course/blob/main/images/Sketch_MPU6050_DMP_Teapot.png?raw=true)
* click ./MPU6050Teapot/MPU6050Teapot.pde
* modify port name: `String portName = "COM3";`
* press Run

---
### Sketchbook> IMU>MPU6050_KalmanFilter
[MPU6050_KalmanFilter.ino](https://github.com/rkuo2000/arduino/blob/master/examples/IMU/MPU6050_KalmanFilter/MPU6050_KalmanFilter.ino)<br>

* **Initialize IMU & get Acceleration**
```
imu.initialize();
delay(100);
imu.getAcceleration(&accX, &accY, &accZ);
```

* **atan2**
![](https://upload.wikimedia.org/wikipedia/commons/thumb/a/ad/Atan2definition.svg/220px-Atan2definition.svg.png)
![](https://github.com/rkuo2000/MCU-course/blob/main/images/atan2.png?raw=true)

* **Complementary Filter**
![](https://www.mouser.tw/images/microsites/sensor_stmicro_fig2.jpg)

```
// Calculate gyro angle without any filter
gyroXangle += gyroXrate * dt
gyroYangle += gyroYrate * dt

// Calculate the angle using a Complimentary filter
compAngleX = 0.97 * (compAngleX + gyroXrate * dt) + 0.03 * accXangle
compAngleY = 0.97 * (compAngleY + gyroYrate * dt) + 0.03 * accYangle
```
![](http://www.pieter-jan.com/images/Complementary_Filter.png)

* **Kalman Filter**<br>
[Kalman.cpp](https://github.com/rkuo2000/arduino/blob/master/examples/IMU/MPU6050_KalmanFilter/Kalman.cpp)
![](https://upload.wikimedia.org/wikipedia/commons/thumb/4/4e/Kalman-filter_en.svg/800px-Kalman-filter_en.svg.png?20160621130330)

```
float Q_angle = 0.001f;
float Q_bias = 0.003f;
float R_measure = 0.03f;

float angle = 0.0f; // Reset the angle
float bias = 0.0f; // Reset bias

float P[2][2] = {0.0, 0.0, 0.0, 0.0};
    
float Kalman::getAngle(float newAngle, float newRate, float dt) {
    rate = newRate - bias;
    angle += dt * rate;

    // Update estimation error covariance - Project the error covariance ahead
    /* Step 2 */
    P[0][0] += dt * (dt*P[1][1] - P[0][1] - P[1][0] + Q_angle);
    P[0][1] -= dt * P[1][1];
    P[1][0] -= dt * P[1][1];
    P[1][1] += Q_bias * dt;

    // Discrete Kalman filter measurement update equations - Measurement Update ("Correct")
    // Calculate Kalman gain - Compute the Kalman gain
    /* Step 4 */
    float S = P[0][0] + R_measure; // Estimate error
    /* Step 5 */
    float K[2]; // Kalman gain - This is a 2x1 vector
    K[0] = P[0][0] / S;
    K[1] = P[1][0] / S;

    // Calculate angle and bias - Update estimate with measurement zk (newAngle)
    /* Step 3 */
    float y = newAngle - angle; // Angle difference
    /* Step 6 */
    angle += K[0] * y;
    bias += K[1] * y;

    // Calculate estimation error covariance - Update the error covariance
    /* Step 7 */
    float P00_temp = P[0][0];
    float P01_temp = P[0][1];

    P[0][0] -= K[0] * P00_temp;
    P[0][1] -= K[0] * P01_temp;
    P[1][0] -= K[1] * P00_temp;
    P[1][1] -= K[1] * P01_temp;

    return angle;
};  
```

---
## Arduino Library: MPU9250
![](https://github.com/rkuo2000/MCU-course/blob/main/images/Arduino_Library_MPU9250.png?raw=true)

### Sketchbook>MPU9250_DMP_Teapot
![](https://github.com/rkuo2000/MCU-course/blob/main/images/Example_ESP32_MPU9250.jpg?raw=true)
![](https://github.com/rkuo2000/MCU-course/blob/main/images/Sketch_MPU9250_DMP_Teapot.png?raw=true)
* click MPU9250Teapot/MPU9250Teapot.pde
![](https://github.com/rkuo2000/MCU-course/blob/main/images/Sketch_MPU9250_DMP_Teapot_Processing4.0b7.png?raw=true)

---
### Sketchbook>MPU9250_BasicAHRS2
[x-io Technologies publications](https://x-io.co.uk/publications/)

---
### Sketchbook> Robot> Esp32Copter
[Esp32Copter](https://github.com/rkuo2000/arduino/blob/master/examples/Robot/Esp32Copter/README.md) 
![](https://github.com/rkuo2000/arduino/blob/master/examples/Robot/Esp32Copter/DSC02360.jpg?raw=true)
![](https://github.com/rkuo2000/arduino/blob/master/examples/Robot/Esp32Copter/DSC02364.jpg?raw=true)

---
## [Ardupilot](https://ardupilot.org/)
**[Github](https://github.com/ArduPilot/ardupilot/)**

### Mission Planner Ground Control Station
![](https://ardupilot.org/planner/_images/mp_hud_full.jpg)

**view of HUD**
![](https://ardupilot.org/planner/_images/mp_hud_detail.jpg)
1. Air speed ( Ground speed if no airspeed sensor is fitted )
2. Crosstrack error and turn rate (T)
3. Heading direction
4. Bank angle
5. Telemetry connection link quality (averaged percentage of good packets)
6. GPS time
7. Altitude ( blue bar is rate of climb )
8. Air speed
9. Ground speed
10. Battery status
11. Artificial Horizon
12. Aircraft Attitude
13. GPS Status
14. Distance to Waypoint > Current Waypoint Number
15. Current Flight Mode

<br>
<br>

*This site was last updated {{ site.time | date: "%B %d, %Y" }}.*
