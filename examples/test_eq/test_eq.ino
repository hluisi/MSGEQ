// include the libary 
#include <MSGEQ.h>

// define the pins
#define strobePin 7
#define resetPin 8
#define analogPin 14

//define the rate at which to print to Serial
#define dpRATE 10

// the counter for printing
unsigned int dpCOUNT = 0;

// create an eq object
MSGEQ eq = MSGEQ(strobePin, resetPin, analogPin);

void setup() {
  Serial.begin(9600); // start serial 
}

void loop() {
	// inc dpCOUNT
	dpCOUNT++;
	
  // read the eq
  eq.read();

  // print the eq 
  if (dpCOUNT % dpRATE == 0)
    print_eq();
}

// helper functions
void print_eq() {
  for(byte i = 0; i < 7; i++) {
    print_eq_value(eq.spectrum[i].value);
  }
  Serial.println();
}

void print_eq_value(int value) {
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