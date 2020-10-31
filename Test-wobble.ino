#include <Wire.h>
#include <Filter.h>

#define AxOffset 4752
#define AyOffset 0
#define AzOffset -100

const int MPU_addr=0x68;  // I2C address of the MPU-6050
float AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ; //7 read values from Sensor (3 axis of Accel and Gyro, and 1 from Temp)

float pitch = 0; //angles in deg
float roll = 0;

ExponentialFilter<float> FAcX(20,0);
ExponentialFilter<float> FAcY(20,0);
ExponentialFilter<float> FAcZ(20,0);
ExponentialFilter<float> FGyX(10,0);
ExponentialFilter<float> FGyY(10,0);
ExponentialFilter<float> FGyZ(10,0);

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Wire.beginTransmission(MPU_addr); //Begin Transmission to the Slave (in this case, the MPU-6050)
  Wire.write(0x6B);  // points to PWR_MGMT_1 register 
  Wire.write(0);     // set the register address value to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  grabValues(); //Grab Raw Values for AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ
  filtering(); //Filter Raw Values and add offsets, re-assign to AcX, AcY, AcZ, GyX, GyY, GyZ

  pitch = atan2f((float)AcX, (float)AcZ)*180/3.14;
  roll = atan2f((float)AcY, sqrt(AcX*AcX+AcZ*AcZ))*180/3.14;

  String p = String(int(pitch));//side to side (90 straight up)
  String r = String(int(roll));//forward to back (0 straight up)

  String dataSent1 = String(p + ',');
  String dataSent2 = String(dataSent1 + r);

  Serial.println(dataSent2);
}

void grabValues()
{
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false); //send a restart, keep connection active
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
}

void filtering()
{
  FAcX.Filter(AcX);
  FAcY.Filter(AcY);
  FAcZ.Filter(AcZ);
  
  AcX = ((FAcX.Current() + AxOffset)/16384);
  AcY = ((FAcY.Current() + AyOffset)/16384);
  AcZ = ((FAcZ.Current() + AzOffset)/16384);
  
  FGyX.Filter(GyX);
  FGyY.Filter(GyY);
  FGyZ.Filter(GyZ);

  GyX = (FGyX.Current())/131;
  GyY = (FGyY.Current())/131;
  GyZ = (FGyZ.Current())/131;
}
