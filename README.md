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

## AI Overview

### [AI 簡介](https://rkuo2000.github.io/AI-course/lecture/2025/09/01/AI-Brief.html)

### [AI 硬體介紹](https://rkuo2000.github.io/AI-course/lecture/2025/09/01/AI-Hardwares.html)

---
## [EVB & IDE 介紹與使用](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/EVB_IDE.md)

### [MCU 硬體界面介紹](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/MCU_interfaces.md)

---
## [ BLE & WiFi 通訊與協定](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/BLE_WiFi.md)

---
## [感測器與週邊裝置](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Sensor_Peripherals.md)

---
## [影音串流](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Video.md)

---
## [音頻應用](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Audio.md)

---
## 8. 人臉辨識與識別

### [人臉辨識與識別介紹](https://rkuo2000.github.io/AI-course/lecture/2024/08/07/Face-Recognition.html)

---
### [人臉檢測範例](https://www.amebaiot.com/en/amebapro2-arduino-neuralnework-face-detection/)
**Examples> AmebaNN > RTSPFaceDetection** <br>

[![](https://markdown-videos-api.jorgenkh.no/youtube/KD95JH6gVew)](https://youtu.be/KD95JH6gVew)

<p><img width="50%" height="50%" src="https://www.amebaiot.com/wp-content/uploads/2023/01/neuralnetwork/facedetectP06.png"></p>

---
### [人臉識別範例](https://www.amebaiot.com/en/amebapro2-arduino-neuralnework-face-recognition/)
**Examples> AmebaNN > RTSPFaceRecognition** <br>

[![](https://markdown-videos-api.jorgenkh.no/youtube/GGOIQmMfeF8)](https://youtu.be/GGOIQmMfeF8)

Serial_monitor: `REG=RKUO`<br>
<p><img width="50%" height="50%" src="https://github.com/rkuo2000/EdgeAI-AMB82-mini/blob/main/assets/FaceRecognition_REG_RKUO.jpeg?raw=true"></p>

* Enter the command **REG=Name** to give the targeted face a name.
* Enter the command **DEL=Name** to delete a certain registered face. For example, `DEL=SAM`
* Enter the command **BACKUP** to save a copy of registered faces to flash.
* If a backup exists, enter the command **RESTORE** to load registered faces from flash.
* Enter the command **RESET** to forget all previously registered faces. 

---
## 9. 影像分類 (Image Classification)
* **[Kaggle平台使用介紹](https://rkuo2000.github.io/AI-course/lecture/2024/08/02/Kaggle-Intro.html)**

* **[卷積層神經網路介紹](https://rkuo2000.github.io/AI-course/lecture/2024/08/03/CNN.html)**

* **[影像分類介紹](https://rkuo2000.github.io/AI-course/lecture/2024/08/04/Image-Classification.html)**

---
### 影像分類範例
[RTSP_GarbageClassification.ino](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Arduino/AMB82-mini/RTSP_GarbageClassification)<br>
[![](https://markdown-videos-api.jorgenkh.no/youtube/c3XGkc9ShwQ)](https://youtu.be/c3XGkc9ShwQ)

---
### Garbage模型訓練與檔案轉換

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
## 10. 物件偵測 (Object Detection)

### Public Dataset
**[Roboflow](https://universe.roboflow.com/)** <br>

---
### [物件檢測介紹](https://rkuo2000.github.io/AI-course/lecture/2024/08/05/Object-Detection.html)
**Kaggle範例:** <br>
* [YOLOv7 Facemask detection](https://www.kaggle.com/code/rkuo2000/yolov7-facemask-detection)
* [YOLOv7 Pothole detection](https://www.kaggle.com/code/rkuo2000/yolov7-pothole)
* [YOLOv7 Sushi detection](https://www.kaggle.com/code/rkuo2000/yolov7-sushi-detection)
* [YOLOv7 refrigeratoryfood](https://www.kaggle.com/code/rkuo2000/yolov7-refrigeratoryfood)<br>
* [YOLOv7 reparm](https://www.kaggle.com/code/rkuo2000/yolov7-reparam)<br>

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
### AMB82 Mini - 物件偵測範例
[![](https://markdown-videos-api.jorgenkh.no/youtube/EvryVoQyqqk)](https://youtu.be/EvryVoQyqqk)

[RTSP_YOLOv7_Pothole](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Arduino/AMB82-mini/RTSP_YOLOv7_Pothole)<br>
[RTSP_YOLOv7_Sushi](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Arduino/AMB82-mini/RTSP_YOLOv7_Sushi)<br>

---
### AMB82 Mini - SD卡加載模型範例
[RTPS_ObjectDetection_AudioClassification.ino](https://github.com/rkuo2000/Arduino/blob/master/examples/AMB82-MINI/RTSP_ObjectDetection_AudioClassification/)<br>
[![](https://markdown-videos-api.jorgenkh.no/youtube/cVvdnXiCAa4)](https://youtu.be/cVvdnXiCAa4)

---
### AMB82 Mini - 綫上AI 模型轉換工具
[![](https://markdown-videos-api.jorgenkh.no/youtube/6cHC2cOKgQk)](https://youtu.be/6cHC2cOKgQk)

---
## 11. 大型語言模型範例 (LLM)

* **[大型語言模型介紹](https://rkuo2000.github.io/AI-course/lecture/2024/08/15/LLM.html)**

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
* 到 https://aistudio.google.com/app/apikey 取得API_Key 填入GOOGLE_API_KEY, 再執行AmebaPro2_Whisper_Gemini_server.py

#### [HTTP_Post_Audio.ino](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Arduino/AMB82-mini/HTTP_Post_Audio)
* 修改server IP位址 in RecordMP4_HTTP_Post_Audio.ino server IP位址, then 燒錄到 AMB82-MINI
* reset AMB82-MINI 來啟動, 按鍵兩秒後即可錄音詢問 LLM/Gemini
  
---
## 12. 視覺語言模型 (VLM)

* **[視覺語言模型介紹](https://rkuo2000.github.io/AI-course/lecture/2024/08/16/VLM.html)** 

---
### 影像+語音交談範例
* [AmebaPro2_whisper_llava_server.py](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/AmebaPro2_server/AmebaPro2_Whisper_Llava_server.py)
  
* [HTTP_Post_ImageAudio](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Arduino/AMB82-mini/HTTP_Post_ImageAudio)

---
## 13. GenAI

**Examples:** AmebaNN > MultimediaAI > GenAIVision<br>
**Examples:** AmebaNN > MultimediaAI > GenAISpeech_Gemini<br>
**Examples:** AmebaNN > MultimediaAI > TextToSpeech<br>

* [GenAIVision_TTS_Text_ReadWordCard](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Arduino/AMB82-mini/GenAIVision_TTS_Text_ReadWordCard/GenAIVision_TTS_Text_ReadWordCard.ino)

* [GenAISpeech.ino](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Arduino/AMB82-mini/GenAISpeech/GenAISpeech.ino)

* [TextToSpeech.ino](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Arduino/AMB82-mini/TextToSpeech/TextToSpeech.ino)
  
---
## 14. EdgeAI 應用專題實作

### [邊緣計算微控制器應用專題實作期末報告](https://itzorange.my.canva.site/edgiai01151022projectpresent/#%E7%B0%A1%E4%BB%8B) (Canva)

### [EdgeAI 專題快速導覽](https://li-kai-huang.github.io/blog/AI-Class/)

* [AI輔助回收系統](https://li-kai-huang.github.io/blog/AI-Class/project1/)
  
* [AI輔助英語讀字卡造句](https://li-kai-huang.github.io/blog/AI-Class/project4/)
  
* [AI看圖說故事](https://li-kai-huang.github.io/blog/AI-Class/project3/)
  
* [AI情緒感知音樂播放器](https://li-kai-huang.github.io/blog/AI-Class/project5/)
  
* [AI監視錄影系統](https://li-kai-huang.github.io/blog/AI-Class/project2/)
  
* [盲人輔助系統](https://li-kai-huang.github.io/blog/AI-Class/project7/)
  
* [盲人導航系統](https://li-kai-huang.github.io/blog/AI-Class/project6/)
