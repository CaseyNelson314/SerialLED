#include "LedController.h"
LedController led0(2, 100, 50);
LedController led1(3, 100, 50);
LedController led2(4, 100, 50);

void setup() {
  Serial.begin(115200);
}

void loop() {
  led0.rainbow(1000, 50);
  led1.rainbow(1000, 50);
  led2.rainbow(2000, 50);
  FastLED.show();
  led1.processingus("\t\n");
}
