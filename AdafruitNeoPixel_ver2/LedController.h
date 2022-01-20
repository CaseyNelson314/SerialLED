/*
        @file   LedController.h
        @brief  Serial led controller
        @author CaseyNelson314
        @date   2022/01/20
*/

#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class LedController {

  private:
    Adafruit_NeoPixel* pixel;
    byte rBrightness = 255, gBrightness = 255, bBrightness = 255;
    int  ledNum; //LED個数
    uint32_t holdTime, time;

  public:
    LedController(byte pin, int ledNum, byte brightness): ledNum(ledNum) {
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

    /*色相移動 発光パターン非同期 [色相が1周する周期(ms),色相が1周するLEDの数,彩度]*/
    void rainbow(int cycle, int ledGroups, byte saturation = 200); //発光パターン非同期
    void sameRainbow(int cycle, byte saturation = 200);                                //発光パターン同期

    /*発光移動 [先端から終端まで流れる時間(ms),LEDの色(RGB下位8+8+8bit),流れるLEDのまとまり]*/
    void flowing(int cycle, int ledGroups, byte duty = 100, uint32_t onColor = 0xfffff, uint32_t offColor = 0x000000);

    /*フラッシュ[]*/
    void stopMotion(int cycle, byte whenFlash = 50, byte flashTime = 30, uint32_t colora = 0xff0000, uint32_t colorb = 0xff5100);

    /*ランダム点灯*/
    void randomPixel(int speed, uint32_t colora = 0xffffff, uint32_t colorb = 0x000000);

    /*全LED点灯*/
    void fill(byte r, byte g, byte b); //R,G,B
    void fill(uint32_t rgb);           //RGB下位8+8+8bit

    /*HSV>RGB変換[軽量化のためVなし]*/
    uint32_t hsv(byte h, byte s);
    void transformBrightness(uint32_t &data);

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
  byte q = 255 - (f * s >> 8);
  byte t = 255 - ((255 - f) * s >> 8);
  switch (h * 6 >> 8) {
    case 0: r = 255, g = t,   b = p;   break;
    case 1: r = q,   g = 255, b = p;   break;
    case 2: r = p,   g = 255, b = t;   break;
    case 3: r = p,   g = q,   b = 255; break;
    case 4: r = t,   g = p,   b = 255; break;
    case 5: r = 255, g = p,   b = q;   break;
  }
  return (uint32_t)r << 16 | (uint16_t)g << 8 | b;
}

void LedController::transformBrightness(uint32_t &rgb) { //展開後,調整,圧縮 誤差±1
  if (rBrightness == 0xff && gBrightness == 0xff && bBrightness == 0xff)return;
  byte r = (byte)(rgb >> 16) * rBrightness >> 8;
  byte g = (byte)(rgb >>  8) * gBrightness >> 8;
  byte b = (byte)(rgb      ) * bBrightness >> 8;
  rgb = (uint32_t)r << 16 | (uint16_t)g << 8 | b;
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
void LedController::rainbow(int cycle, int ledGroups, byte saturation = 200) {
  if (ledGroups == 0 || ledGroups == 0)return;                          //除算に使うので0をはじく

  int cycleSign = (cycle > 0) ? 1 : -1;

  cycle = abs(cycle);
  if ((time = millis()) - holdTime > cycle)holdTime = time;

  byte referenceX = (time - holdTime) * 256 / cycle;                    //0から255までcycleに応じて上昇
  byte ledPerGroups = ledNum / ledGroups;                               //同じ光り方のLEDがいくつあるか

  for (uint16_t j = 0; j < ledGroups; j++) {                            //同じ光り方をするLEDの処理一括化
    if (j >= ledNum)break;                                              //led数よりledGroupsが多い場合は処理不要 byte型によって負の数になった時にオーバーフロー

    byte x = referenceX -  cycleSign * (j * 256 / ledGroups);           //LEDごとに色相をずらし流れる様に directionで加算or減算設定

    uint32_t rgb = hsv(x, saturation);                                  //色相環からRGB取得
    transformBrightness(rgb);                                           //最高輝度を変更

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

  if ((time = millis()) - holdTime > cycle)holdTime = time;
  byte x = (time - holdTime) * 255 / cycle;
  
  uint32_t rgb = hsv(x, saturation);
  transformBrightness(rgb);
  
  for (int i = 0; i < ledNum; i++)
    pixel->setPixelColor(i, rgb);
  pixel->show();
}

void LedController::flowing(int cycle, int ledGroups, byte duty = 100, uint32_t onColor = 0xfffff, uint32_t offColor = 0x000000) {
  if (cycle == 0 || ledGroups == 0)return;

  int cycleSign = (cycle > 0) ? 1 : -1;
  cycle = abs(cycle);

  byte ledPerGroups = ledNum / ledGroups;
  if ((time = millis()) - holdTime > cycle)holdTime = time;
  byte x = (time - holdTime) * 255 / cycle;
  byte xSum = 255 / ledGroups;

  uint32_t outputRgb;
  transformBrightness(onColor);
  transformBrightness(offColor);
  
  for (int j = 0; j < ledGroups; j++) {
    if ((x -= cycleSign * xSum) < duty)
      outputRgb = onColor;
    else
      outputRgb = offColor;

    for (byte i = 0; i < ledPerGroups; i++)
      if (j + ledGroups * i < ledNum)
        pixel->setPixelColor(j + ledGroups * i, outputRgb);
      else
        break;
  }
  pixel->show();
}

void LedController::randomPixel(int speed, uint32_t colora = 0xffffff, uint32_t colorb = 0x000000) {
  if ((time = millis()) - holdTime > speed) {
    holdTime = time;
    
    int randomLedIndex = random(0, ledNum);
    
    transformBrightness(colora);
    transformBrightness(colorb);
    
    for (int i = 0; i < ledNum; i++)
      if (i == randomLedIndex)
        pixel->setPixelColor(i, colora);
      else
        pixel->setPixelColor(i, colorb);
    pixel->show();
  }
}

void LedController::stopMotion(int cycle, byte whenFlash = 50, byte flashTime = 30, uint32_t colora = 0xff0000, uint32_t colorb = 0xff5100) {
  if (cycle == 0)return;
  if (cycle / 2 - whenFlash < 0)return;

  if ((time = millis()) - holdTime > cycle)holdTime = time;
  int toMacro = (time - holdTime) - cycle / 2;
  int y = cycle / 2 - abs(toMacro);
  
  transformBrightness(colora);
  transformBrightness(colorb);

  if (cycle / 2 - whenFlash + flashTime > y && y > cycle / 2 - whenFlash)
    for (int i = 0; i < ledNum; i++)
      pixel->setPixelColor(i, colora);
  else
    for (int i = 0; i < ledNum; i++)
      pixel->setPixelColor(i, colorb);
      
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
void LedController::clear(void) {
  pixel->clear();
  pixel->show();
}


void processingus(char ln[] = NULL) {
  static uint32_t lastTime;
  Serial.print(micros() - lastTime);
  Serial.print(ln);
  lastTime = micros();
}
