#include <Wire.h> //Needed for I2C to GPS
#include <STM32FreeRTOS.h>
#include <SparkFun_Ublox_Arduino_Library.h> //http://librarymanager/All#SparkFun_u-blox_GNSS

SFE_UBLOX_GPS myGPS;

#define ERROR_DATA_READ_GPS_ZERO_LAT_OR_LONG -1
#define WARNING_CANNOT_GET_NEW_PVT -2
#define GPS_DATA_GET_SUCCESS 0

enum {GPS_CONNECTED_NETWORK, GPS_DISCONNECTED_NETWORK, 
      GPS_TRYING_RECONNECTING_WIRE, GPS_TRYING_CONNECTING_WIRE, GPS_DISCONNECTED_WIRE };

struct GPS_DATA {
    long latitude;
    long longitude;
    long altitude;
    byte SIV;
};

byte gps_status;
byte gps_reconecting_count;
GPS_DATA gps_data;
SemaphoreHandle_t sem;

void setup_gps();

void display_data();

void display_status();

byte get_gps_status();

GPS_DATA get_gps_data();