#include <SPI.h>
#include "LoRa.h"

#define SS_PIN  5
#define RST_PIN 14
#define DI0_PIN 26

LoRaClass LoRa; // Membuat objek LoRaClass

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
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
  String message = "Hello, LoRa!";

  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();

  Serial.println("Message sent: " + message);
  delay(1000);
}

  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);
  LoRa.enableCrc();

  LoRa.setPins(SS_PIN, RST_PIN, DI0_PIN);
}

void loop() {
  String message = "Hello, LoRa!";

  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();

  Serial.println("Message sent: " + message);
  delay(1000);
}
