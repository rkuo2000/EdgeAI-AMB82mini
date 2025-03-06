import paho.mqtt.subscribe as subscribe

host = "mqtt.eclipseprojects.io"
msg = subscribe.simple("paho/ntou/edgeai/robot1", hostname=host)
print("topic  : %s" % (msg.topic))
print("payload: %s" % (msg.payload.decode("utf-8")))
print()

msg = subscribe.simple("paho/ntou/edgeai/robot1", hostname=host)
print("topic  : %s" % (msg.topic))
print("payload: %s" % (msg.payload.decode("utf-8")))
