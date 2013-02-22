// include the libary 
#include <MSGEQ.h>

// define the pins
#define strobePin 2
#define resetPin 3
#define analogPin 14

//define the rate at which to print to Serial
#define DELAY_RATE 5
#define EQ_DELAY 60

// create an eq object
MSGEQ eq = MSGEQ(strobePin, resetPin, analogPin);

void setup() {
  eq.setDelay(EQ_DELAY);
  Serial.begin(115200); // start serial 
}

void loop() {
  // read the eq
  eq.read();
  
  

  // print the eq 
  for (int i = 0; i < 7; i++) {
    Serial.print(" ");
    //Serial.print(eq.spectrum[i].smoothed);
    Serial.print(eq.spectrum[i].balanced);
    //Serial.print(eq.spectrum[i].raw);
  }
  Serial.print(" ");
  Serial.print(eq.VOLUME);
  
  Serial.println();
  
  delay(DELAY_RATE);
}

/*
void send_eq_value(int value) {
  if (value < 10) 
    Serial.print("   000");
  else if (value < 100) 
    Serial.print("   00");
  else if (value < 1000) 
    Serial.print("   0");
  else
    Serial.print("   ");
  Serial.print(value);
}
*/