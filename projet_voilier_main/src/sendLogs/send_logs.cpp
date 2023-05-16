#include "send_logs.h"

void sendLogs::begin()
{
  Serial.begin(38400);

  // DOIT ETRE PRESENT SUR LE MAIN //
  /*Wire.setSDA(PIN_SDA);
  Wire.setSCL(PIN_SCL);
  Wire.begin(); 
  gps = new Gps_Voilier();*/
}

void printDouble( double val, unsigned int precision){
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
              )
{
  //latitude  = float(gps->gps_data.latitude)/10000000;  - A AJOUTER SUR CODE PRINCIPAL
  //longitude = float(gps->gps_data.longitude)/10000000; - A AJOUTER SUR CODE PRINCIPAL

  Serial.println("SOF");
  Serial.println("\"GPS\":");
  Serial.print("{\"latitude\" : ");
  printDouble(GPS_lat,100000);
  Serial.print(", \"longitude\" : ");
  printDouble(GPS_long,100000);
  Serial.println("},");

  Serial.println("\"IMU\": [");

  Serial.println("{\"Compas\":");
  Serial.print("{\"x\" : ");
  printDouble(IMU_comp_x,1000);
  Serial.print(", \"y\" : ");
  printDouble(IMU_comp_y,1000);
  Serial.print(", \"z\" : ");
  printDouble(IMU_comp_z,1000);
  Serial.println("}},");

  Serial.println("{\"Gyroscope\":");
  Serial.print("{\"x\" : ");
  printDouble(IMU_gyr_x,100000);
  Serial.print(", \"y\" : ");
  printDouble(IMU_gyr_y,100000);
  Serial.print(", \"z\" : ");
  printDouble(IMU_gyr_z,100000);
  Serial.println("}},");

  Serial.println("{\"Acceleration\":");
  Serial.print("{\"x\" : ");
  printDouble(IMU_acc_x,100);
  Serial.print(", \"y\" : ");
  printDouble(IMU_acc_y,100);
  Serial.print(", \"z\" : ");
  printDouble(IMU_acc_z,100);
  Serial.println("}},");

  Serial.print("{\"degNord\": ");
  printDouble(IMU_degNord,1000);
  Serial.println("}");

  Serial.println("],");

  Serial.println("\"GIROUETTE\":");
  Serial.print("{\"degVent\": ");
  printDouble(GIR_degVent,1000000);
  Serial.println("}");

  Serial.println("EOF");
  Serial.flush();
  Serial.println("");
}