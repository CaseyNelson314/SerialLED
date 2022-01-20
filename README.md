# SerialLED
Serial LED control

- millis関数を基に出力しているので遅延がある場合でも周期道理出力できます

- なるべく処理速度を上げています(20LED,rainbow関数使用時で1200us程度)

# Constructor

オブジェクトを複数生成することでパラレル出力できます

```
LedController leds(ADDRESS, LED_NUM, BRIGHTNESS);
LedController leds(信号ピン, LED数, 最高輝度);
```
