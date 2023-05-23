#include <Navigator.h>

Point nantes = {47.218371, -1.553621};
Point saint_nazaire = {47.2734979, -2.213848};

// Create a navigator which destination is Saint-Nazaire
// Destination can be changed with the function setBuoy(Point dest);
Navigator nav(saint_nazaire);

void setup() {
  delay(1000);
  Serial.begin(9600); // open the serial port at 9600 bps:
  while (!Serial);

}

void loop() {
  // Display commands for a course of 90° from North and 45° of wind from the boat
  CommandOutput commands = nav.navigate(nantes, 45, 90);
  Serial.print("Safran output : ");
  Serial.print(commands.safran, 2);
  Serial.print(", Sails output : ");
  Serial.println(commands.sails, 2);
  delay(1000);
}
