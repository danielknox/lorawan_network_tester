#include "sweep.h"

#include <Arduino.h>
#include "icons.h"
#include "screen.h"
#include "settings.h"
#include "joystick.h"
#include "menu.h"

#define DELAY_BETWEEN_TESTS_MS    (4000)
#define NO_CHANNEL_DELAY_SECONDS     (8)

bool sfSuccess[6];

void initSweepResults();
void initSweepError();
void spinBackMenu();

state sweepResultsState = {
  &initSweepResults,
  &spinBackMenu,
  NULL
};

state sweepErrorState = {
  &initSweepError,
  &spinBackMenu,
  NULL
};

/**************************************************************************/
/*!
    @brief  Sets up the screen for sweep mode
*/
/**************************************************************************/
void initSweep() {
  if(!loraJoinIfNeeded()) {
    setState(&menuState);
    return;
  }
  
  setLineInverted(0, true);
  setLineInverted(1, false);
  setLineInverted(2, true);
  setLineInverted(3, false);
  setLineInverted(4, true);

  clearScreen();

  drawText(32, 1, "Press To");
  drawText(50, 2, "Sweep");
}

/**************************************************************************/
/*!
    @brief  Sends a single lorawan transmission. Updates the screen with the results.
    @param sf The spread factor to test at.
    @result The overal result of the transmission.
*/
/**************************************************************************/
transmit_responce testSF(spread_factor sf) {
  int errorAttempt = 0;
  transmit_result res;
  char buffer[20];
  drawSmallIcon(25, 0, 24, 24, transmitting);
  clearLine(4);
  while(1) {
    switch(loraTransmit(false, sf, res)) {
      case TEST_SUCCESS:
        clearLine(2);
        drawText(5, 2, sfToText(sf));
        snprintf(buffer, 20, "%ddBM", getTransmitPower());
        drawText(97, 2, buffer);
        clearSpace(25, 0, 25);
        clearSpace(50, 1, 160);
        snprintf(buffer, 20, "%3.1fMHz", res.freq);
        drawText(64, 1, buffer);
        snprintf(buffer, 20, "%ddBM", res.noise);
        drawText(5, 4, buffer);
        snprintf(buffer, 20, "GW %d", res.gateways);
        drawText(95, 4, buffer);
        return TEST_SUCCESS;
  
      case TEST_FAIL:
        clearLine(2);
        drawText(5, 2, sfToText(sf));
        snprintf(buffer, 20, "%ddBM", getTransmitPower());
        drawText(97, 2, buffer);
        clearSpace(25, 0, 25);
        drawText(5, 4, "Fail");
        return TEST_FAIL;

      case TEST_ERROR:
        clearLine(2);
        drawText(5, 2, sfToText(sf));
        snprintf(buffer, 20, "%ddBM", getTransmitPower());
        drawText(97, 2, buffer);
        clearSpace(25, 0, 25);
        for(int i=0; i<NO_CHANNEL_DELAY_SECONDS; i++) {
          if(readJoystick()==JOY_PRESSED)
            return TEST_ERROR;
          if((errorAttempt++)&1)
            drawText(8, 4, "Cancel Test?");
          else
            drawText(2, 4, "No Free Chan.");
          delay(1000);
          clearLine(4);
        }
        if(readJoystick()==JOY_PRESSED)
          return TEST_ERROR;
    }
  }
}

