from paho.mqtt.enums import MQTTProtocolVersion
import paho.mqtt.publish as publish

host = "mqtt.eclipseprojects.io"

# single
publish.single("ntou/edgeai/robot1", "come to the living room", hostname=host)

publish.single("ntou/edgeai/robot2", "go to the kitchen", hostname=host)

## multiple
msgs = [{'topic':"ntou/edgeai/robot1", 'payload':"At Smarter"}, 
    ("ntou/edgeai/robot2", "RoboDog", 0, False)]
publish.multiple(msgs, hostname=host, protocol=MQTTProtocolVersion.MQTTv5)
