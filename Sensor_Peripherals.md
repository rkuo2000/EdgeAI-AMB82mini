## 感測器

### 紅外線測距
**Blog**: [VL53L0X飛時測距（ToF）感應器模組（一）：硬體簡介](https://swf.com.tw/?p=1347)<br>
![](https://swf.com.tw/images/hardware/arduino/VL53L0X/VL53L0X_module.png)

* **[VL53L0X v2](https://www.ruten.com.tw/item/show?22425810394279)** 
* **[VL53L1X v2](https://www.ruten.com.tw/item/show?22425810394279)** 

**Datasheet**: [VL53L0X - Time-of-Flight ranging sensor](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/assets/vl53l0x.pdf) <br>

#### Examples> AmebaWire > [VL53L0X > Continunous.ino](https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/ameba_pro2/amb82-mini/Example_Guides/I2C/Retrieve%20Data%20from%20VL53L0X%20IR%20sensor.html)
![](https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/_images/image01163.png)

---
### 慣性感測
[慣性感測元件](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/IMU.md)<br>

**[MPU6050](https://www.ruten.com.tw/item/show?22428017261803)** <br>
![](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/assets/MPU6050.png?raw=true)

##### Examples > AmebaWire > MPU6050 > [MPU6050_DMP6_using_DMP_V6v12.ino](https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/ameba_pro2/amb82-mini/Example_Guides/I2C/MPU6050%20DMP6.html)
![](https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/_images/image01149.png)

---
## 馬達驅動

### PWM

#### Examples> AmebaAnalog > [PWM_ServoControl](https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/ameba_pro2/amb82-mini/Example_Guides/PWM/Servo%20Control.html)
![](https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/_images/image01216.png)

```
myservo.attach(8);
```

```
myservo.write(pos);
```

![](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/assets/Gripper.jpg?raw=true)

---
### H-bridge 全橋式馬達驅動
**TB6612** <br>
![](https://i0.wp.com/dronebotworkshop.com/wp-content/uploads/2019/12/TB6612FNG-pinout.jpeg?w=768&ssl=1)

**DRV8833** <br>
![](https://www.jsumo.com/drv8833-stepper-motor-driver-board-2-channel-4094-14-B.jpg)
![](https://jin-hua.com.tw/upload/images/2430000015663-902.jpg)

---
## 顯示器驅動

### ILI9341 TFT-LCD
![](https://github.com/rkuo2000/EdgeAI-AMB82-mini/blob/main/assets/AMB82-mini_button_SPI_TFTLCD.jpg?raw=true)

**Interface signal names:** <br>
* MOSI: Standard SPI Pin
* MISO: Standard SPI Pin
* SLK: Standard SPI Pin
* CS: Standard SPI Pin
* RESET: Used to reboot LCD.
* D/C: Data/Command. When it is at LOW, the signal transmitted are commands, otherwise the data transmitted are data.
* LED (or BL): Adapt the screen backlight. Can be controlled by PWM or connected to VCC for 100% backlight.
* VCC: Connected to 3V or 5V, depends on its spec.
* GND: Connected to GND.

### AMB82-mini & QVGA TFT-LCD wiring diagram
![](https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/_images/image01224.png)

### 範例程式

#### 1) Examples> AmebaSPI > Camera_2_lcd
*Camera output , then Jpeg Decoder to TFT-LCD<br>*<br>

`Libraries/TJpg_Decoder/src/User_Config.h` need to comment out the following line: <br>
```
//#define TJPGD_LOAD_SD_LIBRARY<br>
```

#### 2) Examples > AmebaSPI > Camera_2_Lcd_JPEGDEC
*Camera output, saved to SDcard, then Jpeg Decoder to read to TFT-LCD*<br>

#### 3) Examples > AmebaSPI > [DisplaySDJPG_ILI9341_TFT](https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/ameba_pro2/amb82-mini/Example_Guides/SPI/Display%20SD%20JPG%20ILI9341%20TFT.html)

#### 4) Examples > AmebaSPI > LCD_Screen_ILI9341_TFT
*LCD Draw Tests*<br>
