## 通訊硬體界面 

### BLE (低功耗藍牙）

**Exmples> AmebaBLE > BLEV7RC_CAR_VIDEO** <br>

[BLE V7RC](https://www.amebaiot.com/zh/amebad-arduino-ble-v7rc/)<br>

* [V7RC APP 介紹](https://hackmd.io/@accomdemy/v7rc)<br>
* 安裝[手機App V7RC](https://play.google.com/store/apps/details?id=com.v7idea.v7rcliteandroidsdkversion&hl=en)<br>
<p>
<img width="50%" height="50%" src="https://www.amebaiot.com/wp-content/uploads/2023/08/ble-v7rc/p1_zh.png">
<img width="50%" height="50%" src="https://www.amebaiot.com/wp-content/uploads/2023/08/ble-v7rc/p2_zh.png">
</p>

---
### WiFi（無線網路）
**Examples> WiFi > SimpleTCPServer**<br>
[WiFi - Simple TCP Server](https://www.amebaiot.com/en/amebapro2-arduino-server-client/)<br>

#### HTTP Server
**Examples> WiFi > SimpleHttpWeb > ReceiveData**<br>
[WiFi - Simple Http Server to Receive Data](https://www.amebaiot.com/en/amebapro2-arduino-web-server-status/)<br>

**Examples> WiFi > SimpleHttpWeb > ControlLED**<br>
[WiFi - Simple Http Server to Control LED](https://www.amebaiot.com/en/amebapro2-arduino-ameba-web-server/)<br>

**Sketchbook> AMB82-mini > WebServer_ControlLED**<br>
[Sketchbook> WebServer_ControlLED](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/Arduino/AMB82-mini/WebServer_ControlLED/WebServer_ControlLED.ino)<br>
![](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/assets/AMB82-mini_Arduino_Sketch_WebServer_ControlLED.png?raw=true)

---
### [MQTT通訊協定](https://mqtt.org/)
MQTT is an OASIS standard messaging protocol for the Internet of Things (IoT)<br>
[How MQTT Works -Beginners Guide](http://www.steves-internet-guide.com/mqtt-works/)<br>

**Examples> AmebaMQTTClient > MQTT_basic** <br>
[MQTT - Set up MQTT Client to Communicate with Broker](https://www.amebaiot.com/en/amebapro2-arduino-mqtt-upload-listen/)<br>
![](https://www.amebaiot.com/wp-content/uploads/2023/06/network/mqtt1-1.png)

`pip install paho.mqtt`<br>

**publish messages to AMB82-mini** <br>
```
import paho.mqtt.publish as publish
host = "test.mosquitto.org"
publish.single("ntou/edgeai/robot1", "go to the kitchen", hostname=host)
```

**subsribe messages from AMB82-mini** <br>
```
import paho.mqtt.subscribe as subscribe
host = "test.mosquitto.org"https://github.com/rkuo2000/EdgeAI-AMB82mini
msg = subscribe.simple("ntou/edgeai/robot1", hostname=host)
print("%s %s" % (msg.topic, msg.payload.decode("utf-8")))
```

---
### Gemini MQTT App
**[Google Gemini](https://gemini.google.com/) +Canvas** <br>

**Prompt:** `make an 
---
### HTTP Serverhtml to input MQTT topic and text to publish through Paho-MQTT test.mosquitto.org`<br>

**[Paho-MQTT Publisher](https://g.co/gemini/share/ae62311518bd)** <br>
![](https://github.com/rkuo2000/EdgeAI-AMB82mini/blob/main/assets/Gemini_Canvas_Paho-MQTT_Text_Publisher.png?raw=true)
