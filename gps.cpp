#include "gps.h"
#include <NMEAGPS.h>
#include <GPSport.h>
#include <Streamers.h>
#include "wiring_private.h" // pinPeripheral() function 
#include "hardware.h"

// Define additional hardware serial port on M0
Uart Serial2 (&sercom1, GPS_SERIAL_RX, GPS_SERIAL_TX, SERCOM_RX_PAD_0, UART_TX_PAD_2);

// Check neogps configuration

#ifndef NMEAGPS_INTERRUPT_PROCESSING
  #error You must define NMEAGPS_INTERRUPT_PROCESSING in NMEAGPS_cfg.h!
#endif

#ifndef GPS_FIX_ALTITUDE
  #error You must define GPS_FIX_ALTITUDE in GPSfix_cfg.h!
#endif

#ifndef GPS_FIX_HDOP
  #error You must define GPS_FIX_HDOP in GPSfix_cfg.h!
#endif

static NMEAGPS   gps;
static gps_fix   fix;

/**************************************************************************/
/*!
    @brief  Sercom 1 interupt handler to feed NEOGPS with characters
*/
/**************************************************************************/
void SERCOM1_Handler()
{
  Serial2.IrqHandler();
  gps.handle( Serial2.read() );
}

/**************************************************************************/
/*!
    @brief  Starts the GPS
*/
/**************************************************************************/
void initGPS(){
  gpsPort.begin( 9600 );
  pinPeripheral(GPS_SERIAL_RX, PIO_SERCOM);
  pinPeripheral(GPS_SERIAL_TX, PIO_SERCOM);
}

/**************************************************************************/
/*!
    @brief  Checks to see if GPS has a valid lock (location, alt and hdop)
    @return True if gps fix is valid, false if it is not.
*/
/**************************************************************************/
bool hasGPSLock() {
  while (gps.available()) {
    fix = gps.read();
  }
  if(fix.valid.location && fix.valid.altitude && fix.valid.hdop){  
    return true;
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
   if(!hasGPSLock()) return false;
   location->latitude = fix.latitudeL();
   location->longitude = fix.longitudeL();
   location->altitude = fix.altitude_cm();    
   location->hdop = fix.hdop;
   return true;
}
