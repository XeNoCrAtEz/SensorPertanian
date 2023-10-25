#ifndef _DATA_TABLE_H_
#define _DATA_TABLE_H_

#include <Arduino.h>

#include "soil_data.h"
#include <SPIMemory.h>

class SoilReading {
public:
    uint32_t epoch;
    SoilData soilData;

    SoilReading() = default;
    SoilReading(const SoilData& data, const uint32_t& epch) {
        epoch = epch;
        soilData = soilData;
    }
    bool operator==(const SoilReading& other) {
        return soilData == other.soilData && epoch == other.epoch;
    }
};


class SoilDataTable {
private:
    const uint32_t MAX_ROWS = (MB(4) - 12) / sizeof(SoilReading);

    const uint8_t PIN_SCK;
    const uint8_t PIN_MOSI;
    const uint8_t PIN_MISO;
    const uint8_t PIN_CS;

    const uint8_t ADDR_COUNT = 0;
    const uint8_t ADDR_IDX_IN = 4;
    const uint8_t ADDR_IDX_OUT = 8;
    const uint8_t ADDR_DATA_START = 12;

    const uint8_t SOILDATA_OFFSET = sizeof(SoilReading);

    SPIClass flash_SPI;
    SPIFlash flash = SPIFlash(PIN_CS, &flash_SPI);

    int32_t idx_in;
    int32_t idx_out;
    int32_t count;

    void write_table_params();
    void read_table_params();
public:
    SoilDataTable(uint8_t sck=SCK, uint8_t mosi=MOSI, uint8_t miso=MISO, uint8_t cs=SS);
    bool push(const SoilData& soilData, const unsigned long epoch);
    bool push(const SoilReading& soilReading);
    bool pop(SoilData& soilData, unsigned long& epoch);
    bool pop(SoilReading& soilReading);
    bool is_empty();
    bool is_full();
    int get_count();
    void clear();
    void inc_idx(int& idx);
    void dec_idx(int& idx);
    uint32_t getAddress(int idx);
};


#endif