#include "screen.h"

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
#include "hardware.h"
#include "icons.h"
#include "gps.h"

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

bool lineInverted[5] = {false, false, false, false, false};

void initScreen() {
  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  tft.setRotation(3);
  tft.setTextSize(2);
  tft.fillScreen(ST7735_WHITE);
  clearScreen();
}

void setLineInverted(int line, bool inverted) {
  lineInverted[line] = inverted;
}

int getBackground(int line) {
  return lineInverted[line] ? 0xFFFFFF : 0x000000;
}

int getForeground(int line) {
  return lineInverted[line] ? 0x000000 : 0xFFFFFF;
}

void clearScreen() {
  //tft.fillScreen(ST7735_WHITE);
  for(int i=0; i<5; i++) {
    clearLine(i);
  }
}

void clearLine(int line) {
  clearSpace(0,line,160);
}

void clearSpace(int x, int line, int width) {
  tft.fillRect(x, line*25, width, 25, getBackground(line));
}

int toGrid(int x) {
  int y = (x+24)/25;
  return y*25;
}

void drawSmallIcon(int offset, int line, int width, int height, const unsigned char* icon) {
  tft.fillRect(offset, line*25, toGrid(width), 25, getBackground(line));
  tft.drawBitmap(offset, (line*25), icon, width, height, getForeground(line));
}

void drawText(int offset, int line, char* text) {
  tft.setCursor(offset, line*25+5);
  tft.setTextColor(getForeground(line));
  tft.print(text);
}

void drawFullScreenIcon(const unsigned char* icon) {
  tft.fillScreen(ST7735_WHITE);
  tft.drawBitmap(15, 5, icon, 128, 128, 0x000000);
}

void drawBatteryIcon() {
  drawSmallIcon(130, 0, 24, 24, gpsConnected);
  //TODO: add pe
}

void drawGPSIcon() {
  static bool wasLock = false;
  static bool wasNoLock = false;
  if(hasGPSLock()) {
    if(!wasLock) {
      drawSmallIcon(0, 0, 24, 24, gpsConnected);
      wasNoLock = false;
      wasLock = true;
    }
  } else if(!wasNoLock) {
    drawSmallIcon(0, 0, 24, 24, gpsDisconnected);
    wasNoLock = true;
    wasLock = false;
  }
}
