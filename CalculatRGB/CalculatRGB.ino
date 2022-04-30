#include "LedController.h"
#include <Adafruit_NeoPixel.h>

LedController hoge(20);
Adafruit_NeoPixel pixel(20, 4);

void setup() {
  Serial.begin(115200);
  pixel.begin();
}

void loop() {
  byte rgb[20][3];
  hoge.calculatRainbow(1000, 20, rgb);
  for (int i = 0; i < 20; i++)
    pixel.setPixelColor(i, rgb[i][0], rgb[i][1], rgb[i][2]);
  pixel.show();
  processingus("\n");
}

void processingus(String code) {
  static uint32_t holdTime;
  Serial.print(micros() - holdTime);
  Serial.print(code);
  holdTime = micros();
}
