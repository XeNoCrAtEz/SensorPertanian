#include "dataTable.h"


SoilDataTable::SoilDataTable() {
    if (!filesystem.begin()) {
        Serial.println("LittleFS Mount Failed!");
        return;
    }

    File file = filesystem.open(filename, FILE_READ, true);
    if (!file) return;
    else Serial.println("File not found! Creation failed!");
}


uint8_t SoilDataTable::push(const SoilReading &soilReading) {
    File file = filesystem.open(filename, FILE_APPEND);
    if (!file) return OPEN_FAILED;

    if (file.write((uint8_t*) &soilReading, sizeof(SoilReading))) return SUCCESS;
    else return WRITE_FAILED;
}


uint8_t SoilDataTable::pop(SoilReading& soilReading) {
    File file = filesystem.open(filename, FILE_READ);
    if (!file) return OPEN_FAILED;

    if (file.size() == 0) return EMPTY_FILE;

    if (file.read((uint8_t*) &soilReading, sizeof(SoilReading))) return SUCCESS;
    else return READ_FAILED;
}


uint8_t SoilDataTable::pop_all(SoilReading* &soilReadings, uint32_t& count) {
    File file = filesystem.open(filename, FILE_READ);
    if (!file) return OPEN_FAILED;

    if (file.size() == 0) return EMPTY_FILE;

    count = get_count();
    soilReadings = new SoilReading[count];

    for (uint16_t i = 0; i < count; i++) {
        if (!file.read((uint8_t*) &soilReadings[i], sizeof(SoilReading))) return READ_FAILED;
    }

    return SUCCESS;
}


uint32_t SoilDataTable::get_count() {
    File file = filesystem.open(filename, FILE_READ);
    if (!file) return OPEN_FAILED;

    return file.size() / sizeof(SoilReading);
}


void SoilDataTable::clear() {
    File file = filesystem.open(filename, FILE_WRITE);
}


bool SoilDataTable::is_empty() {
    File file = filesystem.open(filename, FILE_READ);
    if (!file) return OPEN_FAILED;

    if (file.size() == 0) return true;
    else return false;
}

bool SoilDataTable::is_full() {
    return false;
}
