#ifndef MSGEQ_h
#define MSGEQ_h 

#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
 #include <pins_arduino.h>
#endif


struct audio {
	int raw;
	int range;
	int value;
	int MIN;
	int MAX;
};

class MSGEQ {
private:
  uint8_t strobePin;
  uint8_t resetPin;
  uint8_t analogPin;
  unsigned int rDelay;

public:
  unsigned int DELAY; 
  audio spectrum[7];

  int VOLUME;

  MSGEQ(uint8_t, uint8_t, uint8_t);

  void setResetPin(uint8_t);
  void setStrobePin(uint8_t);
  void setAnalogPin(uint8_t);
  void setDelay(int);
  void read();
};

#endif