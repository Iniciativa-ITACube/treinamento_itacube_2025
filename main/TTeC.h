#ifndef TTeC_H
#define TTeC_H

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "HT_SSD1306Wire.h"
#include<Wire.h>

class BME280Sensor
{
public:
  void begin();
  float readTemperature();
  float readPressure();
  float readHumidity();

private:
  Adafruit_BME280 bme;
  const uint8_t ADDRESS = 0x76;
  TwoWire wire = TwoWire(1);
};

class MPU6050Sensor
{
public:
  void VextON();
  void VextOFF();
  void begin();
  void update();
  float getAcX();
  float getAcY();
  float getAcZ();
  float getGyX();
  float getGyY();
  float getGyZ();

private:
  const int MPU_ADDR = 0x68;
  float AcX, AcY, AcZ;
  float GyX, GyY, GyZ;
};

#endif
