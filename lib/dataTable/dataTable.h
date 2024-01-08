#ifndef _DATA_TABLE_H_
#define _DATA_TABLE_H_

#include <Arduino.h>
#include <LittleFS.h>

#include "soil_data.h"



class SoilReading {
public:
    uint32_t epoch;
    SoilData soilData;


    SoilReading()
            : soilData{}, epoch{0} {}
    SoilReading(const SoilData& data, const uint32_t& epch)
            : soilData{data}, epoch{epch} { }

    bool operator==(const SoilReading& s2) {
        return soilData == s2.soilData &&
                epoch == s2.epoch;
    }
};


class SoilDataTable {
public:
    // ERROR CODES
    enum ErrorCodes {
        SUCCESS,
        OPEN_FAILED,
        WRITE_FAILED,
        READ_FAILED,
        EMPTY_FILE,
        LITTLEFS_FAILED,
        UNKNOWN_ERROR,
    };


private:
    enum SoilDataTableParams {
        MAX_COUNT = 10000,
    };


private:
    static const char filename[];
    fs::LittleFSFS filesystem = LittleFS;

    bool ready = false;


public:
    SoilDataTable();
    ErrorCodes push(const SoilReading& soilReading);
    ErrorCodes pop(SoilReading& soilReading);
    ErrorCodes pop_all(SoilReading* &soilReadings, uint16_t& size);
    ErrorCodes clear();
    uint32_t get_count();
    bool is_empty();
    bool is_full();
    bool is_ready();
};


#endif