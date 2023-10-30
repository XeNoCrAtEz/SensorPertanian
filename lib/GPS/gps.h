#include <Arduino.h>
#include <TinyGPSPlus.h>

const uint8_t GPS_FIX = 0x00;
const uint8_t GPS_NO_FIX = 0x01;
const uint8_t GPS_FAILED = 0x02;

class GPS {
private:
    static const uint16_t TIMEOUT = 2000;   // ms

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