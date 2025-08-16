#ifndef PAY_H
#define PAY_H

class MQ135
{
public:
  void begin();
  float readVoltage();
  bool isInitializedSensor();
    bool isInitialized = false;

private:

};

class GUVA
{
public:
  void begin();
  float readUV();
  bool isInitializedSensor();
    bool isInitialized = false;

private:

};

#endif
