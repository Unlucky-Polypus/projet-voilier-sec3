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

class Gps_Voilier {

  public : 

    SFE_UBLOX_GNSS myGNSS;
    GPS_DATA gps_data;
    byte gps_status;
    byte gps_reconecting_count;
    bool booted;

    int update_data_gps();

    Gps_Voilier();
    void gps_display_data();
    GPS_DATA gps_get_data();
    bool gps_update_status();

};