#ifndef _DATA_TABLE_H_
#define _DATA_TABLE_H_

#include <Arduino.h>
#include <LittleFS.h>

#include "soil_data.h"



class SoilReading {
public:
    uint32_t m_epoch;         // seconds from 2000
    SoilData m_soilData;


    SoilReading()
            : m_soilData{}, m_epoch{0} {}
    SoilReading(const SoilData& data, const uint32_t& epch)
            : m_soilData{data}, m_epoch{epch} { }

    bool operator==(const SoilReading& s2) {
        return m_soilData == s2.m_soilData &&
                m_epoch == s2.m_epoch;
    }
};


class SoilDataTable {
public:
    // class status codes
    enum Status {
        READY,
        LITTLEFS_FAILED,
        UNKNOWN_ERROR,
    };
    
    // Operation status codes
    enum OpStatus {
        SUCCESS,
        OPEN_FAILED,
        WRITE_FAILED,
        READ_FAILED,
        EMPTY_FILE,
        STATUS_ERROR,
    };


private:
    enum SoilDataTableParams {
        MAX_COUNT = 10000,
    };


private:
    static const char m_filename[];
    fs::LittleFSFS& m_filesystem = LittleFS;

    Status m_status = UNKNOWN_ERROR;


public:
    SoilDataTable();
    OpStatus push(const SoilReading& soilReading);
    OpStatus pop(SoilReading& soilReading);
    OpStatus pop_all(SoilReading* &soilReadings, uint16_t& size);
    OpStatus clear();
    uint32_t get_count();
    bool is_empty();
    bool is_full();
    Status status();
};


#endif