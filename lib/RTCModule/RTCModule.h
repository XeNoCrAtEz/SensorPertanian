#ifndef __RTC_MODULE_H__
#define __RTC_MODULE_H__


#include <Arduino.h>
#include <RtcDS1302.h>


class RTC {
public:
    // class status codes
    enum Status {
        READY,
        NO_RTC,
        UNKNOWN_ERROR,
    };

    // operation status codes
    enum OpStatus {
        SUCCESS,
        STATUS_NO_RTC,
        STATUS_ERROR,
    };


private:
    ThreeWire m_RTCWire; // IO, SCLK, CE
    RtcDS1302<ThreeWire> m_RTC;
    Status m_status = UNKNOWN_ERROR;


public:
    RTC(uint8_t dataPin, uint8_t clkPin, uint8_t rstPin);
    OpStatus get_date_time(RtcDateTime& time);
    OpStatus set_date_time(
        uint16_t year,
        uint8_t month,
        uint8_t dayOfMonth,
        uint8_t hour,
        uint8_t minute,
        uint8_t second
    );
    OpStatus set_date_time(const RtcDateTime& dateTime);
    Status status();


};


String RtcDateTime_to_Str(const RtcDateTime& dt);


#endif