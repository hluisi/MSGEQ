#include "MSGEQ.h"

void MSGEQ::setAnalogPin(uint8_t a) {
  analogPin = a;
  pinMode(analogPin, INPUT);
}

void MSGEQ::setResetPin(uint8_t r) { 
  resetPin = r;
  pinMode(resetPin, OUTPUT);
  digitalWrite(resetPin, LOW);
}

void MSGEQ::setStrobePin(uint8_t s) {
  strobePin = s;
  pinMode(strobePin, OUTPUT);
  digitalWrite(strobePin, LOW);
}

void MSGEQ::setDelay(int value) {
	DELAY = value;
}

MSGEQ::MSGEQ(uint8_t s, uint8_t r, uint8_t a) {
  // set the pins
  setAnalogPin(a);
  setResetPin(r);
  setStrobePin(s);

  // set the eq strobe delay to 30 micro seconds
  setDelay(30);
  VOLUME = 0;

  // init the noise arrays
  for (int i = 0; i < 7; i++) {
    spectrum[i].MAX = -1;
    spectrum[i].MIN = 1024;
  }
  

}

void MSGEQ::read() {
	rDelay++;
  // reset the MSGEQ7
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);
  
  int i;

  // loop through each spectrum and save the values in an array
  for ( i = 0; i < 7; i++) {
    
    // auto correct the sample range
    //if (rDelay % 2 == 0) {
    	spectrum[i].MAX--;
    	spectrum[i].MIN++;
    //}
    

    // Switch spectrum, allow output to settle, read value
    digitalWrite(strobePin, HIGH);
    digitalWrite(strobePin, LOW);
    delayMicroseconds(DELAY); // to allow the output to settle
    spectrum[i].raw = analogRead(analogPin);

    // is the raw spectrum value lower then the MIN value?
    spectrum[i].MIN  = min(spectrum[i].MIN, spectrum[i].raw);
    // is the raw spectrum value higher then the MAX value?
    spectrum[i].MAX = max(spectrum[i].MAX, spectrum[i].raw);
    
    /*
    if (spectrum[i].MIN > 512) 
    	spectrum[i].MIN = 512;
    	
    if (spectrum[i].MAX < 512)
    	spectrum[i].MAX = 512;
   */

    // Set range
    spectrum[i].range = spectrum[i].MAX - spectrum[i].MIN;
    spectrum[i].value = spectrum[i].raw - spectrum[i].MIN;
  }
  int temp = spectrum[0].raw + spectrum[1].raw + spectrum[2].raw + spectrum[3].raw + spectrum[4].raw + spectrum[5].raw + spectrum[6].raw;
  VOLUME = map(temp, 0, 7168, 0, 1024);
  VOLUME = constrain(VOLUME, 0, 1023);
}