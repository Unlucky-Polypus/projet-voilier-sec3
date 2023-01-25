#include <Wire.h> //Needed for I2C to GPS
#include <STM32FreeRTOS.h>
#include <SparkFun_Ublox_Arduino_Library.h> //http://librarymanager/All#SparkFun_u-blox_GNSS
#include "projet_voilier_gps.h"

void setup()
{
  gps_status = GPS_DISCONNECTED_WIRE;
  gps_reconecting_count = 0;
  Serial.begin(115200);
  Wire.setSDA(PIN_SDA);
  Wire.setSCL(PIN_SCL);
  Wire.begin();
}

//Return 0 : Success
//Return -1 : Failed, invalid data with latitude=0 or longitude=0
//Return -2 : Can't pull new data
int update_data_gps(){
  // Calling getPVT returns true if there actually is a fresh navigation solution available.
  // Start the reading only when valid LLH is available
  if (myGPS.getPVT() && (myGPS.getInvalidLlh() == false))
  {
    gps_data.latitude = myGPS.getLatitude();
    gps_data.longitude = myGPS.getLongitude();
    gps_data.altitude = myGPS.getAltitude();
    gps_data.SIV = myGPS.getSIV();

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

void display_data(){
  String s = "Lat: " + gps_data.latitude;
  s = s + " Long: " + gps_data.longitude + " (degrees * 10^-7)" + " Alt: " + gps_data.altitude + " (mm)" + " SIV: " + gps_data.SIV;    
  Serial.println(s);
}

void display_status(){
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

byte get_gps_status(){
  return gps_status;
}

GPS_DATA get_gps_data(){
  return gps_data;
}

void loop()
{
  switch (gps_status) {
    case GPS_TRYING_CONNECTING_WIRE:
      if (myGPS.begin() == false) //Connect to the Ublox module using Wire port
      {
        Serial.println(F("Ublox GPS not detected at I2C address. Please check wiring"));
        delay(50);
      }
      else{
        myGPS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
        myGPS.setNavigationFrequency(2); //Produce two solutions per second
        myGPS.setAutoPVT(true); //Tell the GPS to "send" each solution
        myGPS.saveConfiguration(); //Save the current settings to flash and BBR
        gps_status = GPS_CONNECTED_NETWORK;
      }
      break;
    case GPS_TRYING_RECONNECTING_WIRE:
      if (myGPS.begin() == false && gps_reconecting_count < 10) //Connect to the Ublox module using Wire port
      {
        Serial.println(F("Ublox GPS not detected at I2C address. Please check wiring"));
        gps_reconecting_count++;
        delay(200);
      }
      else if(myGPS.begin() == false && gps_reconecting_count >= 10){
        gps_reconecting_count = 0;
        gps_status = GPS_DISCONNECTED_WIRE;
        display_status();
      }
      else{
        gps_reconecting_count = 0;
        gps_status = GPS_CONNECTED_NETWORK;
        display_status();
      }
      break;
    case GPS_DISCONNECTED_WIRE:
      if (myGPS.begin() == false) //Connect to the Ublox module using Wire port
      {
        Serial.println(F("Ublox GPS not detected at I2C address. Please check wiring"));
        delay(200);
      }
      else{
        gps_status = GPS_CONNECTED_NETWORK;
        display_status();
      }
      break;
    case GPS_CONNECTED_NETWORK:
      {
        if(myGPS.isConnected()){
          int return_value = update_data_gps();
          if(return_value == ERROR_DATA_READ_GPS_ZERO_LAT_OR_LONG){
            gps_status = GPS_DISCONNECTED_NETWORK;
            display_status();
          }
          else if(return_value == GPS_DATA_GET_SUCCESS){
            display_data();
          }
        }
        else{
          gps_status = GPS_TRYING_RECONNECTING_WIRE;   
          display_status(); 
        }
      }
      break;
    case GPS_DISCONNECTED_NETWORK:
      if(myGPS.isConnected()){
        int return_value = update_data_gps();
        if(return_value == GPS_DATA_GET_SUCCESS){
          gps_status = GPS_CONNECTED_NETWORK;
          display_status();
          display_data();
        }
      }
      else{
        gps_status = GPS_TRYING_RECONNECTING_WIRE;  
        display_status();            
      }
      break;
    default:
        gps_status = GPS_TRYING_RECONNECTING_WIRE;
        display_status();     
      break;
  }
}