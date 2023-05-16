#include "src/projet_voilier_gps/projet_voilier_gps.h"
#include "src/projet_voilier_commandes_servo/projet_voilier_commandes_servo.h"
#include "src/projet_voilier_girouette/projet_voilier_girouette.h"
#include "src/projet_voilier_imu/projet_voilier_imu.h"
#include "src/sendLogs/send_logs.h"

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//Espace dédié au controle moteur
const uint8_t pin_keepAlive = 8;
const uint8_t pin_gouv = 2;
const uint8_t pin_voile = 3;

//Espace dédié au keep alive
void triggerFunctionKeepAlive() {
  digitalWrite(pin_keepAlive, HIGH);
  delay(100);
  digitalWrite(pin_keepAlive, LOW);
}

//Espace dédié à l'imu 
Imu_Voilier* imu;
bool imu_is_ready;

//Espace dédié au GPS
Gps_Voilier* gps;

//Espace dédié à la girouette 
int girouette_value; 

void get_sensor_data(){
  //GET GPS
  gps->gps_update_data();

  //GET GIROUETTE
  girouette_value = getWindAngle();

  //GET IMU
  if(imu_is_ready == true){
    imu->imu_update_data();
  }
  else{
    imu_is_ready = imu->read_calibration_status();
  }
}

void send_data(){

  //GPS
  if(gps->gps_get_status() == GPS_CONNECTED_NETWORK){
    Serial.print("SEND GPS data : ");
    gps->gps_display_data();
    Serial.print("\n");
  }
  else{
    Serial.println("SEND Error GPS");
  }

  //GIROUETTE 
  if(girouette_value != ERROR_GIROUETTE_VALUE){
    Serial.print("Valeur de la girouette : ");
    Serial.print(girouette_value);
    Serial.print("\n");
  }
  else{
    Serial.println("SEND Error Girouette");
  }

  //IMU
  if(imu_is_ready){
    Serial.print("SEND IMU VALUE : ");
    imu->imu_get_data_with_negative();
    Serial.print("\n");
  }
  else{
    Serial.println("SEND Error IMU");
  }

}

void do_action(){
  //TODO
}

void setup() {
  Serial.begin(9600);
  pinMode(pin_keepAlive, OUTPUT);
  digitalWrite(pin_keepAlive, LOW);
  Wire.begin(); 
  Serial.println("Begin initialization");
  gps = new Gps_Voilier();
  Serial.println("GPS initialized");
  imu = new Imu_Voilier();
  Serial.println("IMU initialized");
  windSetup();
  Serial.println("Girouette initialized");
  set_gouv(pin_gouv);
  Serial.println("Gouv initialized");
  set_voile(pin_voile);
  Serial.println("Voile initialized");
  //Initialisation des variables
  imu_is_ready = false;
  girouette_value = ERROR_GIROUETTE_VALUE;
}

void loop() {

  get_sensor_data();
  Serial.println("Bonjour");
  send_data();
  //triggerFunctionKeepAlive();
  delay(100);

  //Serial.println(getWindAngle());

  // gps->gps_update_data();
  // //gps->gps_get_data();

  // latitude  = float(gps->gps_data.latitude)/10000000;
  // longitude = float(gps->gps_data.longitude)/10000000;

  // delay(500);  
  // Serial.println("SOF");
  // Serial.println("\"GPS\":");
  // Serial.print("{\"latitude\" : ");
  // printDouble(latitude,100000);
  // Serial.print(", \"longitude\" : ");
  // printDouble(longitude,100000);
  // Serial.println("},");
  
}


