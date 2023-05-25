#ifndef SendLogs_h
#define SendLogs_h

#include <Arduino.h>

class SendLogs
{
  public:

    SendLogs();
    void printDouble( double val, unsigned int precision);
    void setStatusSensors(bool S_IMU, bool S_GPS, bool S_GIR);
    void setStatusMode(bool S_MODE);
    void sendLogs(float GPS_lat, 
                  float GPS_long,
                  float IMU_degNord,
                  float GIR_degVent
                  );
  private:
    bool _GPS_STATUS;
    bool _IMU_STATUS;
    bool _GIR_STATUS;
    bool _MODE;
};


#endif