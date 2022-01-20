# SerialLED

## functions

### セッター
- RGB個々の輝度設定
```
setRBrightness(byte brightness);
setGBrightness(byte brightness);
setBBrightness(byte brightness);
```

- 全体輝度設定
```
updateBrightness(byte brightness);
```

### 出力系
- レインボー出力
```
rainbow(int cycle, int ledGroups, byte saturation = 200); //発光パターン非同期
sameRainbow(int cycle, byte saturation = 200);            //発光パターン同期
```

- 流れるやつ
```
void flowing(int cycle, int ledGroups, byte duty = 100, uint32_t onColor = 0xfffff, uint32_t offColor = 0x000000);
```

- チカチカ
```
stopMotion(int cycle, byte whenFlash = 50, byte flashTime = 30, uint32_t colora = 0xff0000, uint32_t colorb = 0xff5100);
```

- ランダム点灯(未)
```
randomPixel(int speed);
```

- 全灯
```
fill(byte r, byte g, byte b); //R,G,B
fill(uint32_t rgb);           //RGB下位8+8+8bit
```

- クリア
```
clear(void);
```

### その他
- HSV->RGB
```
hsv(byte h, byte s);
```

- RGBの輝度更新
```
transformBrightness(uint32_t &data);
```

- 処理速度表示
```
processingus(char ln[] = NULL);
```
