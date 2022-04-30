# SerialLED
## Serial LED controler
  
- Adafruit_NeoPixelライブラリが必要です.

- 出力はmillis関数に基づくため、遅延があっても周期的に出力されます.

- 8bitマイコンでの使用を前提に最適化しています.

## Functions
### Constructor

  `SerialLedController(pin, ledNum, brightness = 50);` data pin , led num , max brightness

### Setter
- all brightness setting

  `setBrightness(brightness);`

- Brightness setting for each color

  `setRBrightness(brightness);`

  `setGBrightness(brightness);`

  `setBBrightness(brightness);`


### Output

- Rainbow

  `showRainbow(cycle, ledsOfHue);` cycle , how many LEDs in one hue ring

## Else

Adafruit_NeoPixelクラスを継承しているため、内部の関数も使用できます
