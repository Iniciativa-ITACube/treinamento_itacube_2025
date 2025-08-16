#include <Arduino.h>
#include "PAY.h"
#include "Pins.h"

// ====================== MQ135 ======================
void MQ135::begin()
{
  analogReadResolution(12);
}

float MQ135::readVoltage()
{
  int raw = analogRead(PIN_MQ135_A0); // Correção aqui
  return raw * 3.3 / 4095.0;
}

bool MQ135::isInitializedSensor()
{
  return isInitialized;
}

// ====================== GUVA ======================
void GUVA::begin()
{
  analogReadResolution(12);
  pinMode(PIN_GUVA, INPUT);
  isInitialized = true;
}

float GUVA::readUV()
{
  int raw = analogRead(PIN_GUVA);
  return raw * 3.3 / 4095.0;
}

bool GUVA::isInitializedSensor()
{
  return isInitialized;
}
