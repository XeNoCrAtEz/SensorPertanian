#include "probe.h"


Probe::Probe(int HWSerialNum, int addr)
        : ModbusMaster(), m_probe(HWSerialNum) {
    begin(addr, m_probe);

    // m_status = check_status();       // TODO: implement this method
}


// get a single register, use register number constants specified by the probe
Probe::OpStatus Probe::get_data(uint16_t& data, int regNum) {
    if (status() == NO_PROBE) return STATUS_NO_PROBE;
    if (status() != READY) return STATUS_ERROR;
    
    for (int i = 0; i < NUM_SAMPLES; i++) {
        int resultCode = readHoldingRegisters(regNum, 0x01);     // read one register only
        
        for(int attempts = 0; resultCode == ku8MBResponseTimedOut; attempts++) {
            Serial.println("Probe not responding!");
            if (attempts >= MAX_RESEND) {
                Serial.println("Fatal Error! Probe is not connected");
                return STATUS_NO_PROBE;
            }
            Serial.print("Resending... (resend attempt: ");
            Serial.print(attempts);
            Serial.println(")");
            resultCode = readHoldingRegisters(regNum, 0x01);
        }

        if(resultCode != ku8MBSuccess) {
            Serial.print("Probe Error! Result code: ");
            Serial.print(resultCode);
            Serial.println("");

            return STATUS_ERROR;
        }

        data += getResponseBuffer(0);
    }

#ifdef DEBUG
    // tampilkan respond bytes
    Serial.print("Received bytes: ");
    Serial.println(getResponseBuffer(0x00), HEX);
#endif

    data /= NUM_SAMPLES;
    
    return SUCCESS;
}


#ifndef NO_CALIB
void Probe::calibrateNPK(SoilData &soilData) {
    int16_t calibResult = (soilData.nitrogen + NITR_CALIB_B) / NITR_CALIB_A;
    soilData.nitrogen = calibResult < 0 ? 0 : calibResult;

    calibResult = (soilData.phosphorus + PHOS_CALIB_B) / PHOS_CALIB_A;
    soilData.phosphorus = calibResult < 0 ? 0 : calibResult;

    calibResult = (soilData.kalium + KALI_CALIB_B) / KALI_CALIB_A;
    soilData.kalium = calibResult < 0 ? 0 : calibResult;
}
#endif


Probe::Status Probe::status() {
    return m_status;
}


// ---------------------------- Probe KHDTK ------------------------------
ProbeKHDTK::ProbeKHDTK(int rx, int tx, int HWSerialNum, int addr)
        : Probe(HWSerialNum, addr) {
    m_probe.begin(PROBE_BAUDRATE, SERIAL_8N1, rx, tx);

    // m_status = check_status();
}


Probe::OpStatus ProbeKHDTK::sample(SoilData& soilData) {
    // if (status() == NO_PROBE) return STATUS_NO_PROBE;        // TODO: implement to enable this writing
    // if (status() != READY) return STATUS_ERROR;

    if (get_data(soilData.nitrogen, REG_NITRO) != SUCCESS) return STATUS_ERROR;
    if (get_data(soilData.phosphorus, REG_PHOS) != SUCCESS) return STATUS_ERROR;
    if (get_data(soilData.kalium, REG_KALI) != SUCCESS) return STATUS_ERROR;

    uint16_t temp_pH = 0;
    if (get_data(temp_pH, REG_PH) != SUCCESS) return STATUS_ERROR;
    soilData.pH = temp_pH / (float) 100;

    uint16_t temp_temperature = 0;
    if (get_data(temp_temperature, REG_TEMP) != SUCCESS) return STATUS_ERROR;
    soilData.temperature = temp_temperature / (float) 10;
    
    uint16_t temp_humidity = 0;
    if (get_data(temp_humidity, REG_HUM) != SUCCESS) return STATUS_ERROR;  
    soilData.humidity = temp_humidity / (float) 10;
    
    if (get_data(soilData.EC, REG_EC) != SUCCESS) return STATUS_ERROR;

#ifndef NO_CALIB
    calibrateNPK(soilData);
#endif

    return SUCCESS;
}


// ------------------------- Probe Default (Aliexpress) ---------------------------
ProbeDefault::ProbeDefault(int rx, int tx, int HWSerialNum, int addr)
        : Probe(HWSerialNum, addr) {
    m_probe.begin(PROBE_BAUDRATE, SERIAL_8N1, rx, tx);
}


ProbeDefault::OpStatus ProbeDefault::sample(SoilData& soilData) {
    for (int i = 0; i < NUM_SAMPLES; i++) {
        int resultCode = readHoldingRegisters(0x00, TOTAL_DATA);     // read from 0x00-0x06
        
        for(int attempts = 0; resultCode == ku8MBResponseTimedOut; attempts++) {
            Serial.println("Probe not responding!");
            if (attempts >= MAX_RESEND) {
                Serial.println("Fatal Error! Probe is not connected");
                return STATUS_NO_PROBE;
            }
            Serial.print("Resending... (resend attempt: ");
            Serial.print(attempts);
            Serial.println(")");
            resultCode = readHoldingRegisters(0x00, TOTAL_DATA);
        }

        if(resultCode != ku8MBSuccess) {
            Serial.print("Probe Error! Result code: ");
            Serial.print(resultCode);
            Serial.println("");

            return STATUS_ERROR;
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

#ifndef NO_CALIB
    calibrateNPK(soilData);
#endif

    return SUCCESS;
}


// ------------------------- Probe New (Mas Nando) ---------------------------
ProbeNew::ProbeNew(int rx, int tx, int HWSerialNum, int addr)
        : Probe(HWSerialNum, addr) {
    m_probe.begin(PROBE_BAUDRATE, SERIAL_8N1, rx, tx);
}


ProbeNew::OpStatus ProbeNew::sample(SoilData& soilData) {
    for (int i = 0; i < NUM_SAMPLES; i++) {
        int resultCode = readHoldingRegisters(0x00, TOTAL_DATA);     // read from 0x00-0x07
        
        for(int attempts = 0; resultCode == ku8MBResponseTimedOut; attempts++) {
            Serial.println("Probe not responding!");
            if (attempts >= MAX_RESEND) {
                Serial.println("Fatal Error! Probe is not connected");
                return STATUS_NO_PROBE;
            }
            Serial.print("Resending... (resend attempt: ");
            Serial.print(attempts);
            Serial.println(")");
            resultCode = readHoldingRegisters(0x00, TOTAL_DATA);
        }

        if(resultCode != ku8MBSuccess) {
            Serial.print("Probe Error! Result code: ");
            Serial.print(resultCode);
            Serial.println("");

            return STATUS_ERROR;
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

#ifndef NO_CALIB
    calibrateNPK(soilData);
#endif

    return SUCCESS;
}