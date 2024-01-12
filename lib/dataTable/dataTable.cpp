#include "dataTable.h"


const char SoilDataTable::m_filename[17] = "/soilReading.bin";


SoilDataTable::SoilDataTable() {
    if (!m_filesystem.begin()) {
        Serial.println("LittleFS Mount Failed!");
        m_status = LITTLEFS_FAILED;
        return;
    }

    File file = m_filesystem.open(m_filename, FILE_READ, true);
    if (!file) Serial.println("File not found! \"soilReading.bin\" created!");

    m_status = READY;
}


SoilDataTable::Status SoilDataTable::status() {
    return m_status;
}


SoilDataTable::OpStatus SoilDataTable::push(const SoilReading &soilReading) {
    if (!status()) return STATUS_ERROR;

    File file = m_filesystem.open(m_filename, FILE_APPEND);
    if (!file) return OPEN_FAILED;

    if (!file.write(reinterpret_cast<const uint8_t*>(&soilReading), sizeof(SoilReading))) return WRITE_FAILED;
    
    return SUCCESS;
}


SoilDataTable::OpStatus SoilDataTable::pop(SoilReading& soilReading) {
    if (status() != READY) return STATUS_ERROR;

    File file = m_filesystem.open(m_filename, FILE_READ);
    if (!file) return OPEN_FAILED;

    if (is_empty()) return EMPTY_FILE;

    if (!file.read(reinterpret_cast<uint8_t*>(&soilReading), sizeof(SoilReading))) return READ_FAILED;
    
    // TODO: delete entry when done popping

    return SUCCESS;
}


SoilDataTable::OpStatus SoilDataTable::pop_all(SoilReading* &soilReadings, uint16_t& count) {
    if (status() != READY) return STATUS_ERROR;
    
    File file = m_filesystem.open(m_filename, FILE_READ);
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
    if (status() != READY) return STATUS_ERROR;

    File file = m_filesystem.open(m_filename, FILE_READ);
    if (!file) return 0;

    return file.size() / sizeof(SoilReading);
}


SoilDataTable::OpStatus SoilDataTable::clear() {
    if (status() != READY) return STATUS_ERROR;
    
    File file = m_filesystem.open(m_filename, FILE_WRITE);
    if (!file) return OPEN_FAILED;

    return SUCCESS;
}


bool SoilDataTable::is_empty() {
    if (status() != READY) return STATUS_ERROR;

    File file = m_filesystem.open(m_filename, FILE_READ);
    if (!file) return false;

    if (file.size() == 0) return true;
    
    return false;
}


bool SoilDataTable::is_full() {
    if (status() != READY) return STATUS_ERROR;
    
    File file = m_filesystem.open(m_filename, FILE_READ);
    if (!file) return false;

    if (get_count() >= MAX_COUNT) return true;

    return false;
}
