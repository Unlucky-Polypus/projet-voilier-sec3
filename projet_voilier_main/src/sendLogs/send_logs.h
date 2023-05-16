#ifndef SendLogs_h
#define SendLogs_h

class sendLogs
{
  public:
    void begin();
    void printDouble( double val, unsigned int precision);
    void sendLogs(float GPS_lat, 
                  float GPS_long,
                  float IMU_degNord,
                  float IMU_comp_x,
                  float IMU_comp_y,
                  float IMU_comp_z,
                  float IMU_gyr_x,
                  float IMU_gyr_y,
                  float IMU_gyr_z,
                  float IMU_acc_x,
                  float IMU_acc_y,
                  float IMU_acc_z,
                  float GIR_degVent
                  );
};

#endif