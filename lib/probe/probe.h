#ifndef _PROBE_H_
#define _PROBE_H_


#include <Arduino.h>
#include <ModbusMaster.h>

#include "soil_data.h"

// pre-define DEBUG when compiling to enable debug
#ifdef DEBUG

#include "debug.h"

#endif


// class for a probe
class Probe: public ModbusMaster {
public:
    // class status codes
    enum Status {
        READY,
        NO_PROBE,
        PROBE_ERROR,
        UNKNOWN_ERROR,
    };

    // operation status codes
    enum OpStatus {
        SUCCESS,
        STATUS_NO_PROBE,
        STATUS_ERROR,
    };


protected:
    enum ProbeParams {
        MAX_RESEND = 3,
        NUM_SAMPLES = 10,
        TOTAL_DATA = 7,
    };
    static const uint16_t ku16MBResponseTimeout = 100;  // ms

    Status m_status = UNKNOWN_ERROR;

    HardwareSerial m_probe;


protected:
    Status check();


public:
    Probe(int HWSerialNum=1, int addr=0x01);

    OpStatus get_data(uint16_t& data, int regNum);
    void calibrateNPK(SoilData& soilData);

    Status status();
    void end();

    virtual OpStatus sample(SoilData& soilData) = 0;
    virtual void begin(int rx, int tx) = 0;

};


class ProbeKHDTK : public Probe {
private:
    enum ProbeParams {
        PROBE_BAUDRATE = 9600,

        REG_NITRO   = 0x001E,
        REG_PHOS    = 0x001F,
        REG_KALI    = 0x0020,
        REG_PH      = 0x0006,
        REG_TEMP    = 0x0013,
        REG_HUM     = 0x0012,
        REG_EC      = 0x0015,

        INDEX_NITRO = 0x0004,
        INDEX_PHOS  = 0x0005,
        INDEX_KALI  = 0x0006,
        INDEX_PH    = 0x0003,
        INDEX_TEMP  = 0x0001,
        INDEX_HUM   = 0x0000,
        INDEX_EC    = 0x0002,
    };


public:
    ProbeKHDTK(int rx, int tx, int HWSerialNum=1, int addr=0x01);

    OpStatus sample(SoilData& soilData) override;
    void begin(int rx, int tx);
};


class ProbeDefault : public Probe {
private:
    enum ProbeParams {
        PROBE_BAUDRATE = 4800,

        REG_NITRO   = 0x0004,
        REG_PHOS    = 0x0005,
        REG_KALI    = 0x0006,
        REG_PH      = 0x0003,
        REG_TEMP    = 0x0001,
        REG_HUM     = 0x0000,
        REG_EC      = 0x0002,

        INDEX_NITRO = REG_NITRO,
        INDEX_PHOS  = REG_PHOS,
        INDEX_KALI  = REG_KALI,
        INDEX_PH    = REG_PH,
        INDEX_TEMP  = REG_TEMP,
        INDEX_HUM   = REG_HUM,
        INDEX_EC    = REG_EC,
    };


public:
    ProbeDefault(int rx, int tx, int HWSerialNum=1, int addr=0x01);

    OpStatus sample(SoilData& soilData) override;
    void begin(int rx, int tx);
};

class ProbeNew : public Probe {
private:
    enum ProbeParams {
        TOTAL_DATA = 8,

        PROBE_BAUDRATE = 9600,

        REG_NITRO   = 0x0005,
        REG_PHOS    = 0x0006,
        REG_KALI    = 0x0007,
        REG_PH      = 0x0004,
        REG_TEMP    = 0x0000,
        REG_HUM     = 0x0001,
        REG_EC      = 0x0002,
        REG_SALT    = 0x0003,


        INDEX_NITRO = REG_NITRO,
        INDEX_PHOS  = REG_PHOS,
        INDEX_KALI  = REG_KALI,
        INDEX_PH    = REG_PH,
        INDEX_TEMP  = REG_TEMP,
        INDEX_HUM   = REG_HUM,
        INDEX_EC    = REG_EC,
        INDEX_SALT  = REG_SALT,
    };


public:
    ProbeNew(int rx, int tx, int HWSerialNum=1, int addr=0x01);

    OpStatus sample(SoilData& soilData);
    void begin(int rx, int tx);
};


#endif