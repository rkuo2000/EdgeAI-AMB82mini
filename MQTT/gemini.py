# pip install google.generativeai
# pip install paho.mqtt

import paho.mqtt.publish as publish
import google.generativeai as genai
import PIL.Image
import os

host = "test.mosquitto.org"
GOOGLE_API_KEY= "xxxxxxxxxxxxxxxxxxxx" ## https://aistudio.google.com/app/apikey
genai.configure(api_key=GOOGLE_API_KEY)

## Setup Model
model_id = "gemini-2.0-flash"

model = genai.GenerativeModel(model_id)

## Prompting
prompt = "You are a home robot, please go to the kitchen"

## Generate content
response = model.generate_content( [prompt] )

print(response.text)

publish.signle("ntou/edgeai/robot", response.text, hostname=host)

