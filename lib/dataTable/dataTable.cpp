#include "dataTable.h"
#include "limits.h"


SoilDataTable::SoilDataTable(uint8_t sck, uint8_t mosi, uint8_t miso, uint8_t cs)
        : idx_in{-1}, idx_out{-1}, count{0},
          PIN_SCK{sck}, PIN_MOSI{mosi}, PIN_MISO{miso}, PIN_CS{cs}
{
    flash_SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_CS);
    flash.begin();

    read_table_params();
}


bool SoilDataTable::push(const SoilReading& soilReading) {
    inc_idx(idx_in);
    if (is_empty()) {
        inc_idx(idx_out);
        count++;
    } else if (!is_empty() && !is_full()) {
        count++;
    } else if (is_full()) {
        inc_idx(idx_out);
    }

    write_table_params();
    flash.writeAnything(getAddress(idx_in), soilReading);
    
    return true;
}


bool SoilDataTable::push(const SoilData& soilData, const unsigned long epoch) {
    return push(SoilReading(soilData, epoch));
}


bool SoilDataTable::pop(SoilReading& soilReading) {
    if (is_empty()) return false;
    else {
        flash.readAnything(getAddress(idx_out), soilReading);
        
        if (count == 1) {
            clear();
        } else {
            count--;
            inc_idx(idx_out);
        }
    }

    write_table_params();
    return true;
}


bool SoilDataTable::pop(SoilData& soilData, unsigned long& epoch) {
    SoilReading reading = SoilReading();
    if(!pop(reading)) return false;

    soilData = reading.soilData;
    epoch = reading.epoch;
    
    return true;
}


bool SoilDataTable::is_empty() {
    return (!count) ? true : false;
}


bool SoilDataTable::is_full() {
    return (count == MAX_ROWS) ? true : false;
}


int SoilDataTable::get_count() {
    return count;
}


void SoilDataTable::clear() {
    idx_in = -1;
    idx_out = -1;
    count = 0;

    write_table_params();
}


void SoilDataTable::inc_idx(int& idx) {
    idx = (idx+1) % MAX_ROWS;
}


void SoilDataTable::dec_idx(int& idx) {
    idx = (idx-1) % MAX_ROWS;
}


uint32_t SoilDataTable::getAddress(int idx) {
    return ADDR_DATA_START + (idx * sizeof(SoilReading));
}


void SoilDataTable::write_table_params() {
    flash.writeLong(ADDR_COUNT, count);
    flash.writeLong(ADDR_IDX_IN, idx_in);
    flash.writeLong(ADDR_IDX_OUT, idx_out);
}


void SoilDataTable::read_table_params() {
    count = flash.readLong(ADDR_COUNT);
    idx_in = flash.readLong(ADDR_IDX_IN);
    idx_out = flash.readLong(ADDR_IDX_OUT);
}