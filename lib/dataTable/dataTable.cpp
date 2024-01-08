#include "dataTable.h"


const char SoilDataTable::filename[17] = "/soilReading.bin";


SoilDataTable::SoilDataTable() {
    if (!filesystem.begin()) {
        Serial.println("LittleFS Mount Failed!");
        return;
    }

    File file = filesystem.open(filename, FILE_READ, true);
    if (!file) Serial.println("File not found! \"soilReading.bin\" created!");

    ready = true;
}


bool SoilDataTable::is_ready() {
    return ready;
}


SoilDataTable::ErrorCodes SoilDataTable::push(const SoilReading &soilReading) {
    if (!is_ready()) return LITTLEFS_FAILED;

    File file = filesystem.open(filename, FILE_APPEND);
    if (!file) return OPEN_FAILED;

    if (!file.write(reinterpret_cast<const uint8_t*>(&soilReading), sizeof(SoilReading))) return WRITE_FAILED;
    
    return SUCCESS;
}


SoilDataTable::ErrorCodes SoilDataTable::pop(SoilReading& soilReading) {
    if (!is_ready()) return LITTLEFS_FAILED;

    File file = filesystem.open(filename, FILE_READ);
    if (!file) return OPEN_FAILED;

    if (is_empty()) return EMPTY_FILE;

    if (!file.read(reinterpret_cast<uint8_t*>(&soilReading), sizeof(SoilReading))) return READ_FAILED;
    
    // TODO: delete entry when done popping

    return SUCCESS;
}


SoilDataTable::ErrorCodes SoilDataTable::pop_all(SoilReading* &soilReadings, uint16_t& count) {
    if (!is_ready()) return LITTLEFS_FAILED;
    
    File file = filesystem.open(filename, FILE_READ);
    if (!file) return OPEN_FAILED;

    if (is_empty()) return EMPTY_FILE;

    count = get_count();
    soilReadings = new SoilReading[count];

    for (uint16_t i = 0; i < count; i++)
        if (!file.read(reinterpret_cast<uint8_t*>(&soilReadings[i]), sizeof(SoilReading))) return READ_FAILED;

    clear();

    return SUCCESS;
}


uint32_t SoilDataTable::get_count() {
    if (!is_ready()) return LITTLEFS_FAILED;

    File file = filesystem.open(filename, FILE_READ);
    if (!file) return 0;

    return file.size() / sizeof(SoilReading);
}


SoilDataTable::ErrorCodes SoilDataTable::clear() {
    if (!is_ready()) return LITTLEFS_FAILED;
    
    File file = filesystem.open(filename, FILE_WRITE);
    if (!file) return OPEN_FAILED;

    return SUCCESS;
}


bool SoilDataTable::is_empty() {
    if (!is_ready()) return LITTLEFS_FAILED;

    File file = filesystem.open(filename, FILE_READ);
    if (!file) return false;

    if (file.size() == 0) return true;
    
    return false;
}


bool SoilDataTable::is_full() {
    if (!is_ready()) return LITTLEFS_FAILED;
    
    File file = filesystem.open(filename, FILE_READ);
    if (!file) return false;

    if (get_count() >= MAX_COUNT) return true;

    return false;
}
