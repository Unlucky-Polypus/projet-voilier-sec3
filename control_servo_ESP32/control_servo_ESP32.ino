#include <Commandes_voilier_ESP32.h>
#include <Ticker.h>

const int keepAlive = 15;
Ticker timer_keepAlive;


void triggerFunctionKeepAlive() {
  Serial.println("ALIVE !");
  digitalWrite(keepAlive, HIGH);
  delay(100);
  digitalWrite(keepAlive, LOW);
}

void setup() {
  // keepAlive
  // Initialisation de la broche de sortie
  pinMode(keepAlive, OUTPUT);
  digitalWrite(keepAlive, LOW);
  // Initialisation du timer
  timer_keepAlive.attach(2,triggerFunctionKeepAlive);


  Serial.begin(115200);
  // gouvernail pin 9
  set_gouv(9);

  // voile pin 13
  set_voile(13);
}

// the loop routine runs over and over again forever:
void loop() {
  // wait until serial is working
  while (Serial.available() == 0) {
  }

  int commande_gouv;
  int commande_voile = 100;
  int cmd_lue = Serial.parseInt();

  if (cmd_lue != commande_gouv && 0 <= cmd_lue && cmd_lue <= 100) {
    commande_gouv = cmd_lue;
    Serial.print("Commande gouvernail : ");
    Serial.println(commande_gouv);

    // envoie cmd gouvernail
    cmd_gouv(commande_gouv);


    // envoie cmd voile
    cmd_voile(commande_gouv);
  }
}
