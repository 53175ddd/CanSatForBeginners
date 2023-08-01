/* 結線 
| SD   | ESP32 |
| VDD  | 3.3V  |
| CD   | D5    |
| CMD  | D23   |
| CLK  | D18   |
| DAT0 | D19   |
| VSS  | GND   |

| BME280 | ESP32 |
| VDD    | 3.3V  |
| GND    | GND   |
| SDI    | D21   |
| SDO    | GND   |
| SCK    | D22   |
 */

/* SD カードを使うためのライブラリ */
#include "SD.h"

/* BME280 を使うためのライブラリ*/
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

/* 使用する GPIO を指定 */
#define LED 4

/* 1 ループごとの秒数を指定(ミリ秒) */
#define interval 1
#define ms       500

/* インスタンス作成 */
Adafruit_BME280 bme;
File file;

/* 各データを保存する変数を宣言 */
float temp;
float pres;
float hmid;

/* ログを保存するファイル名を指定 */
char filename[32] = "/ESP32_Base_Sample_Log.csv";

void setup() {
  /* デバッグ用のシリアル通信を開始 */
  Serial.begin(115200);

  /* GPIO の初期化 */
  pinMode(LED, OUTPUT);

  /* SD カードが使えないときの処理 */
  while(!SD.begin(5)) {
    Serial.print("Card Mount Failed.\n");
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED,  LOW);
    delay(500);
  }

  /* BME280 が使えないときの処理 */
  while(!bme.begin(0x76)) {
    Serial.print("BME280 sensor Failed.\n");
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED,  LOW);
    delay(500);
  }

  uint8_t count = 0;
  while(SD.exists(filename)) {
    count++;
    sprintf(filename, "/ESP32_Base_Sample_Log%d.csv", count);
  }

  Serial.print("\nfilename is ");
  Serial.print(filename);
  Serial.print("\n");

  SD_Append("temp, pres, hmid\n");
  Serial.print("\n-------------------\ntemp, pres, hmid\n");
}

void loop() {
  digitalWrite(LED, HIGH);

  temp = bme.readTemperature();
  pres = bme.readPressure() / 100.0F;
  hmid = bme.readHumidity();

  char buffer[42];
  sprintf(buffer, "%f, %f, %f\n", temp, pres, hmid);
  SD_Append(buffer);
  Serial.print(buffer);

  digitalWrite(LED,  LOW);

  for(uint8_t i = 0; i < interval; i++) {
    delay(ms);
  }
}

void SD_Append(String loc) {
  file = SD.open(filename, FILE_APPEND);

  if(!file) {
    Serial.print("Failed to open file for writing.\n");
    return;
  }

  file.print(loc);
  file.close();
}

void SD_Read(void) {
  file = SD.open(filename);

  if(!file) {
    Serial.print("Failed to open file for reading.\n");
    return;
  }

  while(file.available()) {
    Serial.write(file.read());
  }

  file.close();
}

void SD_ListDir(void) {
  File root = SD.open("/");
  file = root.openNextFile();

  while(file) {
    Serial.println(file.name());
    file = root.openNextFile();
  }
}
