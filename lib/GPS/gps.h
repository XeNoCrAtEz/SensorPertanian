#include <Arduino.h>
#include <TinyGPSPlus.h>


class GPS {
private:
    enum GPSParams {
        TIMEOUT = 2000,   // ms
    };


public:
    enum ErrorCodes {
        GPS_FIX,
        GPS_NO_FIX,
        GPS_FAILED,
    };


private:
    const uint8_t RX_PIN;
    const uint8_t TX_PIN;
    const uint16_t BAUDRATE;

    double lat;
    double lng;

    TinyGPSPlus _gps;
    HardwareSerial _serial;


public:
    GPS(uint16_t baudrate=9600, uint8_t rx=19, uint8_t tx=18, uint8_t HWSerialNum=1);

    bool get_location(double& lat, double& lng);
    uint8_t get_location_till_timeout();
};