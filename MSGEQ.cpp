#include "MSGEQ.h"

// Analog pin setup
void MSGEQ::setAnalogPin(uint8_t a) {
  analogPin = a;
  pinMode(analogPin, INPUT);
}

// Reset pin setup
void MSGEQ::setResetPin(uint8_t r) { 
  resetPin = r;
  pinMode(resetPin, OUTPUT);
  // we set the MSGEQ7 reset pin high and keep it that way
  // until we are ready to read from the chip
  digitalWrite(resetPin, HIGH);                             
}

// Strobe pin setup
void MSGEQ::setStrobePin(uint8_t s) {
  strobePin = s;
  pinMode(strobePin, OUTPUT);
  digitalWrite(strobePin, LOW);
}

// Sets the delay between strobes that allows the MSGEQ values to settle before reading them
void MSGEQ::setDelay(int value) {
	DELAY = value;
}

// Constructor
MSGEQ::MSGEQ(uint8_t s, uint8_t r, uint8_t a) {
  setAnalogPin(a);   // Set the analog pin
  setResetPin(r);    // Set the reset pin
  setStrobePin(s);   // Set the strobe pin
  setDelay(30);      // Set the default delay to 30 microseconds
  VOLUME = 0;        // Set volume to 0
  smooth_count = -1; // Set smooth count to -1 so its 0 the first time through the read loop
  beat_count = 0;
  bpm_start = millis();
  last_range = 0;

  // Init the arrays
  for (int i = 0; i < 7; i++) {
    spectrum[i].MAX = -1;    // Set MAX low
    spectrum[i].MIN = 1024;  // Set MIN high
    
    // Set all smooth values to 0
    for (int j = 0; j < smooth_total; j++) {
    	spectrum[i].raw_smooth[j] = 0; 
    	spectrum[i].min_smooth[j] = 0;
    	spectrum[i].max_smooth[j] = 0;
    }
  }
}

// Main running loop
void MSGEQ::read() {
	smooth_count++;  // Inc the smooth count
	
	// If we reached the end of the smooth array, go to the begining
	if (smooth_count == smooth_total) {
		smooth_count = 0;
  }
  
  // Set reset pin low, which resets the multiplexor of the chip back to the first band for reading
  digitalWrite(resetPin, LOW);
  
  // loop through each spectrum and save the values in an array
  for (int i = 0; i < 7; i++) {
  	int check = millis();
  	
  	
  	if (check - bpm_start > 1000) {
  		BPM = beat_count; // * 2;
  		beat_count = 0;
  		bpm_start = millis();
  	}
  	
  	// Try to pushing the MAX down and the MIN up keeping
  	// the range of the waveform somewhat balanced
  	spectrum[i].MAX--;
  	spectrum[i].MIN++;
    

    // Switch spectrum, allow output to settle, read value
    //if (i != 0) {
    // Send strobe, switch the multiplexor to the next eq band
      digitalWrite(strobePin, HIGH);
      digitalWrite(strobePin, LOW);
    //}
    // Read and set the raw data
    delayMicroseconds(DELAY);                               // to allow the output to settle
    spectrum[i].raw = analogRead(analogPin);                // read the analog value
    spectrum[i].raw_smooth[smooth_count] = spectrum[i].raw; // set the raw into the smooth array

    // Set the MIN, MAX, range values
    spectrum[i].MIN  = min(spectrum[i].MIN, spectrum[i].raw); // is the raw value lower then the MIN value?
    spectrum[i].MAX = max(spectrum[i].MAX, spectrum[i].raw);  // is the raw value higher then the MAX value?
    spectrum[i].range = spectrum[i].MAX - spectrum[i].MIN;    // the range between the top and bottom of the current waveform (kindof..)
    
    // Set the min and max smoothed arrays
    spectrum[i].min_smooth[smooth_count] = spectrum[i].MIN;
    spectrum[i].max_smooth[smooth_count] = spectrum[i].MAX;
    
    // reset the smoothed values 
    spectrum[i].smoothed        = 0;
    spectrum[i].range_smoothed  = 0;
    spectrum[i].min_smoothed    = 0;
    spectrum[i].max_smoothed    = 0;
    
    // Now that all the current values are set, loop through the smooth arrays
    // and set the smoothed values
    for (int j = 0; j < smooth_total; j++) {
    	spectrum[i].smoothed     += spectrum[i].raw_smooth[j];
    	spectrum[i].min_smoothed += spectrum[i].min_smooth[j];
    	spectrum[i].max_smoothed += spectrum[i].max_smooth[j];
    }
    
    spectrum[i].smoothed     /= smooth_total;
    spectrum[i].min_smoothed /= smooth_total;
    spectrum[i].max_smoothed /= smooth_total;
    
    spectrum[i].range_smoothed = spectrum[i].max_smoothed - spectrum[i].min_smoothed;
    if (spectrum[i].range_smoothed < range_cutoff) {
    	spectrum[i].range_smoothed = range_cutoff;
    }
    spectrum[i].balanced = map(spectrum[i].smoothed - spectrum[i].min_smoothed, 0, spectrum[i].range_smoothed, 0, 1023);
  }
  int temp = (spectrum[0].raw + spectrum[1].raw + spectrum[2].raw + spectrum[3].raw + spectrum[4].raw + spectrum[5].raw) / 6; // + spectrum[6].raw;
  VOLUME = map(temp, 80, 1023, 0, 1023);
  VOLUME = constrain(VOLUME, 0, 1023);
  
  if (spectrum[0].range_smoothed > last_range) {
  	beat_count++;
  }
  
  last_range = spectrum[0].range_smoothed;
  digitalWrite(resetPin, HIGH);
}