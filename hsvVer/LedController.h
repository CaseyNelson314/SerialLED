#pragma once
#include <Adafruit_NeoPixel.h>

class LedController {

  private:
    Adafruit_NeoPixel* pixel;
    byte pin;              //pin
    int  ledNum;            //LED個数

    unsigned long time, holdTime;


  public:
    LedController(byte pin, int ledNum, byte brightness): pin(pin), ledNum(ledNum) {
      pixel = new Adafruit_NeoPixel(ledNum, pin);
      pixel->setBrightness(brightness);
      pixel->begin();
    }
    ~LedController(void) {
      pixel->~Adafruit_NeoPixel();
    }

    /*最高輝度*/
    void updateBrightness(byte brightness) {
      pixel->setBrightness(brightness);
    }


    /*色相移動 発光パターン非同期 [色相が1周する周期(ms),色相が1周するLEDの数,流れる方向,彩度]*/
    void showRainbow(int cycle, int ledGroups, bool direction = 1, byte saturation = 180);

    /*色相移動 発光パターン同期 [色相が1周する周期(ms),彩度]*/
    void showSameRainbow(int cycle, byte saturation = 200);

    /*移動 [先端から終端まで流れる時間(ms),LEDの色(RGB下位8+8+8bit),流れるLEDのまとまり]*/
    void showFlowing(int cycle, uint32_t color, byte fillLedNum);

    void fill(byte r, byte g, byte b);
    void fill(uint32_t rgb);
    
    uint32_t hsv(byte h, byte s); //HSV>RGB 軽量化のためVなし


    /*処理時間表示*/
    void processingus(char ln[] = NULL) {
      static uint32_t lastTime;
      Serial.print(micros() - lastTime);
      Serial.print(ln);
      lastTime = micros();
    }

    /*全消去*/
    void clear(void) {
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

void LedController::showRainbow(int cycle, int ledGroups, bool direction = 1, byte saturation = 180) {
  /*error*/
  if (ledGroups == 0)return;                                                //除算に使うので0をはじく

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

void LedController::showSameRainbow(int cycle, byte saturation = 200) {
  if (ledGroups == 0)return;
  if ((time = millis()) - holdTime > cycle)holdTime = time;
  byte x = (time - holdTime) * 255 / cycle;
  uint32_t rgb = hsv(x, saturation);
  for (int i = 0; i < ledNum; i++)
    pixel->setPixelColor(i, rgb);
  pixel->show();
}

void LedController::showFlowing(int cycle, uint32_t color, byte fillLedNum) {
  if (ledGroups == 0)return;
  if ((time = millis()) - holdTime > cycle)holdTime = time;
  int index = (time - holdTime) * (ledNum + fillLedNum) / cycle;
  pixel->setPixelColor(index - 1 - fillLedNum, 0);
  for (int i = 0; i < fillLedNum; i++)
    if (index - i >= 0)
      pixel->setPixelColor(index - i, color);
  pixel->show();
}

void LedController::fill(byte r, byte g, byte b) {
  for (int i = 0; i < ledNum; i++)
    pixel->setPixelColor(i, r, g, b);
  pixel->show();
}
void LedController::fill(uint32_t rgb) {
  for (int i = 0; i < ledNum; i++)
    pixel->setPixelColor(i, rgb);
  pixel->show();
}
