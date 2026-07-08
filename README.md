# EdgeAI AMB82mini

## 1. AMB82-mini 開發板介紹

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

<p><img width="25%" height="25%" src="https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/assets/AMB82-mini.png?raw=true"></p>

<p><img src="https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/assets/AMB82-MINI_pinout.png?raw=true"></p>

---
## 2. IDE使用介紹 (Arduino-IDE intro)

### [Arduino IDE 2.3.10](https://www.arduino.cc/en/software/) 下載安裝

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
### 基本練習 (Arduino examples)

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

---
## 3. 通訊硬體界面 

### BLE (低功耗藍牙）

#### Examples> AmebaBLE > BLEWiFiConfig
* Google Play Store: https://play.google.com/store/apps/details?id=com.rtk.btconfig
* Apple App Store: https://apps.apple.com/sg/app/easy-wifi-config/id1194919510
<p>
<img width="25%" height="25%" src="https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/_images/image02110.png">
<img width="25%" height="25%" src="https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/_images/image0392.png">
<img width="25%" height="25%" src="https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/_images/image0478.png">
<img width="25%" height="25%" src="https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/_images/image0565.png">
<img width="25%" height="25%" src="https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/_images/image0660.png">
<img width="25%" height="25%" src="https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/_images/image0756.png">
</p>

---
#### Examples> AmebaBLE > BLEUartClient
[BLE UART Client](https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/ameba_pro2/amb82-mini/Example_Guides/BLE/BLE%20UART%20Client.html)<br>

---
#### Examples> AmebaBLE > DHT_Over_BLEUart
[DHT over BLE UART](https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/ameba_pro2/amb82-mini/Example_Guides/BLE/DHT%20over%20BLE%20UART.html)<br>
![](https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/_images/image02103.png)

---
#### Examples> AmebaBLE > BLEWifiConfig
[WiFi Config Service](https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/ameba_pro2/amb82-mini/Example_Guides/BLE/WiFi%20Config%20Service.html)<br>
![](https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/_images/image0392.png)

