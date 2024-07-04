#include "LoRa_module.h"


LoRaModule::LoRaModule(uint8_t pinCS, uint8_t pinRST, uint8_t pinIRQ) {
    setPins(pinCS, pinRST, pinIRQ);
    
    if (begin(FREQ)) m_status = READY;
    else m_status = NO_LORA;
}


LoRaModule::Status LoRaModule::status() {
    return m_status;
}


LoRaReceiver::LoRaReceiver(uint8_t pinCS, uint8_t pinRST, uint8_t pinIRQ) 
    : LoRaModule(pinCS, pinRST, pinIRQ) {

}


LoRaReceiver::OpStatus LoRaReceiver::receiveData(int packetSizeT) {
    int packetSize = LoRa.parsePacket();
  
    if (!packetSize) {
        Serial.print("Received packet from transmitter 1: '");
        
        while (LoRa.available()) {
            String LoRaData = LoRa.readString();
            Serial.print(LoRaData); 
        }
        
        Serial.print("' with Kelompok TE 1 ");
        Serial.println(LoRa.packetRssi());
    } 

    return SUCCESS;
}


LoRaTransmitter::LoRaTransmitter(uint8_t pinCS, uint8_t pinRST, uint8_t pinIRQ) 
    : LoRaModule(pinCS, pinRST, pinIRQ) {
}


LoRaTransmitter::OpStatus LoRaTransmitter::transmitData(SoilReading& soilReading) {
    if (!beginPacket()) {
        Serial.println("Failed to start packet.");
        return OpStatus::FAIL;
    }

    print(soilReading.m_soilData);
    print(',');
    print(soilReading.m_epoch);

    if (!endPacket(true)) {
        Serial.println("Failed to end packet.");
        return OpStatus::FAIL;
    }

    Serial.println("Data transmission successful.");
    return OpStatus::SUCCESS;
}
