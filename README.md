# clock4lab
## about
ESP32 + ST7789 (OLED display)

で，Wi-Fiでインターネット接続し，NTPを用いてインターネット時刻同期を行い，OLEDディスプレイに時刻を表示する時計．

ついでに，

- 今日
- 3日前
- 7日前
- 14日前

の日付も表示する．

（ハエの管理に便利な日付）

## Wi-Fi接続について
SSIDとパスワードは`wifi_credentials.h`に定義しておく．

このファイルは`.gitignore`で同期しないようになっている．（インシデントは良くないので）

雛形は `wifi_credentials.h.sample` に掲示するので，適宜内容を変更の上，`wifi_credentials.h`にリネームすることで使用可能と思われる．