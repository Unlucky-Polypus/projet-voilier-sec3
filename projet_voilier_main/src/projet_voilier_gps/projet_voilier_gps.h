#include <Wire.h> //Needed for I2C to GPS
#include <SparkFun_u-blox_GNSS_Arduino_Library.h> //http://librarymanager/All#SparkFun_u-blox_GNSS
#include <climits>

#define RAISED_ERROR_GPS_INIT_FAILED -1

#define MAX_CONNECTION_ATTEMPT 10

#define ERROR_DATA_READ_GPS_ZERO_LAT_OR_LONG -1
#define WARNING_CANNOT_GET_NEW_PVT -2
#define GPS_DATA_GET_SUCCESS 0

struct GPS_DATA {
    int32_t latitude;
    int32_t longitude;
    int32_t altitude;
    byte SIV;
};

enum {GPS_CONNECTED_NETWORK, GPS_DISCONNECTED_NETWORK, 
      GPS_TRYING_RECONNECTING_WIRE, GPS_TRYING_CONNECTING_WIRE, GPS_DISCONNECTED_WIRE };

class Gps_Voilier {

  public : 

    SFE_UBLOX_GNSS myGNSS;
    GPS_DATA gps_data;
    byte gps_status;
    byte gps_reconecting_count;

    int update_data_gps();

    Gps_Voilier();
    void gps_update_data();
    void gps_display_data();
    void gps_display_status();
    byte gps_get_status();
    GPS_DATA gps_get_data();

};