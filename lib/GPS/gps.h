#ifndef __GPS_H__
#define __GPS_H__

#include <Arduino.h>
#include <TinyGPSPlus.h>


class GPS {
private:
    enum GPSParams {
        GPS_TIMEOUT = 2000,   // ms
        BAUDRATE = 9600
    };


public:
    // class status codes
    enum Status {
        READY,
        NO_GPS,
        UNKNOWN_ERROR,
    };

    // operation status codes
    enum OpStatus {
        SUCCESS,
        STATUS_NO_FIX,
        STATUS_ERROR,
    };


private:
    TinyGPSPlus m_gps;
    HardwareSerial m_serial;

    Status m_status = UNKNOWN_ERROR;


public:
    GPS(uint8_t rx, uint8_t tx, uint8_t HWSerialNum=1);

    OpStatus get_location(double& lat, double& lng);
    OpStatus get_location_till_timeout();
    Status status();
};


#endif