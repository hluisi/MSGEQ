#ifndef MSGEQ_h
#define MSGEQ_h

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#include <pins_arduino.h>
#endif

#define smooth_total 7   // Size of the smoothing arrays
#define range_cutoff 400
#define BPM_TIME 30000;

struct audio {
  int raw;                      // The current raw analog value
  int smoothed;                 // The raw value smoothed over time
  int range;                    // The raw range (low to high) of the waveform
  int balanced;                 // The raw value balanced between the smoothed min and max of the waveform
  int MIN;                      // The low end of the waveform  (kinda...)
  int MAX;                      // The high end of the waveform (kinda...)
  int min_smoothed;             // The min of the waveform smoothed over time
  int max_smoothed;             // The max of the waveform smoothed over time
  int range_smoothed;           // The range (low to high) of the waveform smoothed over time
  int raw_smooth[smooth_total]; // Smoothing array of raw values
  int min_smooth[smooth_total]; // Smoothing array of min values
  int max_smooth[smooth_total]; // Smoothing array of max values
};

class MSGEQ {
private:
  uint8_t strobePin;  // MSGEQ strobe pin
  uint8_t resetPin;   // MSGEQ reset pin
  uint8_t analogPin;  // MSGEQ analog pin
  unsigned int smooth_count;  // Count used for smoothing arrays
  unsigned int beat_count;
  unsigned int bpm_start;
  int last_range;
  

  void setResetPin(uint8_t);    // Set's the reset pin
  void setStrobePin(uint8_t);   // Set's the strobe pin
  void setAnalogPin(uint8_t);   // Set's the analog pin

public:
  unsigned int DELAY; // The delay between strobes that allows the MSGEQ values to settle before reading them
  audio spectrum[7];  // MSGEQ spectrum array of the audio structure

  int VOLUME;         // Current VOLUME
  int BPM;

    MSGEQ(uint8_t, uint8_t, uint8_t);  // constructor

  void setDelay(int);  // Set's the MSGEQ strobing delay (see DELAY above) void
  void read();         // where the magic happens };
};


#endif