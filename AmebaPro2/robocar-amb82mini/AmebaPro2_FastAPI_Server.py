## To run server (PC / RPi5): python AmebaPro2_FastAPI_Server.py
## To run client (AMB82mini): RoboCar_DRV8833_IMU_IR_MP3_GTimer.ino 

# pip install git+https://github.com/openai/whisper.git
# pip install fastapi uvicorn
# pip install nest-asyncio

from PIL import Image
from fastapi import FastAPI, HTTPException
from fastapi.responses import Response
from pydantic import BaseModel
import uvicorn
import base64

import whisper
ASR = whisper.load_model("base").to("cpu")

import nest_asyncio
nest_asyncio.apply()

class Base64Text(BaseModel):
    text_base64_string: str

class Base64Audio(BaseModel):
    audio_base64_string: str

class Base64Image(BaseModel):
    image_base64_string: str

class Base64Data(BaseModel):
    audio_base64_string: str
    image_base64_string: str

class Base64TextImage(BaseModel):
    text_base64_string: str
    image_base64_string: str

Command = "STOP"
app = FastAPI()

@app.get("/")
def home():
    return Response("AmebaPro2 FastAPI Server")

@app.get("/cmd")
async def get_cmd():
    try:
        cmd = Command;
        print(cmd)
        return Response(cmd)
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@app.post("/comm")
async def post_comm(data: Base64Text):
    try:
        Command = data.text_base64_string;
        print(Command);        
        return Response("OK!")
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))
        
@app.post("/asr")
async def post_asr(data: Base64Audio):
    try:
        # Save the decoded audio data to a MP4 file
        decoded_data = base64.b64decode(data.audio_base64_string)
        with open("audio.mp4", "wb") as f:
            f.write(decoded_data)

        # Whisper transcribe
        result = ASR.transcribe("audio.mp4",fp16=False)
        header1 = "ASR:"
        print(header1+result["text"])
        return Response("OK!")
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))
        
@app.post("/imgau")
async def post_imgau(data: Base64Data):
    inst = "describe the scene in a few words"
    try:
        # Save the decoded audio data to a MP4 file
        decoded_data = base64.b64decode(data.audio_base64_string)
        with open("output.mp4", "wb") as f:
            f.write(decoded_data)

        # Save the decoded image data to a JPG file
        decoded_data = base64.b64decode(data.image_base64_string)
        with open("output.jpg", "wb") as f:
            f.write(decoded_data)

        return Response("OK!")
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@app.post("/imgtxt")
async def post_imgtxt(data: Base64TextImage):
    inst = "describe the scene in a few words"
    try:
        text = data.text_base64_string;
        print(text);

        # Save the decoded image data to a JPG file
        decoded_data = base64.b64decode(data.image_base64_string)
        with open("image.jpg", "wb") as f:
            f.write(decoded_data)

        # VLM prompt
        return Response("OK!")
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=5000, log_level="info")
