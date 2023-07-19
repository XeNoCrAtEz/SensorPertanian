#include "probe.h"


Probe::Probe(int rx, int tx, int HWSerialNum, int addr)
        : ModbusMaster(),
          RX(rx), TX(tx), probe(HWSerialNum), address(addr)
{
    begin(addr, probe);
}


// ---------------------------- Probe KHDTK ------------------------------
ProbeKHDTK::ProbeKHDTK(int rx, int tx, int HWSerialNum, int addr)
        : Probe(rx, tx, HWSerialNum, addr)
{
    probe.begin(PROBE_BAUDRATE, SERIAL_8N1, rx, tx);
}


SoilData ProbeKHDTK::sample() {
    SoilData soilData;

    static const byte REG_NITRO = 0x001E;
    static const byte REG_PHOS  = 0x001F;
    static const byte REG_KALI  = 0x0020;
    static const byte REG_PH    = 0x0006;
    static const byte REG_TEMP  = 0x0013;
    static const byte REG_HUM   = 0x0012;
    static const byte REG_EC    = 0x0015;

    for (int i = 0; i < TOTAL_DATA; i++) {

    }

















    int resultCode = readHoldingRegisters(0x00, TOTAL_DATA);     // read from 0x00-0x06
    
    for(int i = 0; resultCode == ku8MBResponseTimedOut; i++) {
        Serial.println("Probe not responding!");
        if (i >= MAX_RESEND) {
            Serial.println("Max resend reached! Communicating with probe failed!");
            return SoilData();
        }
        Serial.print("Resending... (resend attempt: ");
        Serial.print(i);
        Serial.println(")");
        resultCode = readHoldingRegisters(0x00, TOTAL_DATA);
    }

    if(resultCode != ku8MBSuccess) {
        Serial.print("Error! Result code: ");
        Serial.print(resultCode);
        Serial.println("");

        return SoilData();
    }

#ifdef DEBUG
        // tampilkan respond bytes
        Serial.print("Received bytes: ");
        for (byte i = 0; i < TOTAL_DATA; i++) {
            Serial.print(getResponseBuffer(i), HEX);
            Serial.print(" ");
        }
        Serial.println();
#endif

    soilData.nitrogen = getResponseBuffer(INDEX_NITRO);
    soilData.phosphorus = getResponseBuffer(INDEX_PHOS);
    soilData.kalium = getResponseBuffer(INDEX_KALI);
    soilData.pH = getResponseBuffer(INDEX_PH);
    soilData.temperature = getResponseBuffer(INDEX_TEMP);
    soilData.humidity = getResponseBuffer(INDEX_HUM);
    soilData.EC = getResponseBuffer(INDEX_EC);

    return soilData;

}


// ------------------------- Probe Default (Aliexpress) ---------------------------
ProbeDefault::ProbeDefault(int rx, int tx, int HWSerialNum, int addr)
        : Probe(rx, tx, HWSerialNum, addr)
{
    probe.begin(PROBE_BAUDRATE, SERIAL_8N1, rx, tx);
}


SoilData ProbeDefault::sample() {
    SoilData soilData = SoilData();

    for (int i = 0; i < NUM_SAMPLES; i++) {
        int resultCode = readHoldingRegisters(0x00, TOTAL_DATA);     // read from 0x00-0x06
        
        for(int attempts = 0; resultCode == ku8MBResponseTimedOut; attempts++) {
            Serial.println("Probe not responding!");
            if (attempts >= MAX_RESEND) {
                Serial.println("Max resend reached! Communicating with probe failed!");
                return soilData;
            }
            Serial.print("Resending... (resend attempt: ");
            Serial.print(attempts);
            Serial.println(")");
            resultCode = readHoldingRegisters(0x00, TOTAL_DATA);
        }

        if(resultCode != ku8MBSuccess) {
            Serial.print("Error! Result code: ");
            Serial.print(resultCode);
            Serial.println("");

            return soilData;
        }

    #ifdef DEBUG
            // tampilkan respond bytes
            Serial.print("Received bytes: ");
            for (byte i = 0; i < TOTAL_DATA; i++) {
                Serial.print(getResponseBuffer(i), HEX);
                Serial.print(" ");
            }
            Serial.println();
    #endif

        soilData.nitrogen += getResponseBuffer(INDEX_NITRO);
        soilData.phosphorus += getResponseBuffer(INDEX_PHOS);
        soilData.kalium += getResponseBuffer(INDEX_KALI);
        soilData.pH += getResponseBuffer(INDEX_PH) / (float) 10;
        soilData.temperature += getResponseBuffer(INDEX_TEMP) / (float) 10;
        soilData.humidity += getResponseBuffer(INDEX_HUM) / (float) 10;
        soilData.EC += getResponseBuffer(INDEX_EC);
    }

    soilData.nitrogen /= NUM_SAMPLES;
    soilData.phosphorus /= NUM_SAMPLES;
    soilData.kalium /= NUM_SAMPLES;
    soilData.pH /= NUM_SAMPLES;
    soilData.temperature /= NUM_SAMPLES;
    soilData.humidity /= NUM_SAMPLES;
    soilData.EC /= NUM_SAMPLES;

    return soilData;
}