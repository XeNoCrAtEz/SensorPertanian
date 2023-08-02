#include "dataTable.h"


SoilDataTable::SoilDataTable()
        : idx_in{-1}, idx_out{-1}, count{0}
{
}


bool SoilDataTable::push(const SoilData& soilData, const unsigned long epoch) {
    inc_idx(idx_in);
    if (is_empty()) {
        inc_idx(idx_out);
        count++;
    } else if (!is_empty() && !is_full()) {
        count++;
    } else if (is_full()) {
        inc_idx(idx_out);
    }

    readings[idx_in].soilData = soilData;
    readings[idx_in].epoch = epoch;

    return true;
}


bool SoilDataTable::push(const SoilReading& soilReading) {
    return push(soilReading.soilData, soilReading.epoch);
}


bool SoilDataTable::pop(SoilData& soilData, unsigned long& epoch) {
    if (is_empty()) return false;
    else {
        soilData = readings[idx_out].soilData;
        epoch = readings[idx_out].epoch;
        
        if (count == 1) {
            clear();
        } else {
            count--;
            inc_idx(idx_out);
        }
    }
    return true;
}


bool SoilDataTable::pop(SoilReading& soilReading) {
    return pop(soilReading.soilData, soilReading.epoch);
}


bool SoilDataTable::is_empty() {
    return (!count) ? true : false;
}


bool SoilDataTable::is_full() {
    return (count == MAX_SIZE) ? true : false;
}


int SoilDataTable::get_count() {
    return count;
}


void SoilDataTable::clear() {
    idx_in = -1;
    idx_out = -1;
    count = 0;
}


void SoilDataTable::inc_idx(int& idx) {
    idx = (idx+1) % MAX_SIZE;
}


void SoilDataTable::dec_idx(int& idx) {
    idx = (idx-1) % MAX_SIZE;
}