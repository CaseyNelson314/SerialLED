#pragma once
#include <Adafruit_NeoPixel.h>

class LedController {

  private:
    Adafruit_NeoPixel* pixel;
    byte pin;              //pin
    int ledNum;            //LED個数
    byte saturation = 200; //色彩

    bool direction = 1;    //流れる方向
    unsigned long time, holdTime;

    uint32_t hsv(byte h, byte s); //HSV>RGB v:setBrightnessで変更

  public:
    LedController(byte pin, int ledNum, byte maxPower): pin(pin), ledNum(ledNum) {
      pixel = new Adafruit_NeoPixel(ledNum, pin);
      pixel->setBrightness(maxPower);
      pixel->begin();
    }
    ~LedController() {
      pixel->~Adafruit_NeoPixel();
    }

    /*最高輝度(0~255)*/
    void setBrightness(byte maxPower) {
      pixel->setBrightness(maxPower);
    }

    /*彩度(0~255)*/
    void setSaturation(byte saturation) {
      this->saturation = saturation;
    }

    /*流れる方向(1:データ転送方向と同じ方向 0:逆方向)*/
    void setDirection(bool direction) {
      this->direction = direction;
    }



    /*出力*/
    void showWave(int cycle, int ledGroups) {
      /*error*/
      if (!ledGroups)return;                                                //除算に使うので0をはじく

      if ((time = millis()) - holdTime > cycle)holdTime = time;
      byte referenceX = (time - holdTime) * 255 / cycle;                    //0から255までcycleに応じて上昇

      int ledPerGroups = ledNum / ledGroups;                                //同じ光り方のLEDがいくつあるか

      for (int j = 0; j < ledGroups; j++) {                                 //同じ光り方をするLEDの処理一括化
        if (j >= ledNum)break;                                              //led数よりledGroupsが多い場合は処理不要 byteがたに入れることで負の数になった時に補数を取る

        byte x = referenceX - (direction ? 1 : -1) * (j * 255 / ledGroups); //LEDごとに色相をずらし流れる様に directionで加算or減算設定
        uint32_t rgb = hsv(x, saturation);                                  //色相環からRGB取得

        for (int i = 0; i <= ledPerGroups; i++)                             //同じ光り方のLEDのグループ数ループ 無い場合は1ループ
          if (j + ledGroups * i < ledNum)
            pixel->setPixelColor(j + ledGroups * i, rgb);
          else
            break;
      }

      pixel->show();
    }

    /*LEDすべて同じ色で出力*/
    void showSameWave(int cycle) {
      if ((time = millis()) - holdTime > cycle)holdTime = time;
      byte x = (time - holdTime) * 255 / cycle;
      uint32_t rgb = hsv(x, saturation);
      for (int i = 0; i < ledNum; i++)
        pixel->setPixelColor(i, rgb);
      pixel->show();
    }
    
    /*1loopの処理時間を表示*/
    void processingTime(char ln[] = 0) {
      static uint32_t lastTime;
      Serial.print(micros() - lastTime);
      Serial.print(ln);
      lastTime = micros();
    }

    /*全消去*/
    void clear() {
      pixel->clear();
      pixel->show();
    }

};

uint32_t LedController::hsv(byte h, byte s) {
  byte r, g, b;
  byte f = 0xff & (h * 6);
  byte p = 255 - s;
  byte q = 255 - (f * s >> 8); // ÷256
  byte t = 255 - ((255 - f) * s >> 8);
  switch (h * 6 >> 8) {
    case 0: r = 255, g = t,   b = p;   break;
    case 1: r = q,   g = 255, b = p;   break;
    case 2: r = p,   g = 255, b = t;   break;
    case 3: r = p,   g = q,   b = 255; break;
    case 4: r = t,   g = p,   b = 255; break;
    case 5: r = 255, g = p,   b = q;   break;
  }
  return (uint32_t)r << 16 | (uint32_t)g << 8 | b;
}
