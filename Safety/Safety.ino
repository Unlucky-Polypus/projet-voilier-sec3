#define AUTO 0
#define MANUEL_TELECOMMANDE 1
#define MANUEL_KEEPALIVE 2

int state = 0;

//Pin keepAlive = 4
const int KeepAlivepin = 4;

//Pin PWM
const int PWMpin = 5;

//PIN Reset
const int RESETpin = 14;

//switch PIN
const int SWITCHpin = 27;

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

  //SWITCH
  pinMode(SWITCHpin, OUTPUT);

  //KeepAlive
  pinMode(KeepAlivepin, INPUT);

  // lectue PWM
  pinMode(PWMpin, INPUT);
  attachInterrupt(digitalPinToInterrupt(PWMpin), handleInterrupt, CHANGE);

  //Démarage mode AUTO
  state = AUTO;
  // low for auto
  digitalWrite(SWITCHpin, HIGH);
  Serial.println("Mode : AUTO setup");
}

void loop() {
  switch (state) {
    case AUTO:
      // si récéption de KeepAlive (PIN a 1) => reset du temps écoulé depusi dernier KeepAlive
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
        // high for manuel
        digitalWrite(SWITCHpin, HIGH);
        state = MANUEL_KEEPALIVE;
      }
      //ENFIN (ordre de priorité) si signal télécommande MANUEL
      if (signalDetected && (900 <= PWM_duration) && (PWM_duration <= 1100)) {
        Serial.println("Mode : MANUEL Telecommande");
        // high for manuel
        digitalWrite(SWITCHpin, HIGH);
        state = MANUEL_TELECOMMANDE;
      }
      break;
    case MANUEL_TELECOMMANDE:
      //si signal télécommande AUTO
      if (signalDetected && (1900 <= PWM_duration) && (PWM_duration <= 2100)) {
        Serial.println("Mode : AUTO Telecommande");
        //reset KeepAlive
        Time_since_alive = millis();
        // low for auto
        digitalWrite(SWITCHpin, LOW);
        state = AUTO;
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
      // low for auto
      digitalWrite(SWITCHpin, LOW);
      state = AUTO;
      break;
    default:
      Serial.println("Mode : AUTO Par defaut");
      // low for auto
      digitalWrite(SWITCHpin, LOW);
      state = AUTO;
      break;
  }
}
