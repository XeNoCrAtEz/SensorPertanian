#ifndef __LoRa_H__
#define __LoRa_H__

#include "Arduino.h"
#include <SPI.h>
#include "LoRa.h"



class LoRaModule : public LoRaClass {
public:
    // class status codes
    enum Status {
        READY,
        UNKNOWN_ERROR,
    };

    // operation status codes
    enum OpStatus {
        SUCCESS,
        STATUS_ERROR,
    };


private:
    enum LoRaParams {
        // MAX_COUNT = 10000,
        // JSON_ENTRY_SIZE = 512,
    };


// CLASS ATTRIBUTES AND METHODS
private:    
    Status m_status = UNKNOWN_ERROR;


public:
    LoRaModule();
    virtual ~LoRaModule() noexcept = default;
    Status status();


};


class LoRaReceiver : public LoRaModule {
public:
    // class status codes
    enum Status {
        READY,
        UNKNOWN_ERROR,
    };

    // operation status codes
    enum OpStatus {
        SUCCESS,
        STATUS_ERROR,
    };


private:
    enum LoRaParams {
        // MAX_COUNT = 10000,
        // JSON_ENTRY_SIZE = 512,
    };


// CLASS ATTRIBUTES AND METHODS
private:
    Status m_status = UNKNOWN_ERROR;


public:
    LoRaReceiver();
    virtual ~LoRaReceiver() noexcept = default;
    Status status();


};


class LoRaTransmitter : public LoRaModule {
public:
    // class status codes
    enum Status {
        READY,
        UNKNOWN_ERROR,
    };

    // operation status codes
    enum OpStatus {
        SUCCESS,
        STATUS_ERROR,
    };


private:
    enum LoRaParams {
        // MAX_COUNT = 10000,
        // JSON_ENTRY_SIZE = 512,
    };


private:
    Status m_status = UNKNOWN_ERROR;


public:
    LoRaTransmitter();
    virtual ~LoRaTransmitter() noexcept = default;
    Status status();


};


#endif