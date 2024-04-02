#include <SPI.h>
#include "LoRa.h"

#define SS_PIN 18
#define RST_PIN 14
#define DI0_PIN 26

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);
  LoRa.enableCrc();

  LoRa.setPins(SS_PIN, RST_PIN, DI0_PIN);
}

void loop() {
  if (LoRa.parsePacket()) {
    String message = "";
    while (LoRa.available()) {
      message += (char)LoRa.read();
    }

    Serial.println("Received packet: " + message);
  }
}


#define SS_PIN 18
#define RST_PIN 14
#define DI0_PIN 26

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);
  LoRa.enableCrc();

  LoRa.setPins(SS_PIN, RST_PIN, DI0_PIN);
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String message = "";
    while (LoRa.available()) {
      message += (char)LoRa.read();
    }

    Serial.println("Received packet: " + message);
  }
}
