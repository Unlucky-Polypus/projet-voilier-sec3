#include "Arduino.h"
#include "Servo.h"
#include "assert.h"

// set les pins des deux servo
// a utiliser dans le setup souvent pin 9 et .. (pwm acceptée non-inversée)
void set_gouv(int pin_g);
void set_voile(int pin_v);

// reçoit une commande entre 0% (gauche / bâbord) et 100% (droite / tribord)
// envoie une commande au servo
void cmd_gouv(int cmd_g);
void cmd_voile(int cmd_v);
