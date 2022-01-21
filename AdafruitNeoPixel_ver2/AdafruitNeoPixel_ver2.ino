#define ADDRESS2 2
#define ADDRESS3 3
#define ADDRESS4 4

#define LED_NUM 20

#define BRIGHTNESS 50

#include "LedController.h"

LedController led0(ADDRESS2, LED_NUM, BRIGHTNESS);
LedController led1(ADDRESS3, LED_NUM, BRIGHTNESS);
LedController led2(ADDRESS4, LED_NUM, BRIGHTNESS);

void setup() {
  Serial.begin(115200);
}

void loop() {
  led0.flowing(300,  5, 150, 0xffffff, 0xff00ff);
  led1.stopMotion(1000);
  processingus("\t\n");
}
