#include "LedController.h"
LedController led0(2, 40, 50);
LedController led1(3, 20, 50);
LedController led2(4, 20, 50);

void setup() {
  Serial.begin(115200);
  led0.setRBrightness(100);
}

void loop() {
  led0.rainbow(500, 40, 0);
  led1.rainbow(1000, 40);
  led2.rainbow(2000, 20);
  processingus("\t\n");
}
//13,100,50,1000,20:2380us
//->2092us
