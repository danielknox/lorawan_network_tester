#include "survey.h"

#include <Arduino.h>
#include "icons.h"
#include "screen.h"
#include "settings.h"

void initSurvey() {
  loraJoinIfNeeded();
  
  setLineInverted(0, true);
  setLineInverted(1, false);
  setLineInverted(2, true);
  setLineInverted(3, false);
  setLineInverted(4, true);

  clearScreen();
  
  clearLine(1);
  drawText(5, 1, "TX");
  clearLine(2);
  clearLine(3);
  drawText(5, 3, "RX Qual.");

  drawText(32, 1, "Press To");
  drawText(50, 2, "Sweep");
}

void spinSurvey() {
  drawGPSIcon();
}

state surveyState = {
  &initSurvey,
  &spinSurvey,
  NULL
};
