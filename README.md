# SerialLED

## function
```
    void showWave() {
      /*error*/
      if (!cycle || !ledGroups)return;
      |処理内に上記の変数で除算を行うとこがあるため、0だと関数を抜けさせる
      

      if ((time = millis()) - lastTime > cycle)lastTime = time;
      float referenceX = (time - lastTime) * 2.0 / cycle;
      |周期内(cycle)で0から1へ上昇   2πにするため|    |cycleまで上昇するのをcycleで割ると1まで上昇する

      for (int j = 0; j < ledGroups; j++) {
                          |処理軽量化のためすべてのledに送るデータは処理せず色相のまとまりだけ計算
                          
        int8_t rgb[3];  //RGBデータ格納用
        
        float x = referenceX - (direction ? 1 : -1) * (j * 2.0 / ledGroups);
        |sin関数に入れる用のX  |流れる向きを変える    |ledごとに色相をずらすledGroupsで割るとその値で色相が１周する
        
        for (int i = 0; i < 3; i++)
          rgb[i] = maxPower * (sin((x - i * 2 / 3.0) * PI) + 1) / 2;
                  |最大値         |rgbごとに色相をずらす    |sin波を正の数に持ち上げる |sin波の振幅は2なので1にする
          
        for (int i = 0; i < ledNum / ledGroups; i++)
          pixel->setPixelColor(j + ledGroups * i, rgb[0], rgb[1], rgb[2]);
          |データセット
      }
      
      pixel->show();
      |出力
    }
```
