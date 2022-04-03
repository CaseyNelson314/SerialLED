# SerialLED
Serial LED control

- Output is based on the millis function, so even if there is a delay, the output is periodic.

- The processing is light because it is designed to be used for 8-bit microcontrollers

  (Arduino nano, 20 LEDs, 1200μs when using rainbow function).

# Functions
### Constructor

`LedController leds(addressPin, ledNum, brightness);`

### Setter
- Overall brightness setting

  `updateBrightness(byte brightness);`

- Brightness setting for each color

  `setRBrightness(byte brightness);`

  `setGBrightness(byte brightness);`

  `setBBrightness(byte brightness);`


### Output
- Rainbow

  `rainbow(int cycle, int ledGroups, byte saturation = 200);`

- Same rainbow

  `sameRainbow(int cycle, byte saturation = 200);`

- Flowing

  `void flowing(cycle, ledGroups, duty = 100, onColor = 0xfffff, offColor = 0x000000);`

- StopMotion

  `stopMotion(cycle, whenFlash = 50, flashTime = 30, colora = 0xff0000, colorb = 0xff5100);`

- Fill

  `fill(r, g, b);``fill(rgb);`

- Clear

  `clear();`

### Else
- HSV->RGB(no Value)

  `hsv(h, s);`

- Display of processing speed

  `processingus(char ln[] = NULL);`

