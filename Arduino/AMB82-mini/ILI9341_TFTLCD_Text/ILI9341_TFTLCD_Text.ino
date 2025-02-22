// This sketch shows how to interface with ILI9314 TFT-LCD
// Guide: https://www.amebaiot.com/en/amebapro2-arduino-spi-lcd

#include "SPI.h"
#include "AmebaILI9341.h"
#include "AmebaLogo.h"

#define TFT_RESET       5
#define TFT_DC          4
#define TFT_CS          SPI_SS

AmebaILI9341 tft = AmebaILI9341(TFT_CS, TFT_DC, TFT_RESET);

#define ILI9341_SPI_FREQUENCY 20000000

#define FONTSIZE 2
#define TEXTCOLOR ILI9341_GREEN

#define TEXT "AMB82-mini with GenAI";

char* text;

void setup()
{
    Serial.begin(115200);
    Serial.println("init ILI9341_TFTLCD...");

    SPI.setDefaultFrequency(ILI9341_SPI_FREQUENCY);

    tft.begin();

    Serial.println("test AMB82-mini driving ILI9341-TFTLCD");
    delay(500);
}

void loop()
{
  char* text = TEXT;
    for (int i = 0; i < 4; i++) {
        init_TFTLCD(TEXTCOLOR, FONTSIZE);
        tft.setRotation(i);
        tft.println(text);
        delay(500);
    }
}

unsigned long init_TFTLCD(int textcolor, int fontsize)
{
    tft.clr();
    tft.setCursor(0, 0);
    tft.setForeground(textcolor);
    tft.setFontSize(fontsize);

    return 0;
}
