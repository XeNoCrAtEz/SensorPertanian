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
    enum ErrorCodes {
        SUCCESS,
        NO_PROBE,
        PROBE_ERROR,
    };


protected:
    enum ProbeParams {
        MAX_RESEND = 10,
        NUM_SAMPLES = 10,
        TOTAL_DATA = 7,
    };
    static const uint16_t ku16MBResponseTimeout = 100;  // ms

    // konstanta kalibrasi
    static const float N_a;
    static const float N_b;

    static const float P_a;
    static const float P_b;
    
    static const float K_a;
    static const float K_b;

    HardwareSerial probe;


public:
    Probe(int rx, int tx, int HWSerialNum=1, int addr=0x01);

    ErrorCodes get_data(uint16_t& data, int regNum);
    virtual ErrorCodes sample(SoilData& soilData) = 0;
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

    ErrorCodes sample(SoilData& soilData);
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

    ErrorCodes sample(SoilData& soilData);
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

    ErrorCodes sample(SoilData& soilData);
};


#endif