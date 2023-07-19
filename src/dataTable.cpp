#include "dataTable.h"


bool SoilDataTable::push(const SoilData& soilData) {
	// readings[idx_in].timestamp = get_time_stamp();
    readings[idx_in].soilData = soilData;

	inc_idx(idx_in);

	if (!is_full())	count++;        // Increase records count
	else inc_idx(idx_out);          // if queue is full, overwrite oldest record, increment idx_out

	return true;
}


bool SoilDataTable::pop(SoilData& soilData) {
    if (is_empty())	return false;   // No more records

    inc_idx(idx_out);

    soilData = readings[idx_out].soilData;

    count--;

    return true;
}


inline bool SoilDataTable::is_empty() {
    return (!count) ? true : false;
}


inline bool SoilDataTable::is_full() {
    return (count == MAX_SIZE) ? true : false;
}


inline int SoilDataTable::get_count() {
    return count;
}


inline void SoilDataTable::clear() {
    idx_in = 0;
    idx_out = 0;
    count = 0;
}


inline void SoilDataTable::inc_idx(int& idx) {
    idx = idx+1 % MAX_SIZE;
}


inline void SoilDataTable::dec_idx(int& idx) {
    idx = idx-1 % MAX_SIZE;
}