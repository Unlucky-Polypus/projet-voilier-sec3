#include <Wire.h> //Needed for I2C to GPS
#include <SparkFun_u-blox_GNSS_Arduino_Library.h> //http://librarymanager/All#SparkFun_u-blox_GNSS
#include <climits>

#define IMU_ADDR 0x60 // 9DOF IMU I2C address
#define REGISTER_VALUE_ANGLE 0x02
#define CALIBRATION_REGISTER 0x1E

class Imu_Voilier {

  private: 

    int16_t angle_value;
    bool calibrated;
    void read_calibration_status();

  public : 

    Imu_Voilier();
    bool imu_update_data();
    int16_t imu_get_data();
    int16_t imu_get_data_with_negative();
    void imu_display_data();
};