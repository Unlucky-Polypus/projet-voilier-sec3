#include "Arduino.h"
#include "projet_voilier_commandes_servo.h"


Servo gouvernail;
Servo voile;
////// a appeler dans le setup
void set_gouv(int pin_g){
	gouvernail.attach(pin_g);
	//position neutre
	gouvernail.write(map(50, 0, 100, 50, 130));
	
}
////// a appeler dans le setup
void set_voile(int pin_v){
	voile.attach(pin_v);
	//position neutre
  voile.writeMicroseconds(map(50, 0, 100,1190,1780));
	//voile.write(43);  // positionner le servomoteur état bas
    // ou
	//voile.write(115);  // positionner le servomoteur état haut
}


void cmd_gouv(int cmd_g){
//Pour le gouvernail
  //50 0%
  //130 100%
  //Position neutre
  //90 50%
  
  // test valeur bien comprise entre 0 et 100
  assert(0 <= cmd_g && cmd_g <= 100);
  // envoie commande au servo
  gouvernail.write(map(cmd_g, 0, 100, 50, 130));
}

void cmd_voile(int cmd_v){
//Pour la voile
  //43 0%
  //115 100%
  // test valeur bien comprise entre 0 et 100
  assert(0 <= cmd_v && cmd_v <= 100);
  
  voile.writeMicroseconds(map(cmd_v, 0, 100,1190,1780));
  //voile.write(map(cmd_v, 0, 100, 44, 115));
}