#include "AmebaFatFS.h"

#define INTERVAL 1000 // Delay between songs (adjust as needed)

char* MP3_DIR = "mp3";

AmebaFatFS fs;


// Function to play an MP3 file
void playMP3(const char *filename) {
  Serial.print("Playing: ");
  Serial.println(filename);

  File file = fs.open(String("mp3/")+String(filename));

  if (!file) {
    Serial.print("Failed to open file: ");
    Serial.println(filename);
    return;
  }

  file.close();
  Serial.println("Finished playing.");

}


void setup() {
    Serial.begin(115200);
    Serial.println("Starting...");

    fs.begin();

    char buf[512];
    char *p;

    /* list root directory and put results in buf */
    memset(buf, 0, sizeof(buf));
    fs.readDir(MP3_DIR, buf, sizeof(buf));
    printf("Files under \"%s\":\r\n", MP3_DIR);

    /* the filenames are separated with '\0', so we scan one by one */
    p = buf;
    while (strlen(p) > 0) {
        printf("%s\r\n", p);
        playMP3(p);
        p += strlen(p) + 1;
    }

    fs.end();  // Important to call end() when done using the filesystem.

    Serial.println("Done listing and playing MP3s.");

}

void loop() {
    // Put your main code here, to run repeatedly:
    delay(10000); // Wait a longer time after playing all files.  Remove this if not needed.
}