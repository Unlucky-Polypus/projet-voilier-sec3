#include "src/projet_voilier_gps/projet_voilier_gps.h"
#include "src/projet_voilier_commandes_servo/projet_voilier_commandes_servo.h"
#include "src/projet_voilier_girouette/projet_voilier_girouette.h"
#include "src/projet_voilier_imu/projet_voilier_imu.h"

long double latitude   = 48.75608;
long double longitude  = 2.30203;

long double comp_x      = 2.583;
long double comp_y      = -4.439;
long double comp_z      = 0.435;

long double gyr_x      = 2.56433;
long double gyr_y      = -4.45352;
long double gyr_z      = 0.47237;

long double acc_x      = 0.29;
long double acc_y      = 0.01;
long double acc_z      = 0.93;

long double posNord    = 47.743;

long double posVent    = 76.46389;

char buf[20];

Gps_Voilier* gps;

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

void setup() {
  Serial.begin(9600);
  Wire.begin(); 
  //Wire.begin(PIN_SCL, PIN_SDA, 400000); // initialize I2C with G21 as SDA and G22 as SCL, with 400 kHz clock frequency
  //  Serial.begin(115200);
  gps = new Gps_Voilier();

}

void loop() {
  gps->gps_update_data();
  //gps->gps_get_data();

  latitude  = float(gps->gps_data.latitude)/10000000;
  longitude = float(gps->gps_data.longitude)/10000000;

  delay(500);  
  Serial.println("SOF");
  Serial.println("\"GPS\":");
  Serial.print("{\"latitude\" : ");
  printDouble(latitude,100000);
  Serial.print(", \"longitude\" : ");
  printDouble(longitude,100000);
  Serial.println("},");

  Serial.println("\"GROVE\": [");

  Serial.println("{\"Compas\":");
  Serial.print("{\"x\" : ");
  printDouble(comp_x,1000);
  Serial.print(", \"y\" : ");
  printDouble(comp_y,1000);
  Serial.print(", \"z\" : ");
  printDouble(comp_z,1000);
  Serial.println("}},");

  Serial.println("{\"Gyroscope\":");
  Serial.print("{\"x\" : ");
  printDouble(gyr_x,100000);
  Serial.print(", \"y\" : ");
  printDouble(gyr_y,100000);
  Serial.print(", \"z\" : ");
  printDouble(gyr_z,100000);
  Serial.println("}},");

  Serial.println("{\"Acceleration\":");
  Serial.print("{\"x\" : ");
  printDouble(acc_x,100);
  Serial.print(", \"y\" : ");
  printDouble(acc_y,100);
  Serial.print(", \"z\" : ");
  printDouble(acc_z,100);
  Serial.println("}},");

  Serial.print("{\"degNord\": ");
  printDouble(posNord,1000);
  Serial.println("}");

  Serial.println("],");

  Serial.println("\"GIROUETTE\":");
  Serial.print("{\"degVent\": ");
  printDouble(posVent,1000000);
  Serial.println("}");
/*
  Serial.println("Coordonees :");
  
  
  Serial.println();
  Serial.println("Gyroscope :");
  sprintf(buf, " - x = %f", gyr_x);
  Serial.println(buf);
  sprintf(buf, " - y = %f", gyr_y);
  Serial.println(buf);
  sprintf(buf, " - z = %f", gyr_z);
  Serial.println(buf);
*/

  Serial.println("EOF");
  Serial.flush();
  Serial.println("");


  //gps->gps_display_data();
  //gps->gps_display_status();
  
}


