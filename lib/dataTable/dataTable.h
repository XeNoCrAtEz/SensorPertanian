#ifndef _DATA_TABLE_H_
#define _DATA_TABLE_H_

#include <Arduino.h>
#include <LittleFS.h>

#include "soil_data.h"

// ERROR CODES
const uint8_t SUCCESS = 0x00;
const uint8_t OPEN_FAILED = 0x01;
const uint8_t WRITE_FAILED = 0x02;
const uint8_t READ_FAILED = 0x03;
const uint8_t EMPTY_FILE = 0x04;
const uint8_t LITTLEFS_FAILED = 0xFE;
const uint8_t UNKNOWN_ERROR = 0xFF;

class SoilReading {
public:
    uint32_t epoch;
    SoilData soilData;


    SoilReading() = default;
    SoilReading(const SoilData& data, const uint32_t& epch)
            : soilData(data), epoch(epch) { }
    uint32_t get_epoch() { return epoch; }
    SoilData get_soilData() { return soilData; }
};


class SoilDataTable {
private:
    const char filename[17] = "/soilReading.bin";
    fs::LittleFSFS filesystem = LittleFS;


public:
    SoilDataTable();
    uint8_t push(const SoilReading& soilReading);
    uint8_t pop(SoilReading& soilReading);
    uint8_t pop_all(SoilReading* &soilReadings, uint32_t& size);
    uint32_t get_count();
    void clear();
    bool is_empty();
    bool is_full();
};


#endif