#include "AudioTools.h"
#include "BluetoothA2DPSink.h"
#include <ESP_I2S.h>
#include <math.h>

#define I2S_BCLK  26
#define I2S_LRCLK 25
#define I2S_DOUT  22

#define SAMPLE_RATE 48000
#define TONE_FREQ   1000
#define AMPLITUDE   12000

I2SClass I2S;

int16_t audioBuffer[48 * 2];


I2SStream out;
BluetoothA2DPSink a2dp_sink(out);

void setup() {
  Serial.begin(115200);
  a2dp_sink.start("MyMusic_Test"); 
  
  // Generate 1 kHz sine wave
  for (int i = 0; i < 48; i++) {
    float sample = sinf(2.0f * PI * i / 48.0f);
    int16_t value = sample * AMPLITUDE;

    audioBuffer[i * 2]     = value; // Left
    audioBuffer[i * 2 + 1] = value; // Right
  }

  // Configure I2S pins
  I2S.setPins(I2S_BCLK, I2S_LRCLK, I2S_DOUT);

  // Start I2S
  if (!I2S.begin(
      I2S_MODE_STD,
      SAMPLE_RATE,
      I2S_DATA_BIT_WIDTH_16BIT,
      I2S_SLOT_MODE_STEREO)) {
    Serial.println("I2S initialization failed!");
    while (true) delay(1000);
  }

  Serial.println("I2S running: 1 kHz sine wave");
  
}


void loop() {
    I2S.write((uint8_t*)audioBuffer,
            sizeof(audioBuffer));
}
