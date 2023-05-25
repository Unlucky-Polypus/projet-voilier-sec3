#include "SendLogs.h"

HardwareSerial MySerial(0);

SendLogs::SendLogs()
{
  MySerial.begin(38400, SERIAL_8N1, RX, TX);
  //Serial.begin(9600);
  _GPS_STATUS = 0; // 0 = KO, 1 = OK
  _IMU_STATUS = 0; // 0 = KO, 1 = OK
  _GIR_STATUS = 0; // 0 = KO, 1 = OK
  _MODE = 0; // 0 = AUTO, 1 = MANUEL
}

void SendLogs::setStatusSensors(bool S_IMU, bool S_GPS, bool S_GIR)
{
  _IMU_STATUS = S_IMU;
  _GPS_STATUS = S_GPS;
  _GIR_STATUS = S_GIR;
}

void SendLogs::setStatusMode(bool S_MODE)
{
  _MODE = S_MODE;
}

void SendLogs::printDouble( double val, unsigned int precision){
// prints val with number of decimal places determine by precision
// NOTE: precision is 1 followed by the number of zeros for the desired number of decimial places
// example: printDouble( 3.1415, 100); // prints 3.14 (two decimal places)

    MySerial.print (int(val));  //prints the int part
    MySerial.print("."); // print the decimal point
    unsigned int frac;
    if(val >= 0)
      frac = (val - int(val)) * precision;
    else
       frac = (int(val)- val ) * precision;
    int frac1 = frac;
    while( frac1 /= 10 )
        precision /= 10;
    precision /= 10;
    while(  precision /= 10)
        MySerial.print("0");

    MySerial.print(frac,DEC) ;
}

void SendLogs::sendLogs(float GPS_lat, 
              float GPS_long,
              float IMU_degNord,
              float GIR_degVent
              )
{
  MySerial.println("SOF");

  MySerial.println("\"SYSTEM STATUS\" :");
  MySerial.print("{\"GPS\" : ");
  MySerial.print(_GPS_STATUS);
  MySerial.print(", \"IMU\" : ");
  MySerial.print(_IMU_STATUS);
  MySerial.print(", \"GIR\" : ");
  MySerial.print(_GIR_STATUS);
  MySerial.print(", \"MODE\" : ");
  MySerial.print(_MODE);
  MySerial.println("},");

  if(_GPS_STATUS) {
    MySerial.println("\"GPS\" :");
    MySerial.print("{\"latitude\" : ");
    printDouble(GPS_lat/10000000,100000);
    MySerial.print(", \"longitude\" : ");
    printDouble(GPS_long/10000000,100000);
    MySerial.println("},");
  }
  
  if(_IMU_STATUS) {
    MySerial.println("\"IMU\" :");
    MySerial.print("{\"degNord\" : ");
    printDouble(IMU_degNord,1000);
    MySerial.println("},");
  }
  
  if(_GIR_STATUS) {
    MySerial.println("\"GIROUETTE\":");
    MySerial.print("{\"degVent\": ");
    printDouble(GIR_degVent,1000000);
    MySerial.println("},");
  }
  
  MySerial.println("EOF");
  MySerial.flush();
  MySerial.println("");
}