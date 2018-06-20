#ifndef PROBE_SCREEN__H_
#define PROBE_SCREEN__H_


void initScreen();

void setLineInverted(int line, bool inverted);

void clearScreen();

void clearLine(int line);

void clearSpace(int x, int line, int width);

void drawSmallIcon(int offset, int line, int width, int height, const unsigned char* icon);

void drawText(int offset, int line, char* text);

void drawFullScreenIcon(const unsigned char* icon);

void drawBatteryIcon();

void drawGPSIcon();

#endif //PROBE_SCREEN__H_
