#include "Arduino.h"

// Joystick
#define JOYSTICK_FIRE_PIN  5
#define JOYSTICK_X_PIN A0
#define JOYSTICK_Y_PIN A1

// LoRaWAN (RN2483)
#define FREQ_PLAN TTN_FP_EU868
#define RN2483_RESET_PIN 12
#define RN2483_SERIAL Serial1
#define DEBUG_SERIAL Serial

// TFT Screen (ST7735)
#define TFT_CS     A5
#define TFT_RST    A4 
#define TFT_DC     A3

// GPS Serial (NEMA device) 
#define GPS_SERIAL_RX 11  // must define a pin that can be mapped to sercom1 and capable of RX
#define GPS_SERIAL_TX 10  // must define a pin that can be mapped to sercom1

// Battery 
#define V_BAT_PIN A7  // Voltage divider required if not using feather.
