# SerialLED
Serial LED control

- millis関数を基に出力しているので遅延がある場合でも周期通り出力できます

- 8bitマイコン用で使う想定なので、処理を軽くしてます(Arduino nano, 20LED, rainbow関数使用時で約1200μs)

# Constructor

オブジェクトを複数生成することでパラレル出力できます

```
LedController leds(ADDRESS, LED_NUM, BRIGHTNESS);
LedController leds(信号ピン, LED数, 最高輝度);
```
