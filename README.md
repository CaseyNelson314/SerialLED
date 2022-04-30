# SerialLED
Serial LED control

- Output is based on the millis function, so even if there is a delay, the output is periodic.

- The processing is light because it is designed to be used for 8-bit microcontrollers

  (Arduino nano, 20 LEDs, 1000μs when using rainbow function).

# Functions
### Constructor

  `SerialLedController(pin, ledNum, brightness = 50);` data pin , led num , max brightness

### Setter
- Overall brightness setting

  `setBrightness(brightness);`

- Brightness setting for each color

  `setRBrightness(brightness);`

  `setGBrightness(brightness);`

  `setBBrightness(brightness);`


### Output

- Rainbow

  `showRainbow(cycle, ledsOfHue);` cycle , how many LEDs in one hue ring

