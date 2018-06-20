#include "hardware.h"

/**************************************************************************/
/*!
    @brief  Get raw battery voltage
    @return Current battery voltage
*/
/**************************************************************************/
float batteryVoltage() {
    int adcReading;
    int voltage;
    adcReading = analogRead(V_BAT_PIN);
    // Discard inaccurate 1st reading
    adcReading = 0;
    // Perform averaging
    for (unsigned char counter = 10; counter > 0; counter--)
    {
      adcReading += analogRead(V_BAT_PIN);
    }
    adcReading = adcReading / 10;
    adcReading *= 2;    // we divided by 2 (voltage divider), so multiply back
    return adcReading * (3.3 / 1023.0); // ref voltage * adc resolution
}

/**************************************************************************/
/*!
    @brief  Gets the state the battery level.
    @return 0 - 15. 15 is full (or powered), 0 is empty.
*/
/**************************************************************************/
uint8_t batteryState(){
   float batterystate = batteryVoltage();
   if(batterystate >= 4.2){
      return 15;
   }
   return map(batterystate, 3.2, 4.2, 0, 15);
}

