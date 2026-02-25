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
## 10. 語音模型

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
## 11. 語言模型

### 大型語言模型範例 (LLM)

## 視覺語言模型 (VLM)
**[視覺語言模型介紹](https://rkuo2000.github.io/AI-course/lecture/2024/08/16/VLM.html)** <br>

---
### 影像+語音交談範例

* [AmebaPro2_whisper_llava_server.py](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/AmebaPro2_server/AmebaPro2_Whisper_Llava_server.py)
  
* [HTTP_Post_ImageAudio](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Arduino/AMB82-mini/HTTP_Post_ImageAudio)

---

### AmebaNN 範例

#### Examples > AmebaNN > MultimediaAI > GenAIVision

#### Examples > AmebaNN > MultimediaAI > SendImageOllama

#### Examples > AmebaNN > MultimediaAI > GenAISpeech_Whisper

#### Examples > AmebaNN > MultimediaAI > GenAISpeech_Gemini

#### Examples:** AmebaNN > MultimediaAI > TextToSpeech
  
---
## 12. EdgeAI 應用專題實作

### [邊緣計算微控制器應用專題實作期末報告](https://itzorange.my.canva.site/edgiai01151022projectpresent/#%E7%B0%A1%E4%BB%8B) (Canva)

### [EdgeAI 專題快速導覽](https://li-kai-huang.github.io/blog/AI-Class/)

* [AI輔助回收系統](https://li-kai-huang.github.io/blog/AI-Class/project1/)
  
* [AI輔助英語讀字卡造句](https://li-kai-huang.github.io/blog/AI-Class/project4/)
  
* [AI看圖說故事](https://li-kai-huang.github.io/blog/AI-Class/project3/)
  
* [AI情緒感知音樂播放器](https://li-kai-huang.github.io/blog/AI-Class/project5/)
  
* [AI監視錄影系統](https://li-kai-huang.github.io/blog/AI-Class/project2/)
  
* [盲人輔助系統](https://li-kai-huang.github.io/blog/AI-Class/project7/)
  
* [盲人導航系統](https://li-kai-huang.github.io/blog/AI-Class/project6/)
