#ifndef OBDH_H
#define OBDH_H

#include <Arduino.h>

class SDCardHandler {
public:
  int begin();
  void logLine(const String &line);
  String timestamp();
};

class LORA32 {
public:
  void begin();
  void sendResumo(byte *linha);
  void handleRx();
  String getLastCommand();
  bool isReady();

  bool sending = false;
  String lastReceived = "";

private:
  void setupLoRa();
};

#endif
