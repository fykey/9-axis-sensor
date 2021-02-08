//test1 地磁気，加速度，ジャイロの算出プログラム．
//正しく算出されれば次はroll,　pitch, yawの計算プログラムを作る

#include "Wire.h"
#include "SD.h"
#include "SPI.h"

int16_t axRaw, ayRaw, azRaw, gxRaw, gyRaw, gzRaw, temperature, mx, my, mz, i;
File dataFile;

void setup() {
  // mpu用setup start
  Wire.begin();
  TWBR = 24;
  Serial.begin(9600);
  setupMPU9255();
  
Serial.print(F("Initializing SD card..."));


 pinMode(SS, OUTPUT);
 digitalWrite(SS, HIGH);
//SD設定start
if (!SD.begin(10)) {
    Serial.println(F("Card failed, or not present"));
    return;
  //nanoの場合は10, unoは4
    while(1);
   }
   
  Serial.println(F("ok."));
    SD.remove("datalog.txt");
   File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
  dataFile.print("mx");dataFile.print("\t");
  dataFile.print("my");dataFile.print("\t");
  dataFile.print("mz");dataFile.print("\t");
  dataFile.print("axRaw");dataFile.print("\t");
  dataFile.print("ayRaw");dataFile.print("\t");
  dataFile.print("azRaw");dataFile.print("\t");
  dataFile.print("gxRaw");dataFile.print("\t");
  dataFile.print("gyRaw");dataFile.print("\t");
  dataFile.print("gzRaw");dataFile.print("\t");
  dataFile.print("\n");
  dataFile.close();
}
//SD設定終了
}

void loop() {
 readCompass();
  Serial.print("mx:");
  Serial.print(mx); Serial.print(",");
  Serial.print("my:");
  Serial.print(my); Serial.print(",");
  Serial.print("mz:");
  Serial.print(mz); Serial.print(",");
  Serial.print("axRaw:");
  Serial.print(axRaw); Serial.print(",");
  Serial.print("ayRaw:");
  Serial.print(ayRaw); Serial.print(",");
  Serial.print("azRaw:");
  Serial.print(azRaw); Serial.print(",");
  Serial.print("gxRaw");
  Serial.print(gxRaw); Serial.print(",");
  Serial.print("gyRaw");
  Serial.print(gyRaw); Serial.print(",");
  Serial.print("gzRaw");
  Serial.println(gzRaw);
 
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
if (dataFile) {
  dataFile.print(mx);dataFile.print("\t");
  dataFile.print(my);dataFile.print("\t");
  dataFile.print(mz);dataFile.print("\t");
  dataFile.print(axRaw);dataFile.print("\t");
  dataFile.print(ayRaw);dataFile.print("\t");
  dataFile.print(azRaw);dataFile.print("\t");
  dataFile.print(gxRaw);dataFile.print("\t");
  dataFile.print(gyRaw);dataFile.print("\t");
  dataFile.print(gzRaw);dataFile.print("\t");
  dataFile.print("\n");
  dataFile.close();
}
else{
  Serial.println(F("error opening datalog.txt"));
}
 
}

void readCompass() {
  //地磁気センサの値読み取り
  Wire.beginTransmission(0x0C);
  Wire.write(0x02);
  Wire.endTransmission();
  Wire.requestFrom(0x0C, 1);

  uint8_t ST1 = Wire.read();
  if (ST1 & 0x01) {
    Wire.beginTransmission(0x0C);
    Wire.write(0x03);
    Wire.endTransmission();
    Wire.requestFrom(0x0C, 7);
    uint8_t i = 0;
    uint8_t buf[7];
    while (Wire.available()) {
      buf[i++] = Wire.read();
    }
    if (!(buf[6] & 0x08)) {
      mx = ((int16_t)buf[1] << 8) | buf[0];
      my = ((int16_t)buf[3] << 8) | buf[2];
      mz = ((int16_t)buf[5] << 8) | buf[4];
    }
  }

  //加速度，gyroセンサの値読み取り
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 14);
  while (Wire.available() < 14);
  axRaw = Wire.read() << 8 | Wire.read();
  ayRaw = Wire.read() << 8 | Wire.read();
  azRaw = Wire.read() << 8 | Wire.read();
  temperature = Wire.read() << 8 | Wire.read();
  gxRaw = Wire.read() << 8 | Wire.read();
  gyRaw = Wire.read() << 8 | Wire.read();
  gzRaw = Wire.read() << 8 | Wire.read();
  

  
}

void setupMPU9255() {
  //クロック設定
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();

  //Gyro初期設定
  Wire.beginTransmission(0x68);
  Wire.write(0x1B);
  Wire.write(0x08);
  Wire.endTransmission();

  //加速度センサー初期設定
  Wire.beginTransmission(0x68);
  Wire.write(0x1C);
  Wire.write(0x10);
  Wire.endTransmission();

//地磁気センサー設定
//地磁気のみ別ICより，INT_PIN_CFGレジスタのバイパスを[ENABLE]にする
  Wire.beginTransmission(0x68);
  Wire.write(0x37);
  Wire.write(0x02);
  Wire.endTransmission();

  Wire.beginTransmission(0x0C);
  Wire.write(0x0A);
  Wire.write(0x16);
  Wire.endTransmission();

  //LPF(ローパスフィルタ)設定
  Wire.beginTransmission(0x68);
  Wire.write(0x1A);
  Wire.write(0x03);//addressが(0x05)の場合と検証してみる
  Wire.endTransmission();

  delay(500);
}
