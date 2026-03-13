# EdgeAI AMB82mini
```
教材網址：https://github.com/rkuo2000/EdgeAI-AMB82mini
```

```
LLM服務器程式範例： [AmebaPro2_server/*.py](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/AmebaPro2_server)
Arduino 程式範例： [Arduino/AMB82-mini/](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Arduino/AMB82-mini)
```

自強基金會 WiFi 
```
SSID: TCFSTWIFI.ALL
Pass: 035623116
```

## 1. AI Overview

### [AI 簡介](https://rkuo2000.github.io/AI-course/lecture/2025/09/01/AI-Brief.html)

### [AI 硬體介紹](https://rkuo2000.github.io/AI-course/lecture/2025/09/01/AI-Hardwares.html)

---
## 2. [EVB & IDE 介紹與使用](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/EVB_IDE.md)

### [Agentic Device using AMB82-mini](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/AgenticDevice.md)

### [MCU 硬體界面介紹](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/MCU_interfaces.md)

---
## 3. [BLE & WiFi 通訊與協定](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/BLE_WiFi.md)

---
## 4. [感測器與週邊裝置](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Sensor_Peripherals.md)

---
## 5. [影音串流](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Video.md)

---
## 6. [音頻應用](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Audio.md)

---
## 7. [人臉辨識與識別](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Face.md)

---
## 8. [影像分類](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Image_Classification.md)

---
## 9. [物件偵測](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Object_Detection.md)

---
## 10. AmebaNN ~ MultimediaAI
**[大型語言模型介紹](https://rkuo2000.github.io/AI-course/lecture/2024/08/16/LLM.html)** <br>
**[視覺語言模型介紹](https://rkuo2000.github.io/AI-course/lecture/2024/08/16/VLM.html)** <br>

### Examples > AmebaNN > MultimediaAI > GenAIVision

### Examples > AmebaNN > MultimediaAI > SendImageOllama

### Examples > AmebaNN > MultimediaAI > GenAISpeech_Whisper

### Examples > AmebaNN > MultimediaAI > GenAISpeech_Gemini

### Examples:** AmebaNN > MultimediaAI > TextToSpeech

---
### 語音辨識範例
#### ffmpeg.exe is needed for Windows to run Whisper!
* [AmebaPro2 Whisper server](https://github.com/rkuo2000/Arduino/blob/master/examples/AMB82-MINI/src/AmebaPro2_Whisper_server.py)
  
* [HTTP_Post_Audio.ino](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Arduino/AMB82-mini/HTTP_Post_Audio)

---
### 語音交談範例
#### ffmpeg.exe is needed for Windows to run Whisper!
Download [ffmpeg-master-latest-win64-gpl.zip](https://github.com/BtbN/FFmpeg-Builds/releases/download/latest/ffmpeg-master-latest-win64-gpl.zip), extract & put ffmpeg.exe into  where you run Whisper server.<br>

#### [AmebaPro2 Whisper LLM_server](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/AmebaPro2_server/AmebaPro2_Whisper_LLM_server.py)

#### [AmebaPro2_Whisper_Gemini_server](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/AmebaPro2_server/AmebaPro2_Whisper_Gemini_server.py) 

#### [HTTP_Post_Audio.ino](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Arduino/AMB82-mini/HTTP_Post_Audio)
* 修改server IP位址 in RecordMP4_HTTP_Post_Audio.ino server IP位址, then 燒錄到 AMB82-MINI
* reset AMB82-MINI 來啟動, 按鍵兩秒後即可錄音詢問 LLM/Gemini

---
## 11. EdgeAI 應用專題實作

### Vibe Coding
1. [AI輔助回收系統](https://li-kai-huang.github.io/blog/AI-Class/project1/)
2. [AI輔助英語讀字卡造句](https://li-kai-huang.github.io/blog/AI-Class/project4/)
3. [AI看圖說故事](https://li-kai-huang.github.io/blog/AI-Class/project3/)
4. [AI情緒感知音樂播放器](https://li-kai-huang.github.io/blog/AI-Class/project5/)
5. [AI監視錄影系統](https://li-kai-huang.github.io/blog/AI-Class/project2/)
6. [盲人輔助系統](https://li-kai-huang.github.io/blog/AI-Class/project7/)
7. [盲人導航系統](https://li-kai-huang.github.io/blog/AI-Class/project6/)

---
### Agent & Robots

#### RoboCar
<img width="50%" height="50%" src="https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/assets/RoboCar-AMB82mini.jpg?raw=true">

#### Toy-Dog
<img width="50%" height="50%" src="https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/assets/toy-dog.jpg?raw=true">

#### Vaccum Robot
<img width="50%" height="50%" src="https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/assets/vacuum_robot.png?raw=true")

#### Gripper Agent
<img width="50%" height="50%" "https://github.com/rkuo2000/EdgeAI-AMB82mini/50%" src="assets/SO-ARM101.webp?raw=true"<br>

![](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/assets/SO-ARM101.webp?raw=true)
