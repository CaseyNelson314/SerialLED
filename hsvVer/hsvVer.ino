#define I2C_ADDRESS 1
#define DATA_SIZE   1

#include <Wire.h>

#include "SerialLedController.h"
LedController led(13, 300, 30);

int8_t buf[DATA_SIZE];

void setup() {
  delay(100);
  Wire.begin(I2C_ADDRESS);
  Wire.onReceive(receiveEvent);
  Serial.begin(115200);
}

void loop() {
  led.showWave(1000, 10);
  led.processingTime("\n");
}

void receiveEvent() {
  while (Wire.available())
    for (int i = 0; i < DATA_SIZE; i++)
      buf[i] = Wire.read();
}