/**************************************************************************/
/*!
    @brief  Checks for a joystick press then begins a sweep test through all spread factors. Also updates the gps + battery icon.
*/
/**************************************************************************/
void spinSweep() {
  drawGPSIcon();
  drawBatteryIcon();
  if(readJoystick() == JOY_PRESSED) {
    transmit_responce tr;
    clearLine(1);
    drawText(5, 1, "TX");
    clearLine(2);
    clearLine(3);
    drawText(5, 3, "RX Qual.");
    if( (tr = testSF(SF_7)) == TEST_ERROR) {
      setState(&sweepErrorState);
      return;
    }
    sfSuccess[0] = tr==TEST_SUCCESS;
    drawGPSIcon();
    drawBatteryIcon();
    delay(DELAY_BETWEEN_TESTS_MS);
    if( (tr = testSF(SF_8)) == TEST_ERROR) {
      setState(&sweepErrorState);
      return;
    }
    sfSuccess[1] = tr==TEST_SUCCESS;
    drawGPSIcon();
    drawBatteryIcon();
    delay(DELAY_BETWEEN_TESTS_MS);
    if( (tr = testSF(SF_9)) == TEST_ERROR) {
      setState(&sweepErrorState);
      return;
    }
    sfSuccess[2] = tr==TEST_SUCCESS;
    drawGPSIcon();
    drawBatteryIcon();
    delay(DELAY_BETWEEN_TESTS_MS);
    if( (tr = testSF(SF_10)) == TEST_ERROR) {
      setState(&sweepErrorState);
      return;
    }
    sfSuccess[3] = tr==TEST_SUCCESS;
    drawGPSIcon();
    drawBatteryIcon();
    delay(DELAY_BETWEEN_TESTS_MS);
    if( (tr = testSF(SF_11)) == TEST_ERROR) {
      setState(&sweepErrorState);
      return;
    }
    sfSuccess[4] = tr==TEST_SUCCESS;
    drawGPSIcon();
    drawBatteryIcon();
    delay(DELAY_BETWEEN_TESTS_MS);
    if( (tr = testSF(SF_12)) == TEST_ERROR) {
      setState(&sweepErrorState);
      return;
    }
    sfSuccess[5] = tr==TEST_SUCCESS;
    drawGPSIcon();
    drawBatteryIcon();
    delay(DELAY_BETWEEN_TESTS_MS);
    setState(&sweepResultsState);
  }
}

/**************************************************************************/
/*!
    @brief  Ease-of-use function to convert array indicies to spread factor text.
    @param index the array index
    @return The corresponding text
*/
/**************************************************************************/
char* sfIndexToText(int index) {
  switch(index) {
    case 0:  return "SF 7";
    case 1:  return "SF 8";
    case 2:  return "SF 9";
    case 3: return "SF10";
    case 4: return "SF11";
    case 5: return "SF12";
    default: return "Unknown";
  }
}

/**************************************************************************/
/*!
    @brief  Sets up the screen to display the test results
*/
/**************************************************************************/
void initSweepResults() {
  setLineInverted(0, true);
  setLineInverted(1, false);
  setLineInverted(2, false);
  setLineInverted(3, false);
  setLineInverted(4, false);
  clearScreen();

  for(int i=0; i<3; i++) {
    setLineInverted(2, sfSuccess[i]);
    clearSpace(i*53, 2, 54);
    drawText(i*53+2, 2, sfIndexToText(i));
    
    setLineInverted(3, sfSuccess[i+3]);
    clearSpace(i*53, 3, 54);
    drawText(i*53+2, 3, sfIndexToText(i+3));
  }

  drawText(2, 1, "SF Avail.");
  drawText(2, 4, "<Back");
  drawText(99, 4, "Menu>");
}

/**************************************************************************/
/*!
    @brief  Sets up the screen to display an error if the sweep test errors.
*/
/**************************************************************************/
void initSweepError() {
  setLineInverted(0, true);
  setLineInverted(1, false);
  setLineInverted(2, false);
  setLineInverted(3, false);
  setLineInverted(4, false);
  clearScreen();

  drawText(50, 0, "Error");
  //TODO: use lines 1-3 to display some sort of error message. Was it a hardware fault or ran out of duty cycle, etc.
  drawText(2, 4, "<Back");
  drawText(99, 4, "Menu>");
}

/**************************************************************************/
/*!
    @brief  A simple spin that monitors for joystick input. Transitions to initial sweep state, or back to main menu.
*/
/**************************************************************************/
void spinBackMenu() {
  drawGPSIcon();
  drawBatteryIcon();
  switch(readJoystick()) {
    case JOY_LEFT:
      setState(&sweepState);
      return;
    case JOY_RIGHT:
      setState(&menuState);
      return;
  }
}

state sweepState = {
  &initSweep,
  &spinSweep,
  NULL
};
