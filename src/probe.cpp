#include "probe.h"

HardwareSerial probe(1);        // Hardware serial on Serial1

SoilData::SoilData(int HWSerialNum)
    : probe(HWSerialNum) {
}

void SoilData::sample() {
    for (int i = 0; i < NUM_SAMPLES; i++) {
        nitrogen += get_data(nitro);
        phosphorus += get_data(phos);
        kalium += get_data(kali);
        pH += get_data(ph) / (float) 100;
        temperature += get_data(temp) / (float) 10;
        humidity += get_data(hum) / (float) 10;
        EC += get_data(ec);
    }

    nitrogen /=  NUM_SAMPLES;
    phosphorus /=  NUM_SAMPLES;
    kalium /=  NUM_SAMPLES;
    pH /=  NUM_SAMPLES;
    temperature /=  NUM_SAMPLES;
    humidity /=  NUM_SAMPLES;
    EC /=  NUM_SAMPLES;
}


int SoilData::get_data(const byte code[]) {
    // begin Serial for NPK Probe
    probe.begin(PROBE_BAUDRATE, SERIAL_8N1, PROBE_RX, PROBE_TX);

#ifdef DEBUG
    // tampilkan request bytes
    Serial.print("Sent bytes: ");
    for (byte i = 0; i < CODE_SIZE; i++) {
        Serial.print(code[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
#endif
    
    // tunggu respon data dri probe
    int resendCount = 0;
    bool resend = true;
    do {
        if(!send_data_req(code)) return 0;
        
        unsigned long startTime = millis();
        while(millis() - startTime <= TIMEOUT) {
            if (probe.available() >= RESPONSE_SIZE) {
                resend = false;
                break;
            }
        }

        if (resendCount >= MAX_RESEND) {
            Serial.println("Error! Probe not responding");
            return 0;
        }
        
        resendCount++;
    } while(resend);
    
    byte probeOutputBuffer[RESPONSE_SIZE];
    
    // read data
    for (byte i = 0; i < RESPONSE_SIZE; i++)
        probeOutputBuffer[i] = probe.read();

#ifdef DEBUG
    // tampilkan respond bytes
    Serial.print("Received bytes: ");
    for (byte i = 0; i < RESPONSE_SIZE; i++) {
        Serial.print(probeOutputBuffer[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
#endif

    probe.end();

    // gabungkan byte-3 (value_L) dan byte-4 (value_H)
    return (probeOutputBuffer[3] << 8) + probeOutputBuffer[4];
}


bool SoilData::send_data_req(const byte code[]) {
    if (probe.write(code, CODE_SIZE) != CODE_SIZE) {
        Serial.println("Error! Data request sent is not in the same amount");
        return false;
    }

    return true;
}
