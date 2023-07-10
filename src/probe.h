#include <Arduino.h>
#include <HardwareSerial.h>

// pre-define DEBUG when compiling to enable debug
#ifdef DEBUG

#include "debug.h"

#endif

// Probe constants parameters
const int PROBE_SERIAL_BAUDRATE = 9600;
const int PROBE_RX = 2;
const int PROBE_TX = 14;

// const byte code[]= {addressCode(0x01), functionCode(0x03), regStartAddr_L, regStartAddr_H, regLen_L, regLen_H, CRC_L, CRC_H};
const byte nitro[] = {0x01, 0x03, 0x00, 0x1E, 0x00, 0x01, 0xE4, 0x0C};
const byte phos[]  = {0x01, 0x03, 0x00, 0x1F, 0x00, 0x01, 0xB5, 0xCC};
const byte kali[]  = {0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xC0};
const byte ph[]    = {0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0B};
const byte temp[]  = {0x01, 0x03, 0x00, 0x13, 0x00, 0x01, 0x75, 0xCF};
const byte hum[]   = {0x01, 0x03, 0x00, 0x12, 0x00, 0x01, 0x24, 0x0F};
const byte ec[]    = {0x01, 0x03, 0x00, 0x15, 0x00, 0x01, 0x95, 0xCE};
const byte CODE_SIZE = 8;
const byte RESPONSE_SIZE = 7;

// class for storing soil data
class SoilData {
private:
    int nitrogen, phosphorus, kalium;
    float pH, temperature, humidity;
    int EC;
public:
    SoilData() = default;
    SoilData(SoilData&) = default;
    SoilData(
        int nitrogen, int phosphorus, int kalium,
        float pH, float temperature, float humidity,
        int EC
    );
};

// function declaration
void sample(
    int& nitrogen, int& phosphorus, int& kalium,
    float& pH, float& temperature, float& humidity,
    int& EC
);

bool send_data_req(const byte code[]);

int get_data(const byte code[]);