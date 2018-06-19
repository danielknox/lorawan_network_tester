#include "gps.h"

bool hasGPSLock() {
  return false;
}

bool populateGPSPayload(char* payload) {
   if(!hasGPSLock()) return false;

  //TODO : re-write following to match
//  if (locInfo) {
//    if(fix.valid.location){  
//      frameFormat.gpsLoc = 1;
//      LatitudeBinary = (fix.latitudeL() + 900000000 )/ 200;
//      LongitudeBinary = (fix.latitudeL() + 1800000000 )/ 400;
//      
//      txBuffer[4] = (LatitudeBinary >> 16) & 0xFF;
//      txBuffer[5] = (LatitudeBinary >> 8) & 0xFF;
//      txBuffer[6] = LatitudeBinary & 0xFF;
//
//      txBuffer[7] = (LongitudeBinary >> 16) & 0xFF;
//      txBuffer[8] = (LongitudeBinary >> 8) & 0xFF;
//      txBuffer[9] = LongitudeBinary & 0xFF;
//    } else {
//      frameFormat.gpsLoc = 0;
//    }
//    if(fix.valid.altitude){
//      frameFormat.gpsAlt = 1;
//      altitudeGps = fix.alt.whole;    
//      txBuffer[10] = (altitudeGps >> 8) & 0xFF;
//      txBuffer[11] = altitudeGps & 0xFF;
//    } else {
//      frameFormat.gpsAlt = 0;
//    }
//    if(fix.valid.hdop){
//      frameFormat.gpsHdop = 1;
//      txBuffer[12] = (fix.hdop / 1000);
//    } else {
//      frameFormat.gpsHdop = 0;
//    }
//  }
  return true;
}
