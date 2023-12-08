#include <WiFi.h>
#include <time.h>
#include "wifi_credentials.h" // SSID and Password

#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h>  // Hardware-specific library for ST7789
#include <SPI.h>

// #if defined(ARDUINO_FEATHER_ESP32) // Feather Huzzah32
#define TFT_CS 25
#define TFT_RST 27
#define TFT_DC 26

// OPTION 2 lets you interface the display using ANY TWO or THREE PINS,
// tradeoff being that performance is not as fast as hardware SPI above.
#define TFT_MOSI 14  // Data out
#define TFT_SCLK 13  // Clock out

// wifi info (from wifi_credentials.h)
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;

// ntp
time_t tim3, tim7, tim14;
int indxday3, indxday7, indxday14;
struct tm timeInfo;
struct tm *timeInfo3;
struct tm *timeInfo7;
struct tm *timeInfo14;
const char *dayofweek[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
char year[4][6];
// const char *dayofweek[7] = {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"};
char date[20], hour_minute[20], date_3d[20], date_7d[20], date_14d[20];

bool kougo;

// display
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void setup(void) {
  Serial.begin(9600);
  Serial.print(F("Hello! ST77xx TFT Clock"));

  // OR use this initializer (uncomment) if using a 2.0" 320x240 TFT:
  tft.init(240, 320);  // Init ST7789 320x240

  // wifi connect
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  // NTP setting
  configTime(9 * 3600L, 0, "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp");

  Serial.println(F("Initialized"));
}

void loop() {
  // time
  if ((WiFi.status() == WL_CONNECTED)) {
    tft.setTextWrap(false);
    // clear display
    tft.fillScreen(ST77XX_BLACK);
    
    // wifi connected
    tft.setTextColor(ST77XX_BLUE);
    tft.setCursor(0, 0);
    tft.setTextSize(2);
    tft.println("Wi-Fi Connected");

    // time and date
    show_time(kougo);
    kougo = kougo ? false : true;

    delay(30000);
    
  } else {
    Serial.println(F("Wi-Fi not connected"));
    tft.setTextWrap(false);
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_RED);
    tft.setCursor(0, 0);
    tft.setTextSize(2);
    tft.println("Wifi Disconnected");
  }
}

void show_time(bool kougo) {
  getLocalTime(&timeInfo);    //tmオブジェクトのtimeInfoに現在時刻を入れ込む
  sprintf(year[0], "%04d/", timeInfo.tm_year + 1900);
  sprintf(date, "%02d/%02d", timeInfo.tm_mon + 1, timeInfo.tm_mday);  //日付に変換
  // 交互に. を表示したりけしたりする．
  if (kougo){
    sprintf(hour_minute, "%02d:%02d.", timeInfo.tm_hour, timeInfo.tm_min); //時間に変換
  } else {
    sprintf(hour_minute, "%02d:%02d", timeInfo.tm_hour, timeInfo.tm_min); //時間に変換
  }
  
 
  // 3日戻す
  tim3 = mktime(&timeInfo) - 3 * 24*3600;
  timeInfo3 = localtime(&tim3);
  indxday3 = timeInfo3->tm_wday;
  sprintf(year[1], "%04d/", timeInfo3->tm_year + 1900);
  sprintf(date_3d, "%02d/%02d", timeInfo3->tm_mon + 1, timeInfo3->tm_mday);  //日付に変換

  // 更に4日戻して7日前
  tim7 = mktime(&timeInfo) - 7 * 24*3600;
  timeInfo7 = localtime(&tim7);
  indxday7 = timeInfo7->tm_wday;
  sprintf(year[2], "%04d/", timeInfo7->tm_year + 1900);
  sprintf(date_7d, "%02d/%02d", timeInfo7->tm_mon + 1, timeInfo7->tm_mday);  //日付に変換

  // 更に7日戻して14日前
  tim14 = mktime(&timeInfo) - 14 * 24*3600;
  timeInfo14 = localtime(&tim14);
  indxday14 = timeInfo14->tm_wday;
  sprintf(year[3], "%04d/", timeInfo14->tm_year + 1900);
  sprintf(date_14d, "%02d/%02d", timeInfo14->tm_mon + 1, timeInfo14->tm_mday);  //日付に変換

  int datesize = 3;

  tft.setCursor(0, 30);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(7);
  tft.println(hour_minute);

  tft.setTextSize(2);
  tft.println("\n[ Today ]");
  tft.print(year[0]);
  tft.setTextSize(datesize);
  tft.print(date);
  tft.print(" ");
  tft.println(dayofweek[timeInfo.tm_wday]);
  
  tft.setTextSize(2);
  tft.println("\n[ 3-ka mae ]");
  tft.print(year[1]);
  tft.setTextSize(datesize);
  tft.print(date_3d);
  tft.print(" ");
  tft.println(dayofweek[indxday3]);

  tft.setTextSize(2);
  tft.println("\n[ 7-ka mae ]");
  tft.print(year[2]);
  tft.setTextSize(datesize);
  tft.print(date_7d);
  tft.print(" ");
  tft.println(dayofweek[indxday7]);

  tft.setTextSize(2);
  tft.println("\n[ 14-ka mae ]");
  tft.print(year[3]);
  tft.setTextSize(datesize);
  tft.print(date_14d);
  tft.print(" ");
  tft.println(dayofweek[indxday14]);
}
