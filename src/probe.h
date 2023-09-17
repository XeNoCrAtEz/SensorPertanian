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
protected:
    HardwareSerial probe;

    static const int MAX_RESEND = 10;

    static const byte NUM_SAMPLES = 10;

    static const byte TOTAL_DATA = 7;

    static const int PROBE_BAUDRATE = 0;    // needs to be specified by the child classes

    // konstanta kalibrasi
    static constexpr float const N_a = NITR_CALIB_A;
    static constexpr float const N_b = NITR_CALIB_B;

    static constexpr float const P_a = PHOS_CALIB_A;
    static constexpr float const P_b = PHOS_CALIB_B;
    
    static constexpr float const K_a = KALI_CALIB_A;
    static constexpr float const K_b = KALI_CALIB_B;

private:
    int RX, TX;
    int address;

    static const uint16_t ku16MBResponseTimeout = 100;  // ms

public:
    Probe(int rx, int tx, int HWSerialNum=1, int addr=0x01);

    uint16_t get_data(int regNum);
};


class ProbeKHDTK : public Probe {
private:
    static const int PROBE_BAUDRATE = 9600;
    static const byte REG_NITRO = 0x001E;
    static const byte REG_PHOS  = 0x001F;
    static const byte REG_KALI  = 0x0020;
    static const byte REG_PH    = 0x0006;
    static const byte REG_TEMP  = 0x0013;
    static const byte REG_HUM   = 0x0012;
    static const byte REG_EC    = 0x0015;

    static const byte INDEX_NITRO = 0x0004;
    static const byte INDEX_PHOS  = 0x0005;
    static const byte INDEX_KALI  = 0x0006;
    static const byte INDEX_PH    = 0x0003;
    static const byte INDEX_TEMP  = 0x0001;
    static const byte INDEX_HUM   = 0x0000;
    static const byte INDEX_EC    = 0x0002;

public:

    ProbeKHDTK(int rx, int tx, int HWSerialNum=1, int addr=0x01);

    SoilData sample();
};


class ProbeDefault : public Probe {
private:
    static const int PROBE_BAUDRATE = 4800;
    static const byte REG_NITRO = 0x0004;
    static const byte REG_PHOS  = 0x0005;
    static const byte REG_KALI  = 0x0006;
    static const byte REG_PH    = 0x0003;
    static const byte REG_TEMP  = 0x0001;
    static const byte REG_HUM   = 0x0000;
    static const byte REG_EC    = 0x0002;

    static const byte INDEX_NITRO = REG_NITRO;
    static const byte INDEX_PHOS  = REG_PHOS;
    static const byte INDEX_KALI  = REG_KALI;
    static const byte INDEX_PH    = REG_PH;
    static const byte INDEX_TEMP  = REG_TEMP;
    static const byte INDEX_HUM   = REG_HUM;
    static const byte INDEX_EC    = REG_EC;
    


public:

    ProbeDefault(int rx, int tx, int HWSerialNum=1, int addr=0x01);

    SoilData sample();
};

#endif