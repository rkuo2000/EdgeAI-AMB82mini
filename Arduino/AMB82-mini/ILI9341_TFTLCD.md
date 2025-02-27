// This sketch shows how to interface with ILI9314 TFT-LCD
// Guide: https://www.amebaiot.com/en/amebapro2-arduino-spi-lcd

#include "SPI.h"
#include "AmebaILI9341.h"
#include "AmebaLogo.h"

#define TFT_RESET       27
#define TFT_DC          28
#define TFT_CS          SPI1_SS

AmebaILI9341 tft = AmebaILI9341(TFT_CS, TFT_DC, TFT_RESET);

#define ILI9341_SPI_FREQUENCY 20000000

void setup()
{
    Serial.begin(115200);
    Serial.println("ILI9341 Test!");

    SPI.setDefaultFrequency(ILI9341_SPI_FREQUENCY);

    tft.begin();

    Serial.println("test filling screen");
    testFillScreen();
    delay(500);

    Serial.println("test Lines");
    testLines(ILI9341_CYAN);
    delay(500);

    Serial.println("test Circles");
    testCircles(5, ILI9341_BLUE);
    delay(500);

    Serial.println("test Rectangle");
    testRectangle(ILI9341_LIGHTGREY);
    delay(500);

    Serial.println("test Bitmap");
    testBitmap(0, 0, logoWidth, logoHeight, logo);
    delay(500);

    Serial.println("test Text");
    testText();
    delay(500);
}

void loop()
{
    for (int i = 0; i < 4; i++) {
        tft.setRotation(i);
        testText();
        delay(500);
    }
}

unsigned long testFillScreen()
{
    tft.fillScreen(ILI9341_BLACK);
    tft.fillScreen(ILI9341_RED);
    tft.fillScreen(ILI9341_GREEN);
    tft.fillScreen(ILI9341_BLUE);
    tft.fillScreen(ILI9341_BLACK);

    return 0;
}

unsigned long testText()
{
    tft.clr();

    tft.setCursor(0, 0);

    tft.setForeground(ILI9341_GREEN);
    tft.setFontSize(5);
    tft.println("Ameba");

    tft.setForeground(ILI9341_WHITE);
    tft.setFontSize(3);
    tft.println("Hello World!");

    tft.setForeground(ILI9341_YELLOW);
    tft.setFontSize(2);
    tft.println(1234.56);

    tft.setForeground(ILI9341_RED);
    tft.setFontSize(1);
    tft.println(0xDEADBEEF, HEX);

    tft.println();

    tft.setForeground(ILI9341_LIGHTGREY);
    tft.setFontSize(2);
    tft.println("Alice in Wonderland");

    tft.setFontSize(1);
    tft.println("\"Where should I go?\" -Alice. \"That depends on where you want to end up.\" - The Cheshire Cat.");

    return 0;
}

unsigned long testLines(uint16_t color)
{
    int x0, y0, x1, y1;
    int w = tft.getWidth();
    int h = tft.getHeight();

    tft.clr();

    x0 = 0;
    x1 = w;
    for (y0 = y1 = 0; y0 <= h; y0 += 5, y1 += 5) {
        tft.drawLine(x0, y0, x1, y1, color);
    }
    y0 = 0;
    y1 = h;
    for (x0 = x1 = 0; x0 <= w; x0 += 5, x1 += 5) {
        tft.drawLine(x0, y0, x1, y1, color);
    }

    tft.fillScreen(ILI9341_BLACK);

    x0 = w / 2;
    y0 = h / 2;
    x1 = 0;
    for (y1 = 0; y1 < h; y1 += 5) {
        tft.drawLine(x0, y0, x1, y1, color);
    }

    y1 = h;
    for (x1 = 0; x1 < w; x1 += 5) {
        tft.drawLine(x0, y0, x1, y1, color);
    }

    x1 = w;
    for (y1 = h; y1 >= 0; y1 -= 5) {
        tft.drawLine(x0, y0, x1, y1, color);
    }

    y1 = 0;
    for (x1 = w; x1 >= 0; x1 -= 5) {
        tft.drawLine(x0, y0, x1, y1, color);
    }

    return 0;
}

void testRectangle(uint16_t color)
{
    int rw, rh;
    int div = 60;
    int w = tft.getWidth();
    int h = tft.getHeight();

    tft.clr();

    for (rw = w / div, rh = h / div; rw < w; rw += w / div, rh += h / div) {
        tft.drawRectangle((w / 2 - rw), (h / 2 - rh), (rw * 2), (rh * 2), color);
    }
}

void testCircles(uint8_t radius, uint16_t color)
{
    int x, y;
    int w = tft.getWidth();
    int h = tft.getHeight();

    tft.clr();

    for (x = radius; x < w; x += radius * 2) {
        for (y = radius; y < h; y += radius * 2) {
            tft.drawCircle(x, y, radius, color);
        }
    }
}

void testBitmap(int16_t x, int16_t y, int16_t w, int16_t h, const unsigned short *color)
{
    tft.clr();
    tft.drawBitmap(x, y, w, h, color);
    delay(500);
}
