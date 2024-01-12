#ifndef __TIME_CLASS_H__
#define __TIME_CLASS_H__


#include <Arduino.h>
#include "submitter.h"
#include "RTCModule.h"


class TimeClass {
public:
    enum ErrorCodes {
        SUCCESS,
        RTC_UNAVAILABLE,
        NTP_UNAVAILABLE,
        NO_TIME,
    };


private:
    RTC& m_rtc;
    Submitter& m_submitter;
    bool m_RTCAvailable=false, m_NTPAvailable=false;


public:
    TimeClass(RTC& rtc, Submitter& submitter);
    ErrorCodes update_RTC();
    RtcDateTime get_date_time();
    ErrorCodes availability();


};


#endif