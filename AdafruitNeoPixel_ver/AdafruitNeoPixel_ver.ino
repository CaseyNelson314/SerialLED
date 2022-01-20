#include "LedController.h"
LedController led0(2, 20, 50);
LedController led1(3, 20, 50);
LedController led2(4, 20, 50);

void setup() {
  Serial.begin(115200);
}

void loop() {
  led0.flowing(500, 5, 150, 0xff00ff, 0x00ff00);
  led1.rainbow(1000, 20);
  processingus("\t\n");
}
