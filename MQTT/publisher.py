from paho.mqtt.enums import MQTTProtocolVersion
import paho.mqtt.publish as publish

host = "mqtt.eclipseprojects.io"

# single
publish.single("paho/ntou/edgeai/robot1", "come to the living room", hostname=host)

publish.single("paho/ntou/edgeai/robot2", "go to the kitchen", hostname=host)

## multiple
msgs = [{'topic':"paho/ntou/edgeai/robot1", 'payload':"At Smarter"}, 
    ("paho/ntou/edgeai/robot2", "RoboDog", 0, False)]
publish.multiple(msgs, hostname=host, protocol=MQTTProtocolVersion.MQTTv5)
