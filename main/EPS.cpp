#include "EPS.h"
#include "Pins.h"
#include <Arduino.h>

void Battery::begin()
{
  analogReadResolution(12); // ESP32-S3: ADC resolution (0–4095)
}

float Battery::readVoltage()
{
  int raw = analogRead(PIN_VBAT);
  float vAdc = (raw / 4095.0) * 3.3; // Tensão lida na entrada ADC
  float vbat = vAdc * 1.16;          // Correção de leitura
  return vbat;
}
