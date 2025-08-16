#include "TTeC.h"
#include "Pins.h"
#include <Arduino.h>

extern SSD1306Wire factory_display;
extern TwoWire Wire1;
const int MPU_ADDR=0x68; 
//
// BME280Sensor
//
void BME280Sensor::begin() {
  wire.begin(PIN_BME_SDA, PIN_BME_SCL);
  wire.setClock(100000);
  if (!bme.begin(ADDRESS, &wire)) {
    Serial.println("Could not find a valid BME280 sensor!");
    while (1);
  }

  Serial.print("BME280 Sensor ID: 0x");
  Serial.println(bme.sensorID(), 16);
}

float BME280Sensor::readTemperature() {
  return bme.readTemperature();
}

float BME280Sensor::readPressure() {
  return bme.readPressure() / 100.0F;
}

float BME280Sensor::readHumidity() {
  return bme.readHumidity();
}

//
// MPU6050Sensor
//
void  MPU6050Sensor::VextON(void)
{
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext, LOW);
}

void  MPU6050Sensor::VextOFF(void) //Vext default OFF
{
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext, HIGH);
}
void MPU6050Sensor::begin() {
	VextON();
	delay(100);
  Wire1.begin(PIN_MPU_SDA, PIN_MPU_SCL, 400000);
  Wire1.beginTransmission(MPU_ADDR);
  Wire1.write(0x6B);
  Wire1.write(0);
  Wire1.endTransmission(true);
}

void MPU6050Sensor::update() {
  Wire1.beginTransmission(MPU_ADDR);
  Wire1.write(0x3B);
  Wire1.endTransmission(false);
  Wire1.requestFrom(MPU_ADDR, 12, true);

  AcX = Wire1.read() << 8 | Wire1.read();
  AcY = Wire1.read() << 8 | Wire1.read();
  AcZ = Wire1.read() << 8 | Wire1.read();
  GyX = Wire1.read() << 8 | Wire1.read();
  GyY = Wire1.read() << 8 | Wire1.read();
  GyZ = Wire1.read() << 8 | Wire1.read();
}

float MPU6050Sensor::getAcX() { return AcX; }
float MPU6050Sensor::getAcY() { return AcY; }
float MPU6050Sensor::getAcZ() { return AcZ; }
float MPU6050Sensor::getGyX() { return GyX; }
float MPU6050Sensor::getGyY() { return GyY; }
float MPU6050Sensor::getGyZ() { return GyZ; }
