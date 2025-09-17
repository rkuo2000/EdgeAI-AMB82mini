import paho.mqtt.publish as publish

host = "test.mosquitto.org"

publish.single("NTOUEE/EdgeAI/Robot1", "go to the kitchen", hostname=host)
