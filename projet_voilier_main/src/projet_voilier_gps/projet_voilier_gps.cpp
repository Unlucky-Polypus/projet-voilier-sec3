#include <Wire.h> //Needed for I2C to GPS
#include <SparkFun_u-blox_GNSS_Arduino_Library.h> //http://librarymanager/All#SparkFun_u-blox_GNSS
#include "projet_voilier_gps.h"
#include <climits>

Gps_Voilier::Gps_Voilier()
{
  gps_reconecting_count = 0;
  gps_data.altitude = 0;
  gps_data.latitude = 0;
  gps_data.longitude = 0;
  booted = false;
  gps_data.SIV = CHAR_MIN;

  if (myGNSS.begin() == false) //Connect to the Ublox module using Wire port
  {
    Serial.println(F("Error : Setup Failed. Please check wiring"));
    gps_reconecting_count++;
  }
  else{
    booted = true;
    Serial.print(F("Version: "));
    byte versionHigh = myGNSS.getProtocolVersionHigh();
    Serial.print(versionHigh);
    Serial.print(".");
    byte versionLow = myGNSS.getProtocolVersionLow();
    Serial.println(versionLow);
    myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
    myGNSS.setNavigationFrequency(2); //Produce two solutions per second
    myGNSS.setAutoPVT(true); //Tell the GPS to "send" each solution
    myGNSS.saveConfiguration(); //Save the current settings to flash and BBR
  }
}

//Return 0 : Success
//Return -1 : Failed, invalid data with latitude=0 or longitude=0
//Return -2 : Can't pull new data
int Gps_Voilier::update_data_gps(){
  if(!booted && gps_update_status()){
    if (myGNSS.begin()){
      myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
      myGNSS.setNavigationFrequency(2); //Produce two solutions per second
      myGNSS.setAutoPVT(true); //Tell the GPS to "send" each solution
      myGNSS.saveConfiguration(); //Save the current settings to flash and BBR
    }
  }
  // Calling getPVT returns true if there actually is a fresh navigation solution available.
  // Start the reading only when valid LLH is available
  if (myGNSS.getPVT() && (myGNSS.getInvalidLlh() == false))
  {
    gps_data.latitude = myGNSS.getLatitude();
    gps_data.longitude = myGNSS.getLongitude();
    gps_data.altitude = myGNSS.getAltitude();
    //gps_data.SIV = myGNSS.getSIV();
    if(gps_data.latitude == 0 || gps_data.longitude == 0){
      return ERROR_DATA_READ_GPS_ZERO_LAT_OR_LONG;
    }
    Serial.println(gps_data.latitude);
    return GPS_DATA_GET_SUCCESS;
  } else {
    return WARNING_CANNOT_GET_NEW_PVT;
  }
}

void Gps_Voilier::gps_display_data(){
  if( gps_data.altitude == 0 && gps_data.latitude == 0 && gps_data.longitude == 0 && gps_data.SIV == CHAR_MIN){
    Serial.println("Error : No Data get for the moment");
  }
  else{
    Serial.print("Lat : ");
    Serial.print(gps_data.latitude);
    Serial.print(" Long : ");
    Serial.print(gps_data.longitude);
    Serial.print(" (degrees * 10^-7) Alt : ");
    Serial.print(gps_data.altitude);
    Serial.print(" (mm) SIV:");
    Serial.println(gps_data.SIV);
  }
}

GPS_DATA Gps_Voilier::gps_get_data(){
  return gps_data;
}

bool Gps_Voilier::gps_update_status(){
  return (myGNSS.begin() && myGNSS.isConnected());
}
