#include "LedController.h"
LedController led(13, 100, 50);

void setup() {
  Serial.begin(115200);
  led.setRBrightness(30);
  led.setGBrightness(30);
  led.setBBrightness(30);
}

void loop() {
  led.rainbow(1000, 20);
  led.processingus("\t\n");
}
//13,100,50,1000,20:2380ms
