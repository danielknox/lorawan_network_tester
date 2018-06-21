#include "screen.h"

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
#include "hardware.h"
#include "icons.h"
#include "gps.h"
#include "battery.h"

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

bool lineInverted[5] = {false, false, false, false, false};

//gps icon state
bool wasLock = false;
bool wasNoLock = false;

int batteryLevel = -1;

/**************************************************************************/
/*!
   @brief  Initial seup of screen hardware
*/
/**************************************************************************/
void initScreen() {
  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  tft.setRotation(3);
  tft.setTextSize(2);
  tft.fillScreen(ST7735_WHITE);
  clearScreen();
}

/**************************************************************************/
/*!
   @brief  Marks whether or not the requested line has inverted colours. Changes only effect subsequent calls to draw or clear functions.
   @param line The line to update
   @param inverted Whether or not the line is inverted
*/
/**************************************************************************/
void setLineInverted(int line, bool inverted) {
  lineInverted[line] = inverted;
}

/**************************************************************************/
/*!
   @brief  Returns the background colour for a given line
   @param line The line to get the background colour for
   @return White for inverted line, black otherwise
*/
/**************************************************************************/
int getBackground(int line) {
  return lineInverted[line] ? 0xFFFFFF : 0x000000;
}

/**************************************************************************/
/*!
   @brief  Returns the foreground colour for a given line
   @param line The line to get the foreground colour for
   @return Black for inverted line, white otherwise
*/
/**************************************************************************/
int getForeground(int line) {
  return lineInverted[line] ? 0x000000 : 0xFFFFFF;
}


/**************************************************************************/
/*!
   @brief  Clears the screen. Uses the background colour for each line.
*/
/**************************************************************************/
void clearScreen() {
  //tft.fillScreen(ST7735_WHITE);
  for(int i=0; i<5; i++) {
    clearLine(i);
  }
  wasLock = false;
  wasNoLock = false;
  batteryLevel = -1;
}

/**************************************************************************/
/*!
   @brief  Clears a single line by filling the space with the background colour.
   @param line The line to clear
*/
/**************************************************************************/
void clearLine(int line) {
  clearSpace(0,line,160);
}

/**************************************************************************/
/*!
   @brief  Clears a retangular space, vertically bounded by a single line, by filling the space with the background colour.
   @param x The left most edge of the rectangle
   @param line The line to bound the top and bottom edges of the clearing rectangle
   @param width The width of the clearing rectangle
*/
/**************************************************************************/
void clearSpace(int x, int line, int width) {
  tft.fillRect(x, line*25, width, 25, getBackground(line));
}

/**************************************************************************/
/*!
   @brief  Draws a small icon to the screen. The icon is assumed to fit within a 25x25 space. If it is larger, then overlap will occur.
   @param offset The horizontal distance from the left edge of the screen
   @param line The vertical line number to place the icon
   @param width The width of the provided icon
   @param height The height of the provided icon
   @param icon The graphical data for the icon
*/
/**************************************************************************/
void drawSmallIcon(int offset, int line, int width, int height, const unsigned char* icon) {
  tft.fillRect(offset, line*25, 25, 25, getBackground(line));
  tft.drawBitmap(offset, (line*25), icon, width, height, getForeground(line));
}

/**************************************************************************/
/*!
   @brief Draws text to the screen
   @param offset The horizontal distance from the left edge of the screen
   @param line The vertical line number to place the icon
   @param text The string to display (null terminated)
*/
/**************************************************************************/
void drawText(int offset, int line, char* text) {
  tft.setCursor(offset, line*25+5);
  tft.setTextColor(getForeground(line));
  tft.print(text);
}

/**************************************************************************/
/*!
   @brief Draws a 128x128 icon, centered on the screen. Excess screen space is set to white.
   @param icon The graphical data for the icon
*/
/**************************************************************************/
void drawFullScreenIcon(const unsigned char* icon) {
  tft.fillScreen(ST7735_WHITE);
  tft.drawBitmap(15, 5, icon, 128, 128, 0x000000);
}

/**************************************************************************/
/*!
   @brief Specialist code for drawing battery icon to the top right corner. Repects normal/inverted colours. Can be called frequently
*/
/**************************************************************************/
void drawBatteryIcon() {
  int calcBattLevel = batteryState();
  if(batteryLevel==calcBattLevel) return;
  
  drawSmallIcon(130, 0, 24, 24, battery);
  tft.fillRect(133, 7, calcBattLevel, 10, getForeground(0));
  batteryLevel = calcBattLevel;
}

/**************************************************************************/
/*!
   @brief Specialist code for drawing gps icon to the top right corner. Repects normal/inverted colours. Can be called frequently
*/
/**************************************************************************/
void drawGPSIcon() {
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
