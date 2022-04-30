#include "SerialLedController.h"

SerialLedController pixel(4, 100);

void processingus(String code = "");

void setup() {
  Serial.begin(115200);
  pixel.setRBrightness(200);
  pixel.setGBrightness(200);
}

void loop() {
  pixel.showRainbow(3000, 100);
  processingus("\n");
}

void processingus(String code) {
  static uint32_t holdTime;
  Serial.print(micros() - holdTime);
  Serial.print(code);
  holdTime = micros();
}
