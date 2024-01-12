#include "gps.h"


GPS::GPS(uint8_t rx, uint8_t tx, uint8_t HWSerialNum)
        : m_serial{HWSerialNum} {
    m_serial.begin(BAUDRATE, SERIAL_8N1, rx, tx);
}


bool GPS::get_location(double& lat, double& lng) {
    lat = lng = 0;

    uint8_t resultCode = get_location_till_timeout();

    if (resultCode == GPS_FAILED) {
        Serial.println("No GPS Connected! Please check wiring!");
        return false;
    }

    if (resultCode == GPS_NO_FIX)
        Serial.println("GPS Connected, but no fix!");
    
    lat = m_gps.location.lat();
    lng = m_gps.location.lng();

    return true;
}


uint8_t GPS::get_location_till_timeout() {
    uint32_t start = millis();
    bool isCharsProcessed = false;
    bool isAvailable = false;
    while ((millis() - start <  TIMEOUT) && (!isAvailable || !isCharsProcessed)) {
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