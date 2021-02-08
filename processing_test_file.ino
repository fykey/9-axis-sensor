
//0点調整が必要
//
#include "Wire.h"

#include <MadgwickAHRS.h>
Madgwick MadgwickFilter;

int16_t ax, ay, az, gx, gy, gz, temperature, mx, my, mz, i;
float  ax1, ay1, az1, gx1, gy1, gz1, mx1, my1, mz1, ROLL, PITCH, YAW;
void setup() {
  // mpu用setup start
  Wire.begin();
  TWBR = 24;

  MadgwickFilter.begin(500); //filterの初期化の周波数
  //デバッグ:この周波数とserialの周波数が一致していないことに注意
  
  Serial.begin(9600);
  setupMPU9255();
}


void loop() {
 readCompass();
 MadgwickFilter.update(gx1, gy1, gz1, ax1, ay1, az1, mx1, my1, mz1);

 ROLL = MadgwickFilter.getRoll();
 PITCH = MadgwickFilter.getPitch();
 YAW = -180 + MadgwickFilter.getYaw();

  Serial.print(ROLL); Serial.print(",");

  Serial.print(PITCH); Serial.print(",");

  Serial.print(YAW); 
  Serial.print("\n");
  
 
  

 
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
  ax = Wire.read() << 8 | Wire.read();
  ay = Wire.read() << 8 | Wire.read();
  az = Wire.read() << 8 | Wire.read();
  temperature = Wire.read() << 8 | Wire.read();
  gx = Wire.read() << 8 | Wire.read();
  gy = Wire.read() << 8 | Wire.read();
  gz= Wire.read() << 8 | Wire.read();

  ax1 = ax / 16384.0;
  ay1 = ay / 16384.0;
  az1 = az / 16384.0;
  gx1 = gx / 131.0;
  gy1 = gy / 131.0;
  gz1 = gz / 131.0;
  mx1 = mx / 32768.0f * 4800.0f - 19.8; 
  my1 = my / 32768.0f * 4800.0f - 5.2;
  mz1 = mz / 32768.0f * 4800.0f - 21.5;
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
