#include "gps.h"


GPS::GPS(uint8_t rx, uint8_t tx, uint8_t HWSerialNum)
        : m_serial(HWSerialNum) {
    m_serial.begin(BAUDRATE, SERIAL_8N1, rx, tx);

    if (m_serial.available() > 0) m_status = READY;
    else m_status = NO_GPS;
}


GPS::OpStatus GPS::get_location(double& lat, double& lng) {
    lat = lng = 0;

    OpStatus errCode = get_location_till_timeout();
    if (errCode != SUCCESS) return errCode;
    
    lat = m_gps.location.lat();
    lng = m_gps.location.lng();

    return SUCCESS;
}


GPS::Status GPS::status() {
    return m_status;
}


GPS::OpStatus GPS::get_location_till_timeout() {
    if (status() != READY) return STATUS_ERROR;

    uint32_t start = millis();
    bool isCharsProcessed = false;
    bool isAvailable = false;
    while ((millis() - start <  GPS_TIMEOUT) && (!isAvailable || !isCharsProcessed)) {
        if (m_serial.available() > 0) {
            m_gps.encode(m_serial.read());

            isCharsProcessed = m_gps.charsProcessed() > 10;
            isAvailable = m_gps.location.isValid() || m_gps.location.isUpdated();
        }
    }
    
    if (isCharsProcessed && !isAvailable) return STATUS_NO_FIX;
    else return STATUS_ERROR;
}


void GPS::begin(int rx, int tx) {
    m_serial.begin(BAUDRATE, SERIAL_8N1, rx, tx);
}


void GPS::end() {
    m_serial.end();
}