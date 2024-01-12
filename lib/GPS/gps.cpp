#include "gps.h"


GPS::GPS(uint8_t rx, uint8_t tx, uint8_t HWSerialNum)
        : m_serial(HWSerialNum) {
    m_serial.begin(BAUDRATE, SERIAL_8N1, rx, tx);

    m_status = get_location_till_timeout();
}


GPS::OpStatus GPS::get_location(double& lat, double& lng) {
    lat = lng = 0;

    if (status() == GPS_FAILED) return STATUS_ERROR;
    if (status() == GPS_NO_FIX) return STATUS_NO_FIX;
    
    lat = m_gps.location.lat();
    lng = m_gps.location.lng();

    return SUCCESS;
}


GPS::Status GPS::status() {
    return m_status;
}


GPS::Status GPS::get_location_till_timeout() {
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
    
    if (isCharsProcessed && isAvailable) return GPS_FIX;
    else if (isCharsProcessed && !isAvailable) return GPS_NO_FIX;
    else return GPS_FAILED;
}