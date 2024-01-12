#ifndef __GPS_H__
#define __GPS_H__

#include <Arduino.h>
#include <TinyGPSPlus.h>


class GPS {
private:
    enum GPSParams {
        TIMEOUT = 2000,   // ms
        BAUDRATE = 9600
    };


public:
    enum ErrorCodes {
        GPS_FIX,
        GPS_NO_FIX,
        GPS_FAILED,
    };


private:
    double m_lat;
    double m_lng;

    TinyGPSPlus m_gps;
    HardwareSerial m_serial;


public:
    GPS(uint8_t rx, uint8_t tx, uint8_t HWSerialNum=1);

    bool get_location(double& lat, double& lng);
    uint8_t get_location_till_timeout();
};


#endif