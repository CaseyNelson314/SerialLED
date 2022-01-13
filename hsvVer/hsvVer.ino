#define I2C_ADDRESS 1
#define DATA_SIZE   1

#include <Wire.h>

#include "SerialLedController.h"
LedController led(13, 100, 3000, 50, 40);

//遅延約0.7ms/led(ledGroupsの大きさに依存)

int8_t buf[DATA_SIZE];

void setup() {
  delay(100);
  Wire.begin(I2C_ADDRESS);
  Wire.onReceive(receiveEvent);
  Serial.begin(115200);
}

void loop() {
  unsigned long time = micros();
  led.showWave();
  Serial.println(micros() - time);
}

void receiveEvent() {
  while (Wire.available())
    for (int i = 0; i < DATA_SIZE; i++)
      buf[i] = Wire.read();
}
