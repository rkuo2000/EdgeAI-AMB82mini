#include "MP3_Player.h"
#include "HUB8735FatFS.h"
#include <section_config.h>

#define BUTTON_PIN 1  // Change to your actual button pin
#define INTERVAL 1000

char filename[] = "mp3/JarOfLove.mp3"; // Your MP3 file name

HUB8735FatFS fs;

int buttonState = LOW;
int lastButtonState = LOW;
bool isPlaying = false;

void setup() {
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT);
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
        setOutputGain(0x80); // The value must be in [0x0~0xAF]
        audio_helix_mp3();
    } else {
        Serial.println("==== SD init failed ====");
    }
}

void loop() {
    buttonState = digitalRead(BUTTON_PIN);

    if (buttonState == HIGH && lastButtonState == LOW) {
        Serial.println("Button Pressed! Playing MP3...");
        if (!isPlaying) {
            playMP3();
            isPlaying = true;
        }
    } else if (buttonState == LOW) {
        isPlaying = false;
    }

    lastButtonState = buttonState;
    delay(INTERVAL);
}

