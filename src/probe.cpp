#include "probe.h"


Probe::Probe(int rx, int tx, int HWSerialNum, int addr)
        : ModbusMaster(),
          RX(rx), TX(tx), probe(HWSerialNum), address(addr)
{
    begin(addr, probe);
}


// get a single register, use register number constants specified by the probe
uint16_t Probe::get_data(int regNum) {
    uint16_t result = 0;
    
    for (int i = 0; i < NUM_SAMPLES; i++) {
        int resultCode = readHoldingRegisters(regNum, 0x01);     // read one register only
        
        for(int attempts = 0; resultCode == ku8MBResponseTimedOut; attempts++) {
            Serial.println("Probe not responding!");
            if (attempts >= MAX_RESEND) {
                Serial.println("Max resend reached! Communicating with probe failed!");
                return 0;
            }
            Serial.print("Resending... (resend attempt: ");
            Serial.print(attempts);
            Serial.println(")");
            resultCode = readHoldingRegisters(regNum, 0x01);
        }

        if(resultCode != ku8MBSuccess) {
            Serial.print("Error! Result code: ");
            Serial.print(resultCode);
            Serial.println("");

            return 0;
        }

        result += getResponseBuffer(0);
    }

#ifdef DEBUG
    // tampilkan respond bytes
    Serial.print("Received bytes: ");
    Serial.println(getResponseBuffer(0x00), HEX);
#endif

    result /= NUM_SAMPLES;
    
    return result;
}


// ---------------------------- Probe KHDTK ------------------------------
ProbeKHDTK::ProbeKHDTK(int rx, int tx, int HWSerialNum, int addr)
        : Probe(rx, tx, HWSerialNum, addr)
{
    probe.begin(PROBE_BAUDRATE, SERIAL_8N1, rx, tx);
}


SoilData ProbeKHDTK::sample() {
    SoilData soilData = SoilData();

    for (int i = 0; i < TOTAL_DATA; i++) {
        soilData.nitrogen = get_data(REG_NITRO);
        soilData.phosphorus = get_data(REG_PHOS);
        soilData.kalium = get_data(REG_KALI);
        soilData.pH = get_data(REG_PH) / (float) 100;
        soilData.temperature = get_data(REG_TEMP) / (float) 10;
        soilData.humidity = get_data(REG_HUM) / (float) 10;
        soilData.EC = get_data(REG_EC);
        
    }

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
    soilData.salt /= NUM_SAMPLES;

    soilData.nitrogen = (soilData.nitrogen + N_b) / N_a;
    soilData.phosphorus = (soilData.phosphorus + P_b) / P_a;
    soilData.kalium = (soilData.kalium + K_b) / K_a;

    return soilData;
}
// ------------------------- Probe Default (Aliexpress) ---------------------------
Probe_new::Probe_new(int rx, int tx, int HWSerialNum, int addr)
        : Probe(rx, tx, HWSerialNum, addr)
{
    probe.begin(PROBE_BAUDRATE, SERIAL_8N1, rx, tx);
}

SoilData Probe_new::sample() {
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
        soilData.pH += getResponseBuffer(INDEX_PH) / (float) 100;
        soilData.temperature += getResponseBuffer(INDEX_TEMP) / (float) 10;
        soilData.humidity += getResponseBuffer(INDEX_HUM) / (float) 10;
        soilData.EC += getResponseBuffer(INDEX_EC);
        soilData.salt += getResponseBuffer(INDEX_SALT);
    }

    soilData.nitrogen /= NUM_SAMPLES;
    soilData.phosphorus /= NUM_SAMPLES;
    soilData.kalium /= NUM_SAMPLES;
    soilData.pH /= NUM_SAMPLES;
    soilData.temperature /= NUM_SAMPLES;
    soilData.humidity /= NUM_SAMPLES;
    soilData.EC /= NUM_SAMPLES;
    soilData.salt /= NUM_SAMPLES;

    soilData.nitrogen = (soilData.nitrogen + N_b) / N_a;
    soilData.phosphorus = (soilData.phosphorus + P_b) / P_a;
    soilData.kalium = (soilData.kalium + K_b) / K_a;

    return soilData;
}


