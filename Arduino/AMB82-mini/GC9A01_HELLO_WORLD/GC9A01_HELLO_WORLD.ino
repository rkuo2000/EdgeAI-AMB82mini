/*!
	@file   HELLO_WORLD.ino
	@author Gavin Lyons
	@brief  Example file for GC9A01_LTSM arduino library. Test Hello World.
	@note   See USER OPTIONS 1-2 in SETUP function
	@test
		-# Test 101 Print out Hello world  
*/

// libraries
#include "GC9A01_LTSM.hpp"
// Fonts needed
#include "fonts_LTSM/FontArialBold_LTSM.hpp"
///@cond
#include "SPI.h"

//  Test timing related defines
#define TEST_DELAY1 1000  // mS
#define TEST_DELAY2 2000  // mS
#define TEST_DELAY5 5000  // mS
#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif

GC9A01_LTSM myTFT;
bool bhardwareSPI = true;  // true for hardware spi, false for software

void setup(void) {
  Serial.begin(115200);
  delay(1000);
  // === USER OPTION 1 SPI_SPEED + TYPE ===
  int8_t DC_TFT  = 4;
  int8_t RST_TFT = 5;
  int8_t CS_TFT  = 12; // SPI_SS
  if (bhardwareSPI == true) {          // hw spi
    uint32_t TFT_SCLK_FREQ = 8000000;  // Spi freq in Hertz
    myTFT.TFTsetupGPIO_SPI(TFT_SCLK_FREQ, RST_TFT, DC_TFT, CS_TFT);
  } else {                        // sw spi
    uint16_t SWSPICommDelay = 0;  // optional SW SPI GPIO delay in uS
    int8_t SDIN_TFT = 13; // SPI_MOSI
    int8_t SCLK_TFT = 15; // SPI_SCLK
    myTFT.TFTsetupGPIO_SPI(SWSPICommDelay, RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
  }
  // ===
  // === USER OPTION 2 Screen Setup ===
  uint16_t TFT_WIDTH = 240;   // Screen width in pixels
  uint16_t TFT_HEIGHT = 240;  // Screen height in pixels
  myTFT.TFTInitScreenSize(TFT_WIDTH, TFT_HEIGHT);
  // ===
  myTFT.TFTGC9A01Initialize();
  Serial.println("Start");
}

// main
void loop(void) {
  Test100();
  EndTests();
}

void Test100(void) {
  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
  myTFT.setCursor(15, 80);
  myTFT.setFont(FontArialBold);
  myTFT.print("Hello World");
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
  delay(TEST_DELAY1);
}

void EndTests(void) {
  myTFT.TFTPowerDown();
  Serial.println("End");
  while (1) {};
}
///@endcond
