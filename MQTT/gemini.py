# pip install google.generativeai
# pip install paho.mqtt
# pip install grpcio==1.59.0

import paho.mqtt.publish as publish
import google.generativeai as genai
import PIL.Image
import os

host = "test.mosquitto.org"
GOOGLE_API_KEY= "xxxxxxxxxxxxxxxxxxxxx" ## https://aistudio.google.com/app/apikey
genai.configure(api_key=GOOGLE_API_KEY)

## Setup Model
model_id = "gemini-2.0-flash"

model = genai.GenerativeModel(model_id)

## Prompting
prompt = "You are a two-wheel robot, I want you to turn right, please give the short answer of how you set your L-motor and R-motot to turn"
print(prompt)
print()

## Generate content
response = model.generate_content( [prompt] )

print(response.text)

publish.single("ntou/edgeai/robot1", response.text, hostname=host)