---
#### Examples> AmebaBLE > BLEV7RC_CAR_VIDEO
[BLE V7RC](https://www.amebaiot.com/zh/amebad-arduino-ble-v7rc/)<br>

* [V7RC APP 介紹](https://hackmd.io/@accomdemy/v7rc)<br>
* 安裝[手機App V7RC](https://play.google.com/store/apps/details?id=com.v7idea.v7rcliteandroidsdkversion&hl=en)<br>
<p>
<img width="50%" height="50%" src="https://www.amebaiot.com/wp-content/uploads/2023/08/ble-v7rc/p1_zh.png">
<img width="50%" height="50%" src="https://www.amebaiot.com/wp-content/uploads/2023/08/ble-v7rc/p2_zh.png">
</p>

---
### WiFi（無線網路）

### Examples > WiFi > SimpleTCPServer
[WiFi - Simple TCP Server](https://www.amebaiot.com/en/amebapro2-arduino-server-client/)<br>

### Examples > WiFi > SimpleHttpWeb > ReceiveData
[WiFi - Simple Http Server to Receive Data](https://www.amebaiot.com/en/amebapro2-arduino-web-server-status/)<br>

### Sketchbook > AMB82-mini > WebServer_ControlLED
[Sketchbook> WebServer_ControlLED](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Arduino/AMB82-mini/WebServer_ControlLED/WebServer_ControlLED.ino)<br>
![](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/assets/AMB82-mini_Arduino_Sketch_WebServer_ControlLED.png?raw=true)

### Sketchbook > AMB82-min > BLEConfig_WebServer_ControlLED
[Sketchbook> BLEConfig_WebServer_ControlLED](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Arduino/AMB82-mini/BLECofnig_WebServer_ControlLED/BLEConfig_WebServer_ControlLED.ino)<br>

### Examples > AmebaFileSystem > ReadHTMLFile
[Read and display HTML file from SD card](https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/ameba_pro2/amb82-mini/Example_Guides/File%20System/Read%20and%20display%20HTML%20file%20from%20SD%20card.html)

---
### [MQTT通訊協定](https://mqtt.org/)
MQTT is an OASIS standard messaging protocol for the Internet of Things (IoT)<br>
[How MQTT Works -Beginners Guide](http://www.steves-internet-guide.com/mqtt-works/)<br>

---
#### Examples> AmebaMQTTClient > MQTT_basic
[MQTT - Set up MQTT Client to Communicate with Broker](https://www.amebaiot.com/en/amebapro2-arduino-mqtt-upload-listen/)<br>
![](https://www.amebaiot.com/wp-content/uploads/2023/06/network/mqtt1-1.png)

---
#### MQTT App
**Prompt:** `make a HTTP Server html to input MQTT topic and text to publish through Paho-MQTT test.mosquitto.org`<br>

**[Paho-MQTT Publisher](https://g.co/gemini/share/ae62311518bd)** <br>
![](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/assets/Gemini_Canvas_Paho-MQTT_Text_Publisher.png?raw=true)

---
### OTA (Over-The-Air)
To install Ameba-OTA-UI, read [https://github.com/Ameba-AioT/ameba-OTA-UI](https://github.com/Ameba-AioT/ameba-OTA-UI)<br>

### Examples > AmebaOTA > OTA
[Ameba Over-the-Air Firmware Update](https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/ameba_pro2/amb82-mini/Example_Guides/OTA/Ameba%20Over-the-Air%20Firmware%20Update.html)<br>

### Examples > AmebaOTA > SDCardOTA
[Ameba SDCard OTA Firmware Update](https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/ameba_pro2/amb82-mini/Example_Guides/OTA/Ameba%20SDCard%20OTA%20Firmware%20Update.html)<br>

---
## 4. 感測器與週邊裝置

### Sensors

### [IMU](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/IMU.md)
![](https://github.com/rkuo2000/EdgeAI-AMB82mini/raw/main/assets/MPU6050.png?raw=true)

---
#### ToF Distance Sensor (紅外線測距感測器)

#### [VL53L0X](https://esphome.io/components/sensor/vl53l0x/)
![](https://esphome.io/_astro/vl53l0x.C3JuA-N9_1jUK4p.webp)

---
### Peripherals

#### TFT-LCD (液晶顯示器)
![](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/assets/AMB82-mini_button_SPI_TFTLCD.jpg?raw=true)

#### AMB82-mini and QVGA TFT LCD Wiring Diagram
![](https://www.amebaiot.com.cn/wp-content/uploads/2023/01/spi/lcdP01.png)

---
### Motor Driver (馬達驅動器)

### [PWM](https://docs.arduino.cc/learn/microcontrollers/analog-output/)
![](https://docs.arduino.cc/54ef6da144b4531dd9ada686a7e67c56/pwm.gif)

```
myservo.attach(8);
```

```
myservo.write(pos);
```

---
#### TB6612
<p><img width="50%" height="50%" src="https://i0.wp.com/dronebotworkshop.com/wp-content/uploads/2019/12/TB6612FNG-pinout.jpeg"></p>

---
#### DRV8833
<p><img width="50%" height="50%" src="https://www.jsumo.com/drv8833-stepper-motor-driver-board-2-channel-4094-14-B.jpg"></p>
<p><img width="50%" height="50%" src="https://jin-hua.com.tw/upload/images/2430000015663-902.jpg"></p>

#### Sketchbook > AMB82-mini > [RoboCar_DRV8833](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Arduino/AMB82-mini/RoboCar_DRV8833_GPIOPWM/RoboCar_DRV8833_GPIOPWM.ino)

---
#### Gripper (夾具)

<p><img width="50%" height="50%" src="https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/assets/Gripper.jpg?raw=true"></p>

#### Examples> AmebaAnalog > [PWM_ServoControl](https://www.amebaiot.com/en/amebapro2-arduino-pwm-servo/)
<p><img width="50%" height="50%" src="https://microcontrollerslab.com/wp-content/uploads/2021/03/MG995-Servo-Motor-pinout-diagram.jpg"></p>

---
## 5. 影像串流 (Video Streaming) 
* [Amebapro2 AMB82-mini Arduino Example Guides](https://www.amebaiot.com/en/amebapro2-amb82-mini-arduino-peripherals-examples)

### Video-Only

#### Examples> AmebaMultimedia > StreamRTSP > VideoOnly

#### Sketchbook> [RTSP_VideoOnly](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Arduino/AMB82-mini/RTSP_VideoOnly)
[![](https://markdown-videos-api.jorgenkh.no/youtube/OmAnWOmt6WQ)](https://youtu.be/OmAnWOmt6WQ)

---
### WebSocket Viewer
**Examples > AmebaMultimedia > [WebSocketViewer](https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/ameba_pro2/amb82-mini/Example_Guides/Multimedia/Websocket%20Viewer.html)** <br>
![](https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/_images/image04120.png)

---
### [動作偵測 (motion detection)](https://www.amebaiot.com/en/amebapro2-arduino-video-motion/)

#### Examples> AmebaMultimedia > MotionDetection > LoopPostProcessing
* 修改ssid, passwd, 後燒錄到AMB82-mini,
* 按reset後程式即開始運行, 用serial-monitor 查看顯示串流網址
* 啟動手機或電腦上之VLC player, 設定RTSP串流網址
<p><img width="50%" height="50%" src="https://www.amebaiot.com/wp-content/uploads/2023/01/video/motionP06.png"></p>

---
## 6. 音頻應用

### 音頻環回測試

#### Examples> AmebaMultimedia > Audio > [LoopbackTest](https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/ameba_pro2/amb82-mini/Example_Guides/Multimedia/Audio/Loopback%20Test.html)

![](https://github.com/rkuo2000/EdgeAI-AMB82-mini/blob/main/assets/AMB82-mini_button_audiojack.jpeg?raw=true)

---
### 播放MP3
AMB82-mini + PAM8403 + 4ohm 3W speaker<br>

![](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/assets/AMB82-mini_PAM8403.jpg?raw=true)

#### Examples > AmebaMultimedia > [SDCardPlayMP3](https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/ameba_pro2/amb82-mini/Example_Guides/Multimedia/Play%20MP3%20with%20SD%20card.html)

#### Sketchbook> AMB82-mini > [SDCardPlayMP_All](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Arduino/AMB82-mini/SDcardPlayMP3_All/SDcardPlayMP3_All.ino)
*.mp3 files stored under mp3 directory*<br>
    
---
### 音頻串流 (Audio Streaming)

#### Examples > AmebaMultimedia > Audio > [RTSPAudioStream](https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/ameba_pro2/amb82-mini/Example_Guides/Multimedia/Audio/RTSP%20Audio%20Stream.html)

---
### MP4錄音 (Audio Recording)

#### Examples> AmebaMultimedia > Audio > AudioTriggerRecording

#### Examples> AmebaMultimedia > RecordMP4 > AudioOnly

---
### 音頻分類 (Audio Classification)

#### Examples> AmebaNN > [AudioClassification](https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/ameba_pro2/amb82-mini/Example_Guides/Neural%20Network/Audio%20Classification.html)
The default pretrained model can recognise 521 different types of audios in total.

[YAMNet](https://codimd.mcl.math.ncu.edu.tw/s/hoOqEgBSf)<br>
[![](https://markdown-videos-api.jorgenkh.no/youtube/oi8ML6aJcvI)](https://youtu.be/oi8ML6aJcvI)

---
### 聲音播放 (Play MP3)
* Examples > AmebaMultimedia > SDCardPlayMP3
* Sketchbook> AMB82-mini > [SDCardPlayMP_All](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Arduino/AMB82-mini/SDcardPlayMP3_All/SDcardPlayMP3_All.ino)

---
## 7. 語音應用

### 語音合成 (TTS)
#### Examples > AmebaNN > MultimediaAI > TextToSpeech (google-tts)

---
### 語音辨識 (STT)

#### 聲音偵測 (VAD)

#### Examples> AmebaMultimedia > Audio > AudioTriggerRecording

#### Examples> AmebaMultimedia > Audio > SoundDetectorTrigger

---
### 雲端語音辨識 
*ffmpeg.exe is needed for Windows to run Whisper* <br>

#### Examples > AmebaNN > MultimediaAI > GenAISpeech_Whisper (Whisper)

#### Examples > AmebaNN > MultimediaAI > GenAISpeech_Gemini  (Gemini)

---
### 本地PC語音辨識
*[AmebaPro2 Whisper LLM_server](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/AmebaPro2_server/AmebaPro2_Whisper_LLM_server.py)* <br>
* Examples -> AmebaHttp > HTTP_Post_MP4_Whisper_server
* Examples -> AmbeaHttp > HTTP_Post_MP4_Whisper_server_FastAPI

---
## 8. 神經網路模型 (AmebaNN)

### [人臉辨識與識別](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Face.md)

#### [人臉檢測範例](https://www.amebaiot.com/en/amebapro2-arduino-neuralnework-face-detection/)
#### Examples> AmebaNN > RTSPFaceDetection

[![](https://markdown-videos-api.jorgenkh.no/youtube/KD95JH6gVew)](https://youtu.be/KD95JH6gVew)

<p><img width="50%" height="50%" src="https://www.amebaiot.com/wp-content/uploads/2023/01/neuralnetwork/facedetectP06.png"></p>

---
#### [人臉識別範例](https://www.amebaiot.com/en/amebapro2-arduino-neuralnework-face-recognition/)
#### Examples> AmebaNN > RTSPFaceRecognition

[![](https://markdown-videos-api.jorgenkh.no/youtube/GGOIQmMfeF8)](https://youtu.be/GGOIQmMfeF8)

Serial_monitor: `REG=RKUO`<br>
<p><img width="50%" height="50%" src="https://github.com/rkuo2000/EdgeAI-AMB82-mini/blob/main/assets/FaceRecognition_REG_RKUO.jpeg?raw=true"></p>

* Enter the command **REG=Name** to give the targeted face a name.
* Enter the command **DEL=Name** to delete a certain registered face. For example, `DEL=SAM`
* Enter the command **BACKUP** to save a copy of registered faces to flash.
* If a backup exists, enter the command **RESTORE** to load registered faces from flash.
* Enter the command **RESET** to forget all previously registered faces. 

---
### 影像分類 (Image Classification)

#### Examples > AmebaNN > [RTSPImageClassification](https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/ameba_pro2/amb82-mini/Example_Guides/Neural%20Network/Image%20Classification.html)

[![](https://markdown-videos-api.jorgenkh.no/youtube/c3XGkc9ShwQ)](https://youtu.be/c3XGkc9ShwQ)

---
#### Garbage模型訓練與檔案轉換

#### 模型訓練： [kaggle.com/rkuo2000/garbage-cnn](https://www.kaggle.com/code/rkuo2000/garbage-cnn)
required in kaggle for AmebaPro2
1) pip install tensorflow==2.14.1
2) model.save('garbage_cnn.h5', include_optimizer=False)

#### 模型轉換：[AMB82 AI Model Conversion](https://www.amebaiot.com/en/amebapro2-ai-convert-model/)
1. Download garbage_cnn.h5 from [kaggle.com/rkuo2000/garbage-cnn](https://www.kaggle.com/code/rkuo2000/garbage-cnn) `Output`
2. Compress garbage_cnn.h5 to garbage_cnn.zip
3. Go to [Amebapro2 AI convert model](https://www.amebaiot.com/en/amebapro2-ai-convert-model/), fill up your E-mail
4. Upload garbage_cnn.zip
5. Upload one (.jpg) test picture (EX. glass100.jpg from [Garbage dataset](https://www.kaggle.com/datasets/asdasdasasdas/garbage-classification))
6. Email will be sent to you for the link of `network_binary.nb`

#### 程式範例：RTSP_GarbageClassification.ino
1. click the recieved Email link to download `network_binary.nb`
2. create NN_MDL folder in SDcard, save network_binary.nb under NN_MDL folder, and rename it to `imgclassification.nb`
3. plugin SDcard back to AMB82-MINI
4. modify Sketch RTSP_GarbageClassification.ino 
   1) modify SSID and PASSWD
   2) modify imgclass.modelSelect (change DEFAULT_IMGCLASS to CUSTOMIZED_IMGCLASS)
5. burn code into board AMB82-MINI, and run it with VLC player streaming
<p><img width="75%" height="75%" src="https://github.com/rkuo2000/EdgeAI-AmebaPro2/raw/main/assets/RTSP_GarbageClassification.png"><img width="25%" height="25%" src="https://github.com/rkuo2000/EdgeAI-AmebaPro2/raw/main/assets/RTSP_GarbageClassification_VLCplayer.jpeg"></p>

---
### 物件偵測 (Object Detection)
[Roboflow Datasets](https://universe.roboflow.com/)<br>

#### Examples > AmebaNN > [ObjectDetectionLoop](https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/ameba_pro2/amb82-mini/Example_Guides/Neural%20Network/Object%20Detection.html)
![](https://ameba-doc-arduino-sdk.readthedocs-hosted.com/en/latest/_images/image0687.png)

---
#### Kaggle 範例:
* [YOLOv7 FaceMask](https://www.kaggle.com/code/rkuo2000/yolov7-facemask)
* [YOLOv7 Pothole](https://www.kaggle.com/code/rkuo2000/yolov7-pothole)
* [YOLOv7 Sushi](https://www.kaggle.com/code/rkuo2000/yolov7-sushi)
* [YOLOv7 RefrigFood](https://www.kaggle.com/code/rkuo2000/yolov7-refrigfood)
* [YOLOv7 TrafficSign](https://www.kaggle.com/code/rkuo2000/yolov7-trafficsign)
* [YOLOv7 Tanks](https://www.kaggle.com/code/rkuo2000/yolov7-tanks)
* [YOLOv7 reparm](https://www.kaggle.com/code/rkuo2000/yolov7-reparam)

---
### Pothole模型訓練與檔案轉換

#### 模型訓練： [kaggle.com/rkuo2000/yolov7-pothole](https://www.kaggle.com/code/rkuo2000/yolov7-pothole)
1) repro [https://github.com/WongKinYiu/yolov7](https://github.com/WongKinYiu/yolov7)
2) create pothole.yaml
`%%writefile data/pothole.yaml`<br>
```
train: ./Datasets/pothole/train/images
val:  ./Datasets/pothole/valid/images
test: ./Datasets/pothole/test/images

# Classes
nc: 1  # number of classes
names: ['pothole']  # class names
```

3) YOLOv7-Tiny Fixed Resolution Training
```
!sed -i "s/nc: 80/nc: 1/" cfg/training/yolov7-tiny.yaml
!sed -i "s/IDetect/Detect/" cfg/training/yolov7-tiny.yaml
```

#### 模型轉換：[AMB82 AI Model Conversion](https://www.amebaiot.com/en/amebapro2-ai-convert-model/)
1. Download `best.pt` from [kaggle.com/rkuo2000/yolov7-pothole](https://www.kaggle.com/code/rkuo2000/yolov7-pothole)
2. Compress best.pt to `best.zip`
3. Go to [Amebapro2 AI convert model](https://www.amebaiot.com/en/amebapro2-ai-convert-model/), fill up your E-mail
4. Upload best.zip
5. Upload one (.jpg) test picture (EX. pothole_test.jpg from [Pothole dataset](https://www.kaggle.com/datasets/apoorvchaudhary/pothole))
6. Email will be sent to you for the link of `network_binary.nb`

#### 程式範例：RTSP_YOLOv7_Pothole_Detection.ino
1. click the recieved Email link to download `network_binary.nb`
2. create NN_MDL folder in SDcard, save network_binary.nb under NN_MDL folder, and rename it to `yolov7_tiny.nb`
3. plugin SDcard back to AMB82-MINI
4. modify Sketch RTSP_YOLOv7_Pothole_Detection.ino 
   1) modify SSID and PASSWD
   2) modify ObjDet.modelSelect(OBJECT_DETECTION, CUSTOMIZED_YOLOV7TINY, NA_MODEL, NA_MODEL);
5. burn code into board AMB82-MINI, and run it with VLC player streaming
<p><img width="50%" height="50%" src="https://github.com/rkuo2000/EdgeAI-AmebaPro2/raw/main/assets/RTSP_YOLOv7_Pothole.png"><img width="50%" height="50%" src="https://github.com/rkuo2000/EdgeAI-AmebaPro2/raw/main/assets/RTSP_YOLOv7_Pothole_VCplayer.jpeg"></p>

---
#### AMB82 Mini - 物件偵測範例
[![](https://markdown-videos-api.jorgenkh.no/youtube/EvryVoQyqqk)](https://youtu.be/EvryVoQyqqk)

[RTSP_YOLOv7_Pothole](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Arduino/AMB82-mini/RTSP_YOLOv7_Pothole)<br>
[RTSP_YOLOv7_Sushi](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Arduino/AMB82-mini/RTSP_YOLOv7_Sushi)<br>

---
#### AMB82 Mini - SD卡加載模型範例
[RTPS_ObjectDetection_AudioClassification.ino](https://github.com/rkuo2000/Arduino/blob/master/examples/AMB82-MINI/RTSP_ObjectDetection_AudioClassification/)<br>
[![](https://markdown-videos-api.jorgenkh.no/youtube/cVvdnXiCAa4)](https://youtu.be/cVvdnXiCAa4)

---
#### AMB82 Mini - 綫上AI 模型轉換工具
[![](https://markdown-videos-api.jorgenkh.no/youtube/6cHC2cOKgQk)](https://youtu.be/6cHC2cOKgQk)


---
## 9. 語言模型 (LLM/VLM)

### 雲端視覺辨識 

#### Examples > AmebaNN > MultimediaAI > GenAIVision

#### Examples > AmebaNN > MultimediaAI > GenAIVisionTTS

#### Examples > AmebaNN > MultimediaAI > GenAIVideo

---
### 本地PC視覺辨識
*PC-RTX5060OC-8/16GB running Ollama-server with Gemma-4 model* <br>
`ollama run gemma4:e2b`<br>

#### Examples > AmebaNN > MultimediaAI > SendImageOllama

#### Sketchbook > AMB82-mini > [Ollama_sendText](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Arduino/AMB82-mini/Ollama_sendText/Ollama_sendText.ino)

#### Sketchbook > AMB82-mini > [Ollama_sendImage](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Arduino/AMB82-mini/Ollama_sendImage/Ollama_sendImage.ino)

---
## 10. Agentic Robot

### [AMB82 MCP Server](https://lobehub.com/zh-TW/mcp/jimmychintw-amb82-mcp-server) for FreeRTOS SDK
* Dual-mode development:
  - Arduino — arduino-cli compile & upload
  - FreeRTOS SDK — cmake + arm-none-eabi-gcc compile, uartfwburn upload
* Semantic search with ONNX Runtime — pre-built index covers 3,500+ documents from:
  - `amb82-mini-docs` — official tutorials and API docs
  - `ameba-arduino-pro2` — Arduino SDK libraries and examples
  - `ameba-rtos-pro2` — FreeRTOS SDK components

---
### VLA Robot

#### RoboCar: AMB82-mini + DRV8833 + MPU6050 + VL53L0X + PAM8403 + Speaker + DC-motor x2
<p><img width="50%" height="50%" src="https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/assets/RoboCar-AMB82mini.jpg?raw=true"></p>

#### Functions:
* 防撞 - 需讀取紅外線測距 (VL53L0X)
* 朝向 - 需讀取慣性感測元件偵測角度 (MPU6050)
* 行進 - 需讀取行進命令，執行馬達控制 (DRV8833)
* 導航 - 需拍照送VLM識別  (Ollama server: [Gemma4:E2B] )
* 命令 - 提問LLM or Agent (Ollama server: [Gemma4:E2B] )

---
## Appendix. EdgeAI 應用專題實作

### 114年度專題實作
1. [AI輔助回收系統](https://li-kai-huang.github.io/blog/AI-Class/project1/)
2. [AI輔助英語讀字卡造句](https://li-kai-huang.github.io/blog/AI-Class/project4/)
3. [AI看圖說故事](https://li-kai-huang.github.io/blog/AI-Class/project3/)
4. [AI情緒感知音樂播放器](https://li-kai-huang.github.io/blog/AI-Class/project5/)
5. [AI監視錄影系統](https://li-kai-huang.github.io/blog/AI-Class/project2/)
6. [盲人輔助系統](https://li-kai-huang.github.io/blog/AI-Class/project7/)
7. [盲人導航系統](https://li-kai-huang.github.io/blog/AI-Class/project6/)

