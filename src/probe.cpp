#include "probe.h"

HardwareSerial probe(1);        // Hardware serial on Serial1


// SoilData method definitions
SoilData::SoilData(
        int nitro, int phos, int kali,
        float ph, float temp, float hum,
        int ec
    ) {
    nitrogen = nitro;
    phosphorus = phos;
    kalium = kali;
    pH = ph;
    temperature = temp;
    humidity = hum;
    EC = ec;
}



void sample(
        int& nitrogen, int& phosphorus, int& kalium,
        float& pH, float& temperature, float& humidity,
        int& EC
    ) {
    const byte NUM_SAMPLES = 10;

    int nitrogen_sampled = 0;
    int phosphorus_sampled = 0;
    int kalium_sampled = 0;
    float pH_sampled = 0;
    float temperature_sampled = 0;
    float humidity_sampled = 0;
    int EC_sampled = 0;

    for (int i = 0; i < NUM_SAMPLES; i++) {
        nitrogen_sampled += get_data(nitro);
        phosphorus_sampled += get_data(phos);
        kalium_sampled += get_data(kali);
        pH_sampled += get_data(ph) / (float) 100;
        temperature_sampled += get_data(temp) / (float) 10;
        humidity_sampled += get_data(hum) / (float) 10;
        EC_sampled += get_data(ec);
    }

    nitrogen = (nitrogen + (nitrogen_sampled / NUM_SAMPLES)) / 2;
    phosphorus = (phosphorus + (phosphorus_sampled / NUM_SAMPLES)) / 2;
    kalium = (kalium + (kalium_sampled / NUM_SAMPLES)) / 2;
    pH = (pH + (pH_sampled / NUM_SAMPLES)) / 2;
    temperature = (temperature + (temperature_sampled / NUM_SAMPLES)) / 2;
    humidity = (humidity + (humidity_sampled / NUM_SAMPLES)) / 2;
    EC = (EC + (EC_sampled / NUM_SAMPLES)) / 2;
}


int get_data(const byte code[]) {
    // begin Serial for NPK Probe
    probe.begin(PROBE_SERIAL_BAUDRATE, SERIAL_8N1, PROBE_RX, PROBE_TX);

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
    const int TIMEOUT = 50;
    const int MAX_RESEND = 10;
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


bool send_data_req(const byte code[]) {
    if (probe.write(code, CODE_SIZE) != CODE_SIZE) {
        Serial.println("Error! Data request sent is not in the same amount");
        return false;
    }

    return true;
}
