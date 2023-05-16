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
  gps_data.SIV = CHAR_MIN;

  if (myGNSS.begin() == false) //Connect to the Ublox module using Wire port
  {
    Serial.println(F("Error : Setup Failed. Please check wiring"));
    gps_status = GPS_TRYING_CONNECTING_WIRE;
    gps_reconecting_count++;
  }
  else{
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
    gps_status = GPS_CONNECTED_NETWORK;
  }
}

//Return 0 : Success
//Return -1 : Failed, invalid data with latitude=0 or longitude=0
//Return -2 : Can't pull new data
int Gps_Voilier::update_data_gps(){
  // Calling getPVT returns true if there actually is a fresh navigation solution available.
  // Start the reading only when valid LLH is available
  if (myGNSS.getPVT() && (myGNSS.getInvalidLlh() == false))
  {
    gps_data.latitude = myGNSS.getLatitude();
    gps_data.longitude = myGNSS.getLongitude();
    gps_data.altitude = myGNSS.getAltitude();
    //gps_data.SIV = myGNSS.getSIV();
    if(gps_data.latitude == 0 || gps_data.longitude == 0){
      Serial.println("Error : Can't get latitude or longitude with the GPS");
      return ERROR_DATA_READ_GPS_ZERO_LAT_OR_LONG;
    }
    return GPS_DATA_GET_SUCCESS;
  } else {
    Serial.println("Error : Can't get new GPS PVT");
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

void Gps_Voilier::gps_display_status(){
  switch (gps_status)
  {
  case GPS_CONNECTED_NETWORK:
    Serial.println("GPS New Status: GPS_CONNECTED_NETWORK");
    break;
  case GPS_DISCONNECTED_NETWORK:
    Serial.println("GPS New Status: GPS_DISCONNECTED_NETWORK");
    break;
  case GPS_TRYING_RECONNECTING_WIRE:
    Serial.println("GPS New Status: GPS_TRYING_RECONNECTING_WIRE");
    break;
  case GPS_TRYING_CONNECTING_WIRE:
    Serial.println("GPS New Status: GPS_TRYING_CONNECTING_WIRE");
    break;
  case GPS_DISCONNECTED_WIRE:
    Serial.println("GPS New Status: GPS_DISCONNECTED_WIRE");
    break;
  default:
    break;
  }
}

byte Gps_Voilier::gps_get_status(){
  return gps_status;
}

GPS_DATA Gps_Voilier::gps_get_data(){
  return gps_data;
}

void Gps_Voilier::gps_update_data()
{
  switch (gps_status) {
    case GPS_TRYING_CONNECTING_WIRE:
      if (myGNSS.begin() == false) //Connect to the Ublox module using Wire port
      {
        Serial.println(F("Error : Setup Failed. Please check wiring"));
        gps_reconecting_count++;
        if(gps_reconecting_count >= MAX_CONNECTION_ATTEMPT){
          gps_status = GPS_DISCONNECTED_WIRE;
        }
        //delay(200);
      }
      else{
        gps_reconecting_count = 0;
        myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
        myGNSS.setNavigationFrequency(2); //Produce two solutions per second
        myGNSS.setAutoPVT(true); //Tell the GPS to "send" each solution
        myGNSS.saveConfiguration(); //Save the current settings to flash and BBR
        gps_status = GPS_CONNECTED_NETWORK;
      }
      break;
    case GPS_TRYING_RECONNECTING_WIRE:
      if (myGNSS.begin() == false && gps_reconecting_count < MAX_CONNECTION_ATTEMPT) //Connect to the Ublox module using Wire port
      {
        Serial.println(F("Error : u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
        gps_reconecting_count++;
        //delay(200);
      }
      else if(myGNSS.begin() == false && gps_reconecting_count >= MAX_CONNECTION_ATTEMPT){
        gps_reconecting_count = 0;
        gps_status = GPS_DISCONNECTED_WIRE;
        gps_display_status();
      }
      else{
        gps_reconecting_count = 0;
        gps_status = GPS_CONNECTED_NETWORK;
        gps_display_status();
      }
      break;
    case GPS_DISCONNECTED_WIRE:
      if (myGNSS.begin() != false) //Connect to the Ublox module using Wire port
      {
        gps_status = GPS_CONNECTED_NETWORK;
        gps_display_status();
      }
      break;
    case GPS_CONNECTED_NETWORK:
      {
        if(myGNSS.isConnected()){
          int return_value = update_data_gps();
          if(return_value == ERROR_DATA_READ_GPS_ZERO_LAT_OR_LONG){
            gps_status = GPS_DISCONNECTED_NETWORK;
            gps_display_status();
          }
          else if(return_value == GPS_DATA_GET_SUCCESS){
            gps_display_data();
          }
        }
        else{
          gps_status = GPS_TRYING_RECONNECTING_WIRE;   
          gps_display_status(); 
        }
      }
      break;
    case GPS_DISCONNECTED_NETWORK:
      if(myGNSS.isConnected()){
        int return_value = update_data_gps();
        if(return_value == GPS_DATA_GET_SUCCESS){
          gps_status = GPS_CONNECTED_NETWORK;
          gps_display_status();
          gps_display_data();
        }
      }
      else{
        gps_status = GPS_TRYING_RECONNECTING_WIRE;  
        gps_display_status();            
      }
      break;
    default:
        gps_status = GPS_TRYING_RECONNECTING_WIRE;
        gps_display_status();     
      break;
  }
}