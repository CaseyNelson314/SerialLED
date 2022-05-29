#include "SerialLedController.h"

SerialLedController pixel(4, 20);

void processingus(String code = "");

void setup() {
  Serial.begin(115200);
  pixel.setBBrightness(0);
  pixel.setGBrightness(0);
}

void loop() {
  pixel.showRainbow(1000, 50);
  processingus("\n");
}

void processingus(const String &code) {
  static uint32_t holdTime;
  Serial.print(micros() - holdTime);
  Serial.print(code);
  holdTime = micros();
}
