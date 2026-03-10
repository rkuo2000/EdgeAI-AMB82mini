## Sensors

### [IMU](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/IMU.md)
![](https://github.com/rkuo2000/EdgeAI-AMB82mini/raw/main/assets/MPU6050.png?raw=true)

---
### ToF Distance Sensor

#### [VL53L0X](https://esphome.io/components/sensor/vl53l0x/)
![](https://esphome.io/_astro/vl53l0x.C3JuA-N9_1jUK4p.webp)

---
## Peripherals

### TFT-LCD
![](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/assets/AMB82-mini_button_SPI_TFTLCD.jpg?raw=true)

#### AMB82-mini and QVGA TFT LCD Wiring Diagram
![](https://www.amebaiot.com.cn/wp-content/uploads/2023/01/spi/lcdP01.png)

---
## Motor Control

### [PWM](https://docs.arduino.cc/learn/microcontrollers/analog-output/)
![](https://docs.arduino.cc/54ef6da144b4531dd9ada686a7e67c56/pwm.gif)

```
myservo.attach(8);
```

```
myservo.write(pos);
```

---
### H-bridge Motor Driver IC(全橋式馬達驅動IC)

### TB6612
<p><img width="50%" height="50%" src="https://i0.wp.com/dronebotworkshop.com/wp-content/uploads/2019/12/TB6612FNG-pinout.jpeg"></p>

---
### DRV8833
<p><img width="50%" height="50%" src="https://www.jsumo.com/drv8833-stepper-motor-driver-board-2-channel-4094-14-B.jpg"></p>
<p><img width="50%" height="50%" src="https://jin-hua.com.tw/upload/images/2430000015663-902.jpg"></p>

#### Sketchbook > AMB82-mini > [RoboCar_DRV8833](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Arduino/AMB82-mini/RoboCar_DRV8833_GPIOPWM/RoboCar_DRV8833_GPIOPWM.ino)

---
### Gripper

<p><img width="50%" height="50%" src="https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/assets/Gripper.jpg?raw=true"></p>

#### Examples> AmebaAnalog > [PWM_ServoControl](https://www.amebaiot.com/en/amebapro2-arduino-pwm-servo/)
<p><img width="50%" height="50%" src="https://microcontrollerslab.com/wp-content/uploads/2021/03/MG995-Servo-Motor-pinout-diagram.jpg"></p>
