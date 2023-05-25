#define AUTO 0
#define MANUEL_TELECOMMANDE 1
#define MANUEL_KEEPALIVE 2

int state = 0;

//Pin keepAlive = 8
const int KeepAlivepin = 8;

//Pin PWM = 10
const int PWMpin = 10;

//PIN Reset = 9
const int RESETpin = 9;

//switch PIN = 2
const int SWITCHpin = 2;

//Mode = 5 (0 : Auto , Manuel : 1)
const int MODEpin = 5;

unsigned long current_Time;
unsigned long Time_since_alive = 0;
unsigned long Time_since_check = 0;

volatile unsigned long PWM__startTime;
volatile unsigned long PWM_duration;
volatile bool signalDetected = false;


// IRAM_ATTR pour que le code de la fonction soit stocké dans la RAM
void IRAM_ATTR handleInterrupt() {

  if (digitalRead(PWMpin) == HIGH) {
    PWM__startTime = micros();
  } else {
    PWM_duration = micros() - PWM__startTime;
    signalDetected = true;
    //Serial.println(PWM_duration);
  }
}


void setup() {
  Serial.begin(115200);  // déboguer

  //RESET
  pinMode(RESETpin, OUTPUT);
  digitalWrite(RESETpin, HIGH);

  //Mode
  pinMode(MODEpin, OUTPUT);

  //SWITCH
  pinMode(SWITCHpin, OUTPUT);

  //KeepAlive
  pinMode(KeepAlivepin, INPUT);

  // lectue PWM
  pinMode(PWMpin, INPUT);
  attachInterrupt(digitalPinToInterrupt(PWMpin), handleInterrupt, CHANGE);

  //Démarage mode AUTO
  state = AUTO;
  //LOW for auto
  digitalWrite(SWITCHpin, LOW);
  //LOW for auto
  digitalWrite(MODEpin, LOW);
  Serial.println("Mode : AUTO setup");
}

void loop() {
  switch (state) {
    case AUTO:
      // si récéption de KeepAlive (PIN a 1) => reset du temps écoulé depuis dernier KeepAlive
      if ((digitalRead(KeepAlivepin) == 1) && ((millis() - Time_since_check) > 200)) {
        Time_since_alive = millis();
        Time_since_check = millis();
        Serial.println("ALIVE !");
      }

      current_Time = millis();
      //Serial.println(current_Time - Time_since_alive);
      // si le temps écoulé depuis le dernier keepAlive est supérieur à 2sec * 3 => mode Manuel
      if (current_Time - Time_since_alive >= 6000) {
        Serial.println("Mode : MANUEL plus de KeepAlive");
        ///////// PLUS DE RESET /////////
        //state = MANUEL_KEEPALIVE;
        //HIGH for manuel
        digitalWrite(SWITCHpin, HIGH);
        //HIGH for manuel
        digitalWrite(MODEpin, HIGH);
      }
      //ENFIN (ordre de priorité) si signal télécommande MANUEL
      if (signalDetected && (900 <= PWM_duration) && (PWM_duration <= 1100)) {
        Serial.println("Mode : MANUEL Telecommande");
        state = MANUEL_TELECOMMANDE;
        //HIGH for manuel
        digitalWrite(SWITCHpin, HIGH);
        //HIGH for manuel
        digitalWrite(MODEpin, HIGH);
      }
      break;
    case MANUEL_TELECOMMANDE:
      //si signal télécommande AUTO
      if (signalDetected && (1900 <= PWM_duration) && (PWM_duration <= 2100)) {
        Serial.println("Mode : AUTO Telecommande");
        //reset KeepAlive
        Time_since_alive = millis();
        state = AUTO;
        //LOW for auto
        digitalWrite(SWITCHpin, LOW);
        //LOW for auto
        digitalWrite(MODEpin, LOW);
      }
      break;
    case MANUEL_KEEPALIVE:
      // Reset pin
      Serial.println("Reset BOARD");

      digitalWrite(RESETpin, LOW);
      delay(100);
      digitalWrite(RESETpin, HIGH);
      //reset KeepAlive
      Time_since_alive = millis();
      Serial.println("Mode : AUTO après reset");
      state = AUTO;
      //LOW for auto
      digitalWrite(SWITCHpin, LOW);
      //LOW for auto
      digitalWrite(MODEpin, LOW);
      break;
    default:
      Serial.println("Mode : AUTO Par defaut");
      state = AUTO;
      //LOW for auto
      digitalWrite(SWITCHpin, LOW);
      //LOW for auto
      digitalWrite(MODEpin, LOW);
      break;
  }
}