#ifndef __LoRa_H__
#define __LoRa_H__

#include "Arduino.h"
#include <SPI.h>
#include "LoRa.h"

#include "soil_data.h"


class LoRaModule : public LoRaClass {
protected:
    enum LoRaParams {
        FREQ = 433000000,
    };


public:
    // class status codes
    enum Status {
        READY,
        NO_LORA,
        UNKNOWN_ERROR,
    };

    // operation status codes
    enum OpStatus {
        SUCCESS,
        STATUS_NO_LORA,
        STATUS_ERROR,
        TIMEOUT,
        NO_PACKET,
        FAIL,
    };


protected:
    Status m_status = UNKNOWN_ERROR;


public:
    LoRaModule(uint8_t pinCS, uint8_t pinRST, uint8_t pinIRQ);
    Status status();


};


class LoRaReceiver : public LoRaModule {
public:
    LoRaReceiver(uint8_t pinCS, uint8_t pinRST, uint8_t pinIRQ);
    OpStatus receiveData(int packetSize);

};


class LoRaTransmitter : public LoRaModule {
public:
    LoRaTransmitter(uint8_t pinCS, uint8_t pinRST, uint8_t pinIRQ);
    OpStatus transmitData(SoilReading& soilReading);


};


#endif