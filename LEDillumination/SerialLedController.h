#pragma once
#include <Adafruit_NeoPixel.h>

class LedController {

  private:
    Adafruit_NeoPixel* pixel;
    int8_t pin;
    int ledNum;
    int cycle;
    int8_t maxPower;
    int ledGroups;

    unsigned long time, lastTime;
    bool direction = 1;

  public:
    LedController(int8_t pin, int ledNum, int cycle, int8_t maxPower, int ledGroups = 1):
      pin(pin), ledNum(ledNum), cycle(cycle), maxPower(maxPower), ledGroups(ledGroups) {
      pixel = new Adafruit_NeoPixel(ledNum, pin);
      pixel->begin();
    }
    ~LedController() {
      pixel->~Adafruit_NeoPixel();
    }

    /*1つのLEDの色相が1周する周期(ms)*/
    void setCycle(int cycle) {
      this->cycle = cycle;
    }

    /*色相が1周するLEDのまとまりの合計*/
    void setGroups(int ledGroups) {
      this->ledGroups = ledGroups;
    }

    /*最高輝度(0~255)*/
    void setBrightness(int8_t maxPower) {
      this->maxPower = maxPower;
    }

    /*流れる方向(1:データ転送方向と同じ方向 0:逆方向)*/
    void setDirection(bool direction) {
      this->direction = direction;
    }




    /*出力*/
    void showWave() {
      /*error*/
      if (!cycle || !ledGroups)return;

      if ((time = millis()) - lastTime > cycle)lastTime = time;
      float referenceX = (time - lastTime) * 2.0 / cycle;

      for (int j = 0; j < ledGroups; j++) {
        int8_t rgb[3];  //RGBデータ格納用
        float x = referenceX - (direction ? 1 : -1) * (j * 2.0 / ledGroups); //sin波出力用のx軸&ledごとに位相をずらす
        for (int i = 0; i < 3; i++)
          rgb[i] = maxPower * (sin(fmod(x - i * 2 / 3.0, 2) * PI) + 1) / 2;  //rgbで位相をずらす
        for (int i = 0; i < ledNum / ledGroups; i++)
          pixel->setPixelColor(j + ledGroups * i, rgb[0], rgb[1], rgb[2]);
      }
      pixel->show();
    }

    /*LEDすべて同じ色で出力*/
    void showSameWave() {
      /*error*/
      if (!cycle)return;

      if ((time = millis()) - lastTime > cycle)lastTime = time;

      float x = (time - lastTime) * 2.0 / cycle; //sin波出力用のx軸
      int8_t rgb[3];
      for (int i = 0; i < 3; i++)
        rgb[i] = maxPower * (sin((x - i * 2 / 3.0) * PI) + 1) / 2;
      for (int i = 0; i < ledNum; i++)
        pixel->setPixelColor(i, rgb[0], rgb[1], rgb[2]);
      pixel->show();
    }

    /*全消去*/
    void clear() {
      pixel->clear();
      pixel->show();
    }

};
