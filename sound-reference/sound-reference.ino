// Arduino Zero / Feather M0 I2S audio tone generation example.
// Author: Tony DiCola
//
// Connect an I2S DAC or amp (like the UDA1334A) to the Arduino Zero
// and play back simple sine, sawtooth, triangle, and square waves.
// Makes your Zero sound like a NES!
//
// NOTE: The I2S signal generated by the Zero does NOT have a MCLK /
// master clock signal.  You must use an I2S receiver that can operate
// without a MCLK signal (like the UDA1334A).
//
// For an Arduino Zero / Feather M0 connect it to you I2S hardware as follows:
// - Digital 0 -> I2S LRCLK / FS (left/right / frame select clock)
// - Digital 1 -> I2S BCLK / SCLK (bit / serial clock)
// - Digital 9 -> I2S DIN / SD (data output)
// - Ground
//
// Released under a MIT license: https://opensource.org/licenses/MIT
#include "Adafruit_ZeroI2S.h"


#define SAMPLERATE_HZ 44100  // The sample rate of the audio.  Higher sample rates have better fidelity,
                             // but these tones are so simple it won't make a difference.  44.1khz is
                             // standard CD quality sound.

#define AMPLITUDE     ((1<<28)-1)   // Set the amplitude of generated waveforms.  This controls how loud
                             // the signals are, and can be any value from 0 to 2**31 - 1.  Start with
                             // a low value to prevent damaging speakers!

#define WAV_SIZE      256    // The size of each generated waveform.  The larger the size the higher
                             // quality the signal.  A size of 256 is more than enough for these simple
                             // waveforms.


// Define the frequency of music notes (from http://www.phy.mtu.edu/~suits/notefreqs.html):


#define LOW_NOTE    220.00
#define HIGH_NOTE   261.63

float scale[] = { LOW_NOTE, HIGH_NOTE };

// Store basic waveforms in memory.

int32_t sawtooth[WAV_SIZE] = {0};


// Create I2S audio transmitter object.
Adafruit_ZeroI2S i2s;


void generateSawtooth(int32_t amplitude, int32_t* buffer, uint16_t length) {
  // Generate a sawtooth signal that goes from -amplitude/2 to amplitude/2
  // and store it in the provided buffer of size length.
  float delta = float(amplitude)/float(length);
  for (int i=0; i<length; ++i) {
    buffer[i] = -(amplitude/2)+delta*i;
  }
}



void playWave(int32_t* buffer, uint16_t length, float frequency, float seconds) {
  // Play back the provided waveform buffer for the specified
  // amount of seconds.
  // First calculate how many samples need to play back to run
  // for the desired amount of seconds.
  uint32_t iterations = seconds*SAMPLERATE_HZ;
  // Then calculate the 'speed' at which we move through the wave
  // buffer based on the frequency of the tone being played.
  float delta = (frequency*length)/float(SAMPLERATE_HZ);
  // Now loop through all the samples and play them, calculating the
  // position within the wave buffer for each moment in time.
  for (uint32_t i=0; i<iterations; ++i) {
    uint16_t pos = uint32_t(i*delta) % length;
    int32_t sample = buffer[pos];
    // Duplicate the sample so it's sent to both the left and right channel.
    // It appears the order is right channel, left channel if you want to write
    // stereo sound.
    i2s.write(sample, sample);
  }
}
void generateSawtooth(int32_t amplitude, int32_t* buffer, uint16_t length) {
  // Generate a sawtooth signal that goes from -amplitude/2 to amplitude/2
  // and store it in the provided buffer of size length.
  float delta = float(amplitude)/float(length);
  for (int i=0; i<length; ++i) {
    buffer[i] = -(amplitude/2)+delta*i;
  }
}



void playWave(int32_t* buffer, uint16_t length, float frequency, float seconds) {
  // Play back the provided waveform buffer for the specified
  // amount of seconds.
  // First calculate how many samples need to play back to run
  // for the desired amount of seconds.
  uint32_t iterations = seconds*SAMPLERATE_HZ;
  // Then calculate the 'speed' at which we move through the wave
  // buffer based on the frequency of the tone being played.
  float delta = (frequency*length)/float(SAMPLERATE_HZ);
  // Now loop through all the samples and play them, calculating the
  // position within the wave buffer for each moment in time.
  for (uint32_t i=0; i<iterations; ++i) {
    uint16_t pos = uint32_t(i*delta) % length;
    int32_t sample = buffer[pos];
    // Duplicate the sample so it's sent to both the left and right channel.
    // It appears the order is right channel, left channel if you want to write
    // stereo sound.
    i2s.write(sample, sample);
  }
}

void setup() {
  // Configure serial port.
  Serial.begin(115200);
  Serial.println("Zero I2S Audio Tone Generator");

  // Initialize the I2S transmitter.
  if (!i2s.begin(I2S_32_BIT, SAMPLERATE_HZ)) {
    Serial.println("Failed to initialize I2S transmitter!");
    while (1);
  }
  i2s.enableTx();


  generateSawtooth(AMPLITUDE, sawtooth, WAV_SIZE);

}

void loop() {
 

  for (int i=0; i<sizeof(scale)/sizeof(float); ++i) {
    // Play the note for a quarter of a second.
//    playWave(sawtooth, WAV_SIZE, scale[i], 1);
    // Pause for a tenth of a second between notes.
    delay(100);
  }
  
}
