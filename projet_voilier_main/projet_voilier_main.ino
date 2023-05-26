#include "src/projet_voilier_gps/projet_voilier_gps.h"
#include "src/projet_voilier_commandes_servo/projet_voilier_commandes_servo.h"
#include "src/projet_voilier_girouette/projet_voilier_girouette.h"
#include "src/projet_voilier_imu/projet_voilier_imu.h"
#include "src/sendLogs/SendLogs.h"
#include "src/projet_voilier_navigator/Navigator.h"
#include "src/projet_voilier_navigator/utils.h"

#include <math.h>
#include <stdio.h>


//Mode auto/manuel
//0 - auto
//1 - manuel
bool old_mode;

//Espace dédié au logs
SendLogs comXbee;
bool Etat_GPS, Etat_IMU, Etat_Gir;
bool data_rdy_gps, data_rdy_imu, data_rdy_gir; 

//Espace dédié à la navigation 
Point bouee = {47.2734979, -2.213848};
Point depart = {47.2734979, -2.213848};
const float distance_bouee = 3.0;
bool reached_bouy;

Navigator nav(bouee);

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
  if(gps->gps_update_status()){
    if(gps->update_data_gps() == GPS_DATA_GET_SUCCESS){
      data_rdy_gps = true;
    }
    Etat_GPS = true;
  }
  else{
    Etat_GPS = false;
  }

  //GET GIROUETTE
  girouette_value = getWindAngle();
  if(girouette_value != ERROR_GIROUETTE_VALUE){
    Etat_Gir = true;
    data_rdy_gir = true;
  }
  else{
    Etat_Gir = false;
    data_rdy_gir = false;
  }

  //GET IMU
  if(imu_is_ready){
    imu->imu_update_data();
    data_rdy_imu = true;
    imu_is_ready = imu->read_calibration_status();
    Etat_IMU = imu_is_ready;
  }
  else{
    imu_is_ready = imu->read_calibration_status();
    Etat_IMU = false;
  }
}

void send_data(){
  float tmp_gps_lat,tmp_gps_long,tmp_imu,tmp_gir = -999.99;
  comXbee.setStatusSensors(Etat_IMU,Etat_GPS,Etat_Gir);
  comXbee.setStatusMode(old_mode);
  if(Etat_IMU){
    tmp_imu = float(imu->imu_get_data_with_negative())/10;
  }
  if(Etat_Gir){
    tmp_gir = girouette_value;
  }
  if(Etat_GPS){
    tmp_gps_lat = gps->gps_get_data().latitude;
    tmp_gps_long = gps->gps_get_data().longitude;
  }
  comXbee.sendLogs(tmp_gps_lat,tmp_gps_long,tmp_imu,tmp_gir);
}

void do_action(){
  // Serial.print("Values : ");
  // Serial.print(data_rdy_gir);
  // Serial.print(data_rdy_imu);
  // Serial.print(data_rdy_gps);
  // Serial.print(" ");
  // Serial.print(gps->gps_get_data().latitude);
  // Serial.print("\n");
  if(data_rdy_gps && data_rdy_imu && data_rdy_gir){
    data_rdy_gir = false;
    data_rdy_gps = false; 
    data_rdy_imu = false; 
    Serial.println("Send new data to command");
    // Display commands for a course of 90° from North and 45° of wind from the boat
    Point pos_bateau = {gps->gps_get_data().latitude, gps->gps_get_data().longitude};
    CommandOutput commands = nav.navigate(pos_bateau, girouette_value, float(imu->imu_get_data_with_negative())/10);
    cmd_gouv((int)commands.safran);
    cmd_voile((int)commands.sails);
    Serial.print("Safran output : ");
    Serial.print(commands.safran, 2);
    Serial.print(", Sails output : ");
    Serial.println(commands.sails, 2);
  }
}

void reached_bouee(){
  if(nav.arrived(distance_bouee) && !reached_bouy){
    nav.setBuoy(depart);
    reached_bouy = true;
  }
}

void setup() {
  Serial.begin(38400);
  pinMode(pin_keepAlive, OUTPUT);
  digitalWrite(pin_keepAlive, LOW);
  Wire.begin(); 
  Serial.println("Begin initialization");
  gps = new Gps_Voilier();
  Serial.println("GPS initialized");
  imu = new Imu_Voilier();
  comXbee = SendLogs();
  Serial.println("IMU initialized");
  windSetup();
  Serial.println("Girouette initialized");
  set_gouv(pin_gouv);
  Serial.println("Gouv initialized");
  set_voile(pin_voile);
  Serial.println("Voile initialized");
  //Initialisation des variables
  //Mode auto/manuel
  pinMode(5,INPUT);
  old_mode = 0;
  imu_is_ready = false;
  Etat_GPS = false;
  Etat_IMU = false;
  Etat_Gir = false;
  data_rdy_gir = false;
  data_rdy_gps = false; 
  data_rdy_imu = false; 
  reached_bouy = false;
  girouette_value = ERROR_GIROUETTE_VALUE;
}

void loop() {
  get_sensor_data();
  send_data();
  //do_action();
  if(digitalRead(5) != old_mode){
    //envoi nouveau mod
    old_mode != old_mode;
  }
  reached_bouee();
  triggerFunctionKeepAlive();
}
