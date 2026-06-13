# EdgeAI AMB82mini

* [AI 硬體](https://rkuo2000.github.io/AI-course/lecture/2026/06/01/AI-Hardwares.html)
* [MCU 硬體界面介紹](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/MCU_interfaces.md)

## 1. AMB82-mini 開發板介紹

### [EVB & IDE 介紹與使用](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/EVB_IDE.md)

### [BLE & WiFi 通訊與協定](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/BLE_WiFi.md)

### [感測器與週邊裝置](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Sensor_Peripherals.md)

### [影音串流](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Video.md)

---
## 2. [音頻應用](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Audio.md)

### 聲音播放 
* Examples > AmebaMultimedia > SDCardPlayMP3
* Sketchbook> AMB82-mini > [SDCardPlayMP_All](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Arduino/AMB82-mini/SDcardPlayMP3_All/SDcardPlayMP3_All.ino)

### 語音合成 (TTS)
* Examples > AmebaNN > MultimediaAI > TextToSpeech (google-tts)

---
## 3. 語音辨識 (ASR)

### 聲音偵測 (VAD)
* Examples> AmebaMultimedia > Audio > AudioTriggerRecording
* Examples> AmebaMultimedia > Audio > SoundDetectorTrigger

### 雲端語音辨識 
*ffmpeg.exe is needed for Windows to run Whisper* <br>
* Examples > AmebaNN > MultimediaAI > GenAISpeech_Whisper (Whisper)
* Examples > AmebaNN > MultimediaAI > GenAISpeech_Gemini  (Gemini)

### 本地PC語音辨識
*[AmebaPro2 Whisper LLM_server](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/AmebaPro2_server/AmebaPro2_Whisper_LLM_server.py)* <br>
* Examples -> AmebaHttp > HTTP_Post_MP4_Whisper_server
* Examples -> AmbeaHttp > HTTP_Post_MP4_Whisper_server_FastAPI

---
## 4. 神經網路模型 (AmebaNN)
### [人臉辨識與識別](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Face.md)

### [影像分類](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Image_Classification.md)

### [物件偵測](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Object_Detection.md)

---
## 5. 語言模型 (LLM/VLM)

* [大型語言模型介紹](https://rkuo2000.github.io/AI-course/lecture/2026/06/07/LLM.html)
* [視覺語言模型介紹](https://rkuo2000.github.io/AI-course/lecture/2026/06/07/VLM.html)
  
### 雲端視覺辨識
* Examples > AmebaNN > MultimediaAI > GenAIVision （Gemini-Flash2.5)
* Examples > AmebaNN > MultimediaAI > GenAIVisionTTS

### 本地PC視覺辨識
*PC-RTX5060OC-8/16GB running Ollama-server with Gemma-4 model* <br>
`ollama run gemma4:e2b`<br>

* Examples > AmebaNN > MultimediaAI > SendImageOllama
* Sketchbook > AMB82-mini > [Ollama_sendText](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Arduino/AMB82-mini/Ollama_sendText/Ollama_sendText.ino)
* Sketchbook > AMB82-mini > [Ollama_sendImage](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Arduino/AMB82-mini/Ollama_sendImage/Ollama_sendImage.ino)

---
## 6. Agent & Robot-control

### [AMB82 MCP Server](https://lobehub.com/zh-TW/mcp/jimmychintw-amb82-mcp-server) for FreeRTOS SDK
* Dual-mode development:
  - Arduino — arduino-cli compile & upload
  - FreeRTOS SDK — cmake + arm-none-eabi-gcc compile, uartfwburn upload
* Semantic search with ONNX Runtime — pre-built index covers 3,500+ documents from:
  - `amb82-mini-docs` — official tutorials and API docs
  - `ameba-arduino-pro2` — Arduino SDK libraries and examples
  - `ameba-rtos-pro2` — FreeRTOS SDK components

---
### Agentic MCU

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

---
### 115年度期末final report](https://github.com/rkuo2000/EdgeAI-MCU)
#### [EdgeAI-MCU student's report](https://new1moon2cat3-code.github.io/EdgeAI-MCU-/)
