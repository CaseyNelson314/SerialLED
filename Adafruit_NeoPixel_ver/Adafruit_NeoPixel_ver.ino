#include "LedController.h"
#include "WaveGenerator.h"
#include "VariableController.h"
LedController led0(2, 20, 50);
LedController led1(3, 20, 50);
LedController led2(4, 20, 50);

WaveGenerator sin8bit(5000, 1000);

void setup() {
  Serial.begin(115200);
  led0.setRBrightness(100);
}

void loop() {
  sin8bit.Update();
  led0.flowing(500, 5, 150, 0xff00ff, 0x00ff00);
  led1.rainbow(sin8bit.GetTriangle(0, -500), 20);
  processingus("\t\n");
}
//13,100,50,1000,20:2380us
//->2092us
