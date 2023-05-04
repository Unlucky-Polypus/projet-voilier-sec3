#include "windsensor.h" 

//const float Windabslolute = 360;

void setup()
{
  Serial.begin(115200); //start serial - tip: don't use serial if you don't need it (speed considerations)
  // while(1)
  // {
  //   Serial.println(100);
  //   delay(1000);
  //   Serial.println(110);
  //   delay(1000);
  //   Serial.println(120);
  //   delay(1000);
  // }

  windSetup();

}

void loop()
{    
  Serial.println(getWindAngle());
  delay(50);
}
