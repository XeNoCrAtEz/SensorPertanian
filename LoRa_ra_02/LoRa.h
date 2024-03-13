#ifndef LoRa_h
#define LoRa_h

#include "Arduino.h"
#include <SPI.h>

class LoRaClass {
public:
  LoRaClass();

  int begin(long frequency);
  void end();

  int beginPacket();
  int endPacket(bool async = false);
  int parsePacket(int size = 0);
  int packetRssi();
  float packetSnr();

  // ... (definisi metode lainnya)

  size_t write(uint8_t byte);
  size_t write(const uint8_t *buffer, size_t size);
  
  // ... (definisi metode lainnya)
};

extern LoRaClass LoRa;

#endif
