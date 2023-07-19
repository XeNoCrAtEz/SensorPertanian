#ifndef _DATA_TABLE_H_
#define _DATA_TABLE_H_

#include "soil_data.h"

struct SoilReading {
    // int timestamp;
    SoilData soilData;
};


class SoilDataTable {
private:
    static const int MAX_SIZE = 250;

    SoilReading readings[MAX_SIZE];
    int idx_in;
    int idx_out;
    int count;

public:
    // SoilDataTable();     uses default constructor
    // TODO: NTP server, pengambilan timestamp
    bool push(const SoilData& soilData);
    bool pop(SoilData& soilData);
    bool is_empty();
    bool is_full();
    int get_count();
    void clear();
    void inc_idx(int& idx);
    void dec_idx(int& idx);
};


#endif