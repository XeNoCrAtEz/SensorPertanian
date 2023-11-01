#include "dataTable.h"


const char SoilDataTable::filename[17] = "/soilReading.bin";


SoilDataTable::SoilDataTable() {
    if (!filesystem.begin()) {
        Serial.println("LittleFS Mount Failed!");
        return;
    }

    File file = filesystem.open(filename, FILE_READ, true);
    if (!file) Serial.println("File not found! Creation failed!");
}


SoilDataTable::ErrorCodes SoilDataTable::push(const SoilReading &soilReading) {
    File file = filesystem.open(filename, FILE_APPEND);
    if (!file) return OPEN_FAILED;

    if (!file.write((uint8_t*) &soilReading, sizeof(SoilReading))) return WRITE_FAILED;
    
    return WRITE_FAILED;
}


SoilDataTable::ErrorCodes SoilDataTable::pop(SoilReading& soilReading) {
    File file = filesystem.open(filename, FILE_READ);
    if (!file) return OPEN_FAILED;

    if (is_empty()) return EMPTY_FILE;

    if (!file.read((uint8_t*) &soilReading, sizeof(SoilReading))) return READ_FAILED;
    
    return SUCCESS;
}


SoilDataTable::ErrorCodes SoilDataTable::pop_all(SoilReading* &soilReadings, uint32_t& count) {
    File file = filesystem.open(filename, FILE_READ);
    if (!file) return OPEN_FAILED;

    if (is_empty()) return EMPTY_FILE;

    count = get_count();
    soilReadings = new SoilReading[count];

    for (uint16_t i = 0; i < count; i++)
        if (!file.read((uint8_t*) &soilReadings[i], sizeof(SoilReading))) return READ_FAILED;

    return SUCCESS;
}


uint32_t SoilDataTable::get_count() {
    File file = filesystem.open(filename, FILE_READ);
    if (!file) return 0;

    return file.size() / sizeof(SoilReading);
}


SoilDataTable::ErrorCodes SoilDataTable::clear() {
    File file = filesystem.open(filename, FILE_WRITE);
    if (!file) return OPEN_FAILED;

    return SUCCESS;
}


bool SoilDataTable::is_empty() {
    File file = filesystem.open(filename, FILE_READ);
    if (!file) return false;

    if (file.size() == 0) return true;
    
    return false;
}


bool SoilDataTable::is_full() {
    File file = filesystem.open(filename, FILE_READ);
    if (!file) return false;

    if (get_count() >= MAX_COUNT) return true;

    return false;
}
