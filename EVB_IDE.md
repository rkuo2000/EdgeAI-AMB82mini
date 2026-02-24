
## 2. 開發板介紹 (EVB brief)

### [RTL8735B晶片簡介](https://www.realmcu.com/en/Home/Product/RTL8735B-Series)
32-bit Arm v8M, up to 500MHz, 768KB ROM, 512KB RAM, 16MB Flash (MCM embedded DDR2/DDR3L up to 128MB)<br>
802.11 a/b/g/n WiFi 2.4GHz/5GHz, BLE 5.1, *NN Engine 0.4 TOPS*, Crypto Engine, Audio Codec, ...<br>

---
### [HUB 8735 Ultra](https://www.ruten.com.tw/item/show?22434411139868)
#### [https://github.com/ideashatch/HUB-8735](https://github.com/ideashatch/HUB-8735)

<p><img width="25%" height="25%" src="https://robotkingdom.com.tw/wp-content/uploads/2024/09/HUB_8735_Ultra-4-510x680.jpg"></p>

![](https://tie.twtm.com.tw/uploads/exhibit_photos/3ys6pdXKl3QZdiB5fTNFvhQkgevUBtU2hOIVjsjj.jpg)

[![](https://markdown-videos-api.jorgenkh.no/youtube/-_NMUnY0kK4)](https://youtu.be/-_NMUnY0kK4)

---
### [AMB82-mini](https://www.icshop.com.tw/products/368030501864)
#### [Ameba Arduino](https://www.amebaiot.com/en/ameba-arduino-summary/)

<p><img width="25%" height="25%" src="https://github.com/rkuo2000/EdgeAI-AMB82-mini/blob/main/assets/AMB82-mini.png?raw=true"></p>

<p><img src="https://github.com/rkuo2000/EdgeAI-MCU/blob/main/assets/AMB82-MINI_pinout.png?raw=true"></p>

---
## 3. IDE使用介紹 (Arduino-IDE intro)

### [Arduino IDE 2.3.7](https://www.arduino.cc/en/software/) 下載安裝
![](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/assets/Arduino_IDE.png?raw=true)

---
### 偏好設定 (Preferences)
**Hub8735 ultra**<br>
`https://raw.githubusercontent.com/ideashatch/HUB-8735/main/amebapro2_arduino/Arduino_package/ideasHatch.json`<br>

**AMB82-mini**<br>
**main** `https://github.com/Ameba-AIoT/ameba-arduino-pro2/raw/main/Arduino_package/package_realtek_amebapro2_index.json`<br>
**dev** `https://github.com/Ameba-AIoT/ameba-arduino-pro2/raw/dev/Arduino_package/package_realtek_amebapro2_early_index.json`<br>

![](https://github.com/rkuo2000/EdgeAI-AMB82-mini/blob/main/assets/Arduino_IDE_preference.png?raw=true)

[![](https://markdown-videos-api.jorgenkh.no/youtube/-jQDpDFX2ao)](https://youtu.be/-jQDpDFX2ao)

---
### 選定開發板 AMB82-MINI
Tools > Board Manager > Search AMB82 : Realtek Ameba Boards 4.1.0-build20260213<br>
![](https://github.com/rkuo2000/EdgeAI-AMB82-mini/blob/main/assets/Arduino_IDE_BoardManager.png?raw=true)

**Serial-monitor** = `115200` baud <br>

---
### [Getting Started](https://www.amebaiot.com/en/amebapro2-amb82-mini-arduino-getting-started/)
首先將AMB82-mini板子用MicroUSB線 連接至電腦的USB port<br>
確認UART com port (Ubuntu OS需 `sudo chown usrname /dev/ttyUSB0`)

<p><img width="50%" height="50%" src="https://www.amebaiot.com/wp-content/uploads/2022/12/amb82-mini/P03.png"></p>

**燒錄程式碼：** <br>
* 按下UART_DOWNLOAD按鈕, 再按下RESET按鈕, 放開RESET按鈕, 再放開UART_DOWNLOAD按鈕,板子就進入燒錄模式.
* 然後于Arduino IDE上按下燒錄按鍵`Upload`

---
### Realtek AmebaPro2 hardware libraries
`C:\Users\user\AppData\Local\Arduino15\packages\realtek\hardware\AmebaPro2\4.1.0-build20260213\libraries`<br>

`C:\Users\user\AppData\Local\Arduino15\packages\realtek\hardware\AmebaPro2\4.1.0-build20260213\libraries\NeuralNetwork\src`<br>
GenAI.h<br>
GenAI.cpp<br>

---
## 4.基本練習 (Arduino examples)

#### [教學範例程式](https://github.com/rkuo2000/EdgeAI-AmebaPro2/blob/main/Arduino/AMB82-mini)

1. 瀏覽器打開 [EdgeAI-AMB82mini](https://github.com/rkuo2000/EdgeAI-AMB82mini), 點[Code]並選 [Download ZIP]
2. 解壓縮.zip, 並將 Arduino/AMB82-mini 複製到 文件/Arduino下, 成為 `文件/Arduino/AMB82-mini`

#### 基本範例程式練習

#### Examples> 01.Basics > Blink
  
![](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/assets/AMB82-mini_Arduino_examples_01.Basics_Blink.png?raw=true)
    
**Examples> 02.Digitial > GPIO > Button**<br>
  
![](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/assets/AMB82-mini_Arduino_examples_02.Digital_Button.png?raw=true)

程式碼修改：<br>
const int buttonPin = `1`;  // the number of the pushbutton pin<br>
const int ledPin = `LED_BUILTIN`;    // the number of the LED pin<br>

#### Examples> 01.Basic > AnalogReadSerial

![](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/assets/AMB82-mini_Arduino_examples_01.Basics_AnalogReadSerial.png?raw=true)

程式碼修改：Serial.begin(`115200`);<br>
