#include "MP3_Player.h"
#include "AmebaFatFS.h"
#include <section_config.h>

#define TOUCH_PIN A0  // Change to your actual button pin

char filename[] = "mp3/JarOfLove.mp3"; // Your MP3 file name

AmebaFatFS fs;

bool isPlaying = false;

void setup() {
    Serial.begin(115200);
    Serial.println("Waiting for button press to play MP3...");
}

void playMP3() {
    char path[128];
    bool b_result;
    b_result = fs.begin();

    if (b_result) {
        sprintf(path, "%s%s", fs.getRootPath(), filename);
        File file = fs.open(path);
        int size = file.available();
        mp3_data_len = size;
        file.seek(0);
        mp3_data = new unsigned char[mp3_data_len];
        file.read(mp3_data, mp3_data_len);
        file.close();
        fs.end();
        
        parseMP3();
        setOutputGain(0x90); // The value must be in [0x0~0xAF]
        audio_helix_mp3();
    } else {
        Serial.println("==== SD init failed ====");
    }
}

void loop() {
    int touchValue = analogRead(TOUCH_PIN);

    if (!isPlaying && touchValue > 64 ) {
        Serial.println("Button Pressed! Playing MP3...");
        isPlaying = true;        
        playMP3();
    } else if (touchValue == 0) {
        isPlaying = false;
    }

    delay(500);
}

