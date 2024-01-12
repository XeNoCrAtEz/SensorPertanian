#ifndef __RTC_MODULE_H__
#define __RTC_MODULE_H__


#include <Arduino.h>
#include <RtcDS1302.h>


class RTC {
private:
    ThreeWire m_RTCWire; // IO, SCLK, CE
    RtcDS1302<ThreeWire> m_RTC;
    bool ready = false;

public:
    RTC(uint8_t dataPin, uint8_t clkPin, uint8_t rstPin);
    RtcDateTime get_date_time();
    void set_date_time(
        uint16_t year,
        uint8_t month,
        uint8_t dayOfMonth,
        uint8_t hour,
        uint8_t minute,
        uint8_t second
    );
    void set_date_time (const RtcDateTime& dateTime);
    bool is_ready();


};


String RtcDateTime_to_Str(const RtcDateTime& dt);


#endif