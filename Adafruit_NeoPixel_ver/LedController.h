#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class LedController {

  private:
    Adafruit_NeoPixel* pixel;
    byte pin;    //pin
    int  ledNum; //LED個数
    byte rBrightness = 255, gBrightness = 255, bBrightness = 255;

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
    void setRBrightness(byte brightness);
    void setGBrightness(byte brightness);
    void setBBrightness(byte brightness);
    void updateBrightness(byte brightness);

    /*色相移動 発光パターン非同期 [色相が1周する周期(ms),色相が1周するLEDの数,流れる方向,彩度]*/
    void rainbow(int cycle, int ledGroups, bool direction = 1, byte saturation = 200); //発光パターン非同期
    void sameRainbow(int cycle, byte saturation = 200);                                //発光パターン同期

    /*発光移動 [先端から終端まで流れる時間(ms),LEDの色(RGB下位8+8+8bit),流れるLEDのまとまり]*/
    void flowing(int cycle, uint32_t color, byte fillLedNum);

    /*フラッシュ[]*/
    void stopMotion(int cycle, byte whenFlash = 50, byte flashTime = 30, uint32_t colora = 0xff0000, uint32_t colorb = 0xff5100);

    /*全LED点灯*/
    void fill(byte r, byte g, byte b); //R,G,B
    void fill(uint32_t rgb);           //RGB下位8+8+8bit

    /*HSV>RGB変換[軽量化のためVなし]*/
    uint32_t hsv(byte h, byte s);
    uint32_t transformBrightness(uint32_t &data);

    /*処理時間表示*/
    void processingus(char ln[] = NULL);

    /*全消去*/
    void clear(void);

};


/*internal functions*/
uint32_t LedController::hsv(byte h, byte s) {
  byte r, g, b;
  byte f = 0xff & (h * 6);
  byte p = 255 - s;
  byte q = 255 - (f * s >> 8); //÷256
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

uint32_t LedController::transformBrightness(uint32_t &rgb) { //展開後,圧縮,結合 誤差±1
  if (rBrightness == 255 && gBrightness == 255 && bBrightness == 255)return data;
  byte r = (((0xff & (rgb >> 16))) * rBrightness) >> 8;
  byte g = (((0xff & (rgb >>  8))) * gBrightness) >> 8;
  byte b = ( (0xff &  rgb       )  * bBrightness) >> 8;
  data = (uint32_t)r << 16 | (uint32_t)g << 8 | b;
}


/*setter functions*/
void LedController::updateBrightness(byte brightness) {
  pixel->setBrightness(brightness);
}
void LedController::setRBrightness(byte brightness) {
  rBrightness = brightness;
}
void LedController::setGBrightness(byte brightness) {
  gBrightness = brightness;
}
void LedController::setBBrightness(byte brightness) {
  bBrightness = brightness;
}


/*output functions*/
void LedController::rainbow(int cycle, int ledGroups, bool direction = 1, byte saturation = 200) {
  if (ledGroups == 0)return;                                            //除算に使うので0をはじく

  static uint32_t holdTime, time;
  if ((time = millis()) - holdTime > cycle)holdTime = time;

  byte referenceX = (time - holdTime) * 256 / cycle;                    //0から255までcycleに応じて上昇
  byte ledPerGroups = ledNum / ledGroups;                               //同じ光り方のLEDがいくつあるか

  for (uint16_t j = 0; j < ledGroups; j++) {                            //同じ光り方をするLEDの処理一括化
    if (j >= ledNum)break;                                              //led数よりledGroupsが多い場合は処理不要 byte型によって負の数になった時にオーバーフロー

    byte x = referenceX - (direction ? 1 : -1) * (j * 256 / ledGroups); //LEDごとに色相をずらし流れる様に directionで加算or減算設定
    uint32_t rgb = hsv(x, saturation);                                  //色相環からRGB取得
     transformBrightness(rgb);                                     //最高輝度を変更

    for (byte i = 0; i <= ledPerGroups; i++)                            //同じ光り方のLEDのグループ数ループ 無い場合は1ループ
      if (j + ledGroups * i < ledNum)
        pixel->setPixelColor(j + ledGroups * i, rgb);
      else
        break;
  }
  pixel->show();
}

void LedController::sameRainbow(int cycle, byte saturation = 200) {
  if (cycle == 0)return;
  static uint32_t holdTime, time;
  if ((time = millis()) - holdTime > cycle)holdTime = time;
  byte x = (time - holdTime) * 255 / cycle;
  uint32_t rgb = hsv(x, saturation);
   transformBrightness(rgb);
  for (int i = 0; i < ledNum; i++)
    pixel->setPixelColor(i, rgb);
  pixel->show();
}

void LedController::flowing(int cycle, uint32_t color, byte fillLedNum) {
  if (cycle == 0)return;
  static uint32_t holdTime, time;
  if ((time = millis()) - holdTime > cycle)holdTime = time;
  int index = (time - holdTime) * (ledNum + fillLedNum) / cycle;
  pixel->setPixelColor(index - 1 - fillLedNum, 0);
  transformBrightness(color);
  for (int i = 0; i < fillLedNum; i++)
    if (index - i >= 0)
      pixel->setPixelColor(index - i, color);
  pixel->show();
}

void LedController::stopMotion(int cycle, byte whenFlash = 50, byte flashTime = 30, uint32_t colora = 0xff0000, uint32_t colorb = 0xff5100) {
  if (cycle == 0)return;
  if (cycle / 2 - whenFlash < 0)return;
  static uint32_t holdTime, time;
  if ((time = millis()) - holdTime > cycle)holdTime = time;
  int toMacro = (time - holdTime) - cycle / 2;
  int t = cycle / 2 - abs(toMacro);
  if (cycle / 2 - whenFlash + flashTime > t && t > cycle / 2 - whenFlash) {
    transformBrightness(colora);
    for (int i = 0; i < ledNum; i++)
      pixel->setPixelColor(i, colora);
  } else {
    transformBrightness(colorb);
    for (int i = 0; i < ledNum; i++)
      pixel->setPixelColor(i, colorb);
  }
  pixel->show();
}

void LedController::fill(byte r, byte g, byte b) {
  uint32_t rgb = (uint32_t)r << 16 | (uint32_t)g << 8 | b;
   transformBrightness(rgb);
  for (int i = 0; i < ledNum; i++)
    pixel->setPixelColor(i, rgb);
  pixel->show();
}
void LedController::fill(uint32_t rgb) {
   transformBrightness(rgb);
  for (int i = 0; i < ledNum; i++)
    pixel->setPixelColor(i, rgb);
  pixel->show();
}


/*option functions*/
void LedController::processingus(char ln[] = NULL) {
  static uint32_t lastTime;
  Serial.print(micros() - lastTime);
  Serial.print(ln);
  lastTime = micros();
}

void LedController::clear(void) {
  pixel->clear();
  pixel->show();
}
