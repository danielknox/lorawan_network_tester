#include "hardware.h"

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
