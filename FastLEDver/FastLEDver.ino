#include "LedController.h"
LedController led0(10, 100, 50);
LedController led1(13, 100, 50);

void setup() {
  Serial.begin(115200);
  led1.setRBrightness(0);//g
  led1.setGBrightness(50);
  led1.setBBrightness(50);
}

void loop() {
//  led0.rainbow(1000, 50);
  led1.rainbow(2000, 50);
  FastLED.show();
  led1.processingus("\t\n");
}
