/**
       @file   LedController.h
       @brief  RGB値取得用クラス
       @author CaseyNelson
       @date   2022/4/30
*/

#pragma once

class LedController {

    uint16_t ledNum;
    uint8_t brightness;
    uint8_t rBrightness, gBrightness, bBrightness;
    uint32_t holdTime;

  public:

    LedController(uint16_t ledNum, uint8_t brightness = 50);
    LedController(void);
    ~LedController();

    void setRBrightness(uint8_t brightness);
    void setGBrightness(uint8_t brightness);
    void setBBrightness(uint8_t brightness);

    void calculatRainbow(int16_t cycle, uint8_t ledsOfHue, uint8_t returnArray[][3]);
    void calculatRainbow(int16_t cycle, uint8_t ledsOfHue, uint32_t returnArray[]);

  private:

    uint32_t hsv2rgb(uint8_t h, uint8_t s);
    void transformBrightness(uint32_t &rgb);

};


/************************************
   constructor destructor
************************************/
LedController::LedController(uint16_t ledNum, uint8_t brightness):
  ledNum(ledNum), brightness(brightness), rBrightness(255), gBrightness(255), bBrightness(255) {
};
LedController::~LedController() {};


/************************************
   Set rgb brightness
************************************/
void LedController::setRBrightness(uint8_t brightness) {
  rBrightness = brightness;
}
void LedController::setGBrightness(uint8_t brightness) {
  gBrightness = brightness;
}
void LedController::setBBrightness(uint8_t brightness) {
  bBrightness = brightness;
}

/************************************
   Outout rainbow
   ledNum : Adafruit_NeoPixel.h
************************************/
void LedController::calculatRainbow(int16_t cycle, uint8_t ledsOfHue, uint8_t returnArray[][3]) {

  if (ledsOfHue == 0)return;

  int8_t dir = (cycle >= 0) ? 1 : -1;
  cycle = abs(cycle);

  uint32_t time = millis();
  if (time - holdTime > (uint32_t)cycle)holdTime = time;

  //軽量化のためfor文外に記述
  uint8_t referenceHeu = (time - holdTime) * 255 / cycle; //先頭LEDの色相生成
  uint8_t sameColorLeds = ledNum / ledsOfHue;                //同色のLEDの数を計算
  int16_t hueShift = 256.0 / ledsOfHue * dir;                //色相のずれる大きさを計算

  for (uint16_t j = 0; j < ledsOfHue; j++) {                 //先頭LEDの色相を基に各LEDの出力値計算

    if (j >= ledNum)break;

    uint8_t heu = referenceHeu - j * hueShift;            //色相値計算&脳筋キャスト
    uint32_t rgb = hsv2rgb(heu, 200);                     //RGB値に変換
    transformBrightness(rgb);

    for (uint8_t i = 0; i <= sameColorLeds; i++) {        //同色LEDの処理まとめる
      uint16_t ledIndex = j + ledsOfHue * i;
      if (ledIndex < ledNum) {
        returnArray[ledIndex][0] = uint8_t(rgb >> 16);
        returnArray[ledIndex][1] = uint8_t(rgb >> 8);
        returnArray[ledIndex][2] = uint8_t(rgb);
      }
      else
        break;
    }

  }
}

/************************************
   Overload
************************************/
void LedController::calculatRainbow(int16_t cycle, uint8_t ledsOfHue, uint32_t returnArray[]) {

  if (ledsOfHue == 0)return;

  int8_t dir = (cycle >= 0) ? 1 : -1;
  cycle = abs(cycle);

  uint32_t time = millis();
  if (time - holdTime > (uint32_t)cycle)holdTime = time;

  //軽量化のためfor文外に記述
  uint8_t referenceHeu = (time - holdTime) * 255 / cycle; //先頭LEDの色相生成
  uint8_t sameColorLeds = ledNum / ledsOfHue;             //同色のLEDの数を計算
  int16_t hueShift = 256.0 / ledsOfHue * dir;             //色相のずれる大きさを計算

  for (uint16_t j = 0; j < ledsOfHue; j++) {              //先頭LEDの色相を基に各LEDの出力値計算

    if (j >= ledNum)break;

    uint8_t heu = referenceHeu - j * hueShift;            //色相値計算&脳筋キャスト
    uint32_t rgb = hsv2rgb(heu, 200);                     //RGB値に変換
    transformBrightness(rgb);

    for (uint8_t i = 0; i <= sameColorLeds; i++) {        //同色LEDの処理まとめる
      uint16_t ledIndex = j + ledsOfHue * i;
      if (ledIndex < ledNum)
        returnArray[ledIndex] = rgb;
      else
        break;
    }

  }
}

/************************************************
   hsv to rgb (Not v)
   referenced https://github.com/Aeva/lowfi_hsv
************************************************/
uint32_t LedController::hsv2rgb(uint8_t h, uint8_t s) {
  uint8_t r, g, b;
  uint8_t f = h * 6; //脳筋キャスト
  uint8_t p = 255 - s;
  uint8_t q = 255 - (f * s >> 8); // >>8 : ÷256
  uint8_t t = 255 - ((255 - f) * s >> 8);
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

/************************************
   Transform brightness
************************************/
void LedController::transformBrightness(uint32_t &rgb) {
  uint8_t r = uint8_t(uint8_t(rgb >> 16) * rBrightness >> 8) * brightness >> 8;
  uint8_t g = uint8_t(uint8_t(rgb >> 8)  * gBrightness >> 8) * brightness >> 8;
  uint8_t b = uint8_t(uint8_t(rgb)       * bBrightness >> 8) * brightness >> 8;
  rgb = (uint32_t)r << 16 | (uint16_t)g << 8 | b;
}
