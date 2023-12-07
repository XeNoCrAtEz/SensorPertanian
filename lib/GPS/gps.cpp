#include "gps.h"


GPS::GPS(uint8_t rx, uint8_t tx, uint8_t HWSerialNum)
        : _serial{HWSerialNum} {
    _serial.begin(BAUDRATE, SERIAL_8N1, rx, tx);
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
    
    lat = _gps.location.lat();
    lng = _gps.location.lng();

    return true;
}


uint8_t GPS::get_location_till_timeout() {
    uint32_t start = millis();
    bool isCharsProcessed = false;
    bool isAvailable = false;
    while ((millis() - start <  TIMEOUT) && (!isAvailable || !isCharsProcessed)) {
        if (_serial.available() > 0) {
            _gps.encode(_serial.read());

            isCharsProcessed = _gps.charsProcessed() > 10;
            isAvailable = _gps.location.isValid() || _gps.location.isUpdated();
        }
    }
    
    if (isCharsProcessed && isAvailable) return GPS_FIX;
    else if (isCharsProcessed && !isAvailable) return GPS_NO_FIX;
    else return GPS_FAILED;
}