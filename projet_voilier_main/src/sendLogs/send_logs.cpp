#include "SendLogs.h"

SendLogs::SendLogs()
{
  Serial.begin(38400);
  _GPS_STATUS = 1;
  _IMU_STATUS = 1;
  _GIR_STATUS = 1;

  // DOIT ETRE PRESENT SUR LE MAIN //
  /*Wire.setSDA(PIN_SDA);
  Wire.setSCL(PIN_SCL);
  Wire.begin(); 
  gps = new Gps_Voilier();*/
}

void SendLogs::setStatusSensors(bool S_IMU, bool S_GPS, bool S_GIR)
{
  _IMU_STATUS = S_IMU;
  _GPS_STATUS = S_GPS;
  _GIR_STATUS = S_GIR;
  
}

void SendLogs::printDouble( double val, unsigned int precision){
// prints val with number of decimal places determine by precision
// NOTE: precision is 1 followed by the number of zeros for the desired number of decimial places
// example: printDouble( 3.1415, 100); // prints 3.14 (two decimal places)

    Serial.print (int(val));  //prints the int part
    Serial.print("."); // print the decimal point
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
        Serial.print("0");

    Serial.print(frac,DEC) ;
}

void SendLogs::sendLogs(float GPS_lat, 
              float GPS_long,
              float IMU_degNord,
              float GIR_degVent
              )
{
  //latitude  = float(gps->gps_data.latitude)/10000000;
  //longitude = float(gps->gps_data.longitude)/10000000;

  Serial.println("SOF");

  Serial.println("\"SENSORS STATUS\" :");
  Serial.print("{\"GPS\" : ");
  Serial.print(_GPS_STATUS);
  Serial.print(", \"IMU\" : ");
  Serial.print(_IMU_STATUS);
  Serial.print(", \"GIR\" : ");
  Serial.print(_GIR_STATUS);
  Serial.println("},");

  if(_GPS_STATUS) {
    Serial.println("\"GPS\" :");
    Serial.print("{\"latitude\" : ");
    printDouble(GPS_lat/10000000,100000);
    Serial.print(", \"longitude\" : ");
    printDouble(GPS_long/10000000,100000);
    Serial.println("},");
  }
  
  if(_IMU_STATUS) {
    Serial.println("\"IMU\" :");
    Serial.print("{\"degNord\" : ");
    printDouble(IMU_degNord,1000);
    Serial.println("}");
  }
  
  if(_GIR_STATUS) {
    Serial.println("\"GIROUETTE\":");
    Serial.print("{\"degVent\": ");
    printDouble(GIR_degVent,1000000);
    Serial.println("}");
  }
  
  Serial.println("EOF");
  Serial.flush();
  Serial.println("");
}