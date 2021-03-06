
#include "BluefruitConfig.h"

#define FLOWSENSORPIN 2

// count how many pulses!
volatile uint16_t pulses = 0;
// track the state of the pulse pin
volatile uint8_t lastflowpinstate;
volatile float liters;

SIGNAL(TIMER0_COMPA_vect) {
  uint8_t x = digitalRead(FLOWSENSORPIN);

  if (x == lastflowpinstate) {
    return; // nothing changed!
  }

  if (x == HIGH) {
    //low to high transition!
    pulses++;
  }
  lastflowpinstate = x;
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
  }
}

String readString;
//int count = 0;

void setup() {
  Serial1.begin(9600);
  pinMode(FLOWSENSORPIN, INPUT);
  digitalWrite(FLOWSENSORPIN, HIGH);
  lastflowpinstate = digitalRead(FLOWSENSORPIN);
  useInterrupt(true);
}

void loop()                     // run over and over again
{
  
  while (Serial1.available()) {
    char c = Serial1.read();
    readString += c;
  }

  float prevliters = liters;
  liters = pulses;
  liters *= 0.19; // 0.21 or 0.20
  
  if (readString.length() > 0) {
        if(readString == "1"){
        Serial1.println(liters);
        Serial.println(liters);
      }
    

      readString = "";
    }

 if (liters > prevliters) {
    Serial.println(liters);  
  }
  delay(1000);//the delay before was 100
}

