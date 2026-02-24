## 感測器

### 紅外線測距
**Blog**: [VL53L0X飛時測距（ToF）感應器模組（一）：硬體簡介](https://swf.com.tw/?p=1347)<br>
![](https://swf.com.tw/images/hardware/arduino/VL53L0X/VL53L0X_module.png)

* **[VL53L0X v2](https://www.ruten.com.tw/item/show?22425810394279)** 
* **[VL53L1X v2](https://www.ruten.com.tw/item/show?22425810394279)** 

**Datasheet**: [VL53L0X - Time-of-Flight ranging sensor](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/assets/vl53l0x.pdf) <br>

#### Examples> AmebaWire > VL53L0X > Continunous.ino** <br>
```
#include <Wire.h>
#include <VL53L0X_IR_libraries/VL53L0X.h>

VL53L0X sensor;

void setup()
{
    Serial.begin(115200);
    Wire.begin();

    sensor.setTimeout(500);
    if (!sensor.init()) {
        Serial.println("Failed to detect and initialize sensor!");
        while (1) {
        }
    }
    sensor.startContinuous();
}

void loop()
{
    Serial.print(sensor.readRangeContinuousMillimeters());
    if (sensor.timeoutOccurred()) {
        Serial.print(" TIMEOUT");
    }
    Serial.println();
}
```

---
### 慣性感測
[慣性感測元件](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/IMU.md)<br>

**[MPU6050](https://www.ruten.com.tw/item/show?22428017261803)** <br>
![](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/assets/MPU6050.png?raw=true)

##### Sketchbook > AMB82-mini > [MPU6050-DMP6v12](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Arduino/AMB82-mini/MPU6050_DMP6v12)

---
## 馬達驅動

### PWM
[PWM - Servo Control](https://www.amebaiot.com/en/amebapro2-arduino-pwm-servo/) <br>

#### Examples> AmebaAnalog > PWM_ServoControl
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

**[SPI - LCD Screen ILI9341 TFT](https://www.amebaiot.com/en/amebapro2-arduino-spi-lcd/)** <br>

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

**AMB82 MINI and QVGA TFT LCD Wiring Diagram:** <br>
![](https://www.amebaiot.com/wp-content/uploads/2023/01/spi/lcdP01.png)

### 範例程式

#### Sketchbook> AMB82-mini > [HTTP_Post_ImageText_TFTLCD](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Arduino/AMB82-mini/HTTP_Post_ImageText_TFTLCD)

#### Exmples> AmebaSPI > Camera_2_lcd
*Camera output , then Jpeg Decoder to TFT-LCD<br>*<br>
compilation error: need to modify `Libraries/TJpg_Decoder/src/User_Config.h`<br>
//#define TJPGD_LOAD_SD_LIBRARY<br>

#### Exmples > AmebaSPI > Camera_2_Lcd_JPEGDEC
*Camera output, saved to SDcard, then Jpeg Decoder to read to TFT-LCD*<br>

### Exmples > AmebaSPI > LCD_Screen_ILI9341_TFT
*LCD Draw Tests*<br>
