import paho.mqtt.publish as publish

host = "test.mosquitto.org"

publish.single("ntou/edgeai/robot1", "go to the kitchen", hostname=host)
