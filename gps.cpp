#include "gps.h"

//NeoGPS Configurations. Must be before NMEAGPS.h include!
#define gpsPort Serial2
#define DEBUG_PORT Serial

#include <TinyGPS++.h>
#include "wiring_private.h" // pinPeripheral() function 
#include "hardware.h"

// Define additional hardware serial port on M0
Uart Serial2 (&sercom1, GPS_SERIAL_RX, GPS_SERIAL_TX, SERCOM_RX_PAD_0, UART_TX_PAD_2);

TinyGPSPlus gps;

/**************************************************************************/
/*!
    @brief  Sercom 1 interupt handler to allow Serial2 to function correctly
*/
/**************************************************************************/
void SERCOM1_Handler()
{
  Serial2.IrqHandler();
}

/**************************************************************************/
/*!
    @brief  Starts the GPS
*/
/**************************************************************************/
void initGPS() {
  Serial2.begin( 9600 );
  pinPeripheral(GPS_SERIAL_RX, PIO_SERCOM);
  pinPeripheral(GPS_SERIAL_TX, PIO_SERCOM);
}

/**************************************************************************/
/*!
    @brief  Feed the hungry GPS object. Must be called regularly.
*/
/**************************************************************************/
void feedGPS() {
  while (Serial2.available() > 0) {
    gps.encode(Serial2.read());
  }
}

/**************************************************************************/
/*!
    @brief  Checks to see if GPS has a valid lock (location, alt and hdop)
    @return True if gps fix is valid, false if it is not.
*/
/**************************************************************************/
bool hasGPSLock() {
  // Check to see if the data-structures have useful info for use.
  if (gps.location.isValid() && gps.altitude.isValid() && gps.hdop.isValid()) {
    // Check to see if the data is 'fresh' (incase a fix has been lost)
    if ((gps.location.age() < 1500) && (gps.altitude.age() < 1500) && (gps.hdop.age() < 1500)){
      return true;
    }
  }
  return false;
}

/**************************************************************************/
/*!
    @brief  If GPS has a valid lock it will populate the passed structure with location data.
    @param  Reference of a location struct to be populated
    @return True if gps data was available, otherwise false.
*/
/**************************************************************************/
bool retieveGPS(loc* location) {
  if (!hasGPSLock()) return false;
  location->latitude = (int32_t) (gps.location.lat() * 10000000);
  location->longitude = (int32_t) (gps.location.lng() * 10000000);
  location->altitude = gps.altitude.value();
  location->hdop = gps.hdop.value() * 100;
  return true;
}
