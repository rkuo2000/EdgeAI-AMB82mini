## 人臉辨識與識別

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

