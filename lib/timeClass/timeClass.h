#ifndef __TIME_CLASS_H__
#define __TIME_CLASS_H__


#include <Arduino.h>
#include <ESP32Time.h>
#include "submitter.h"
#include "RTCModule.h"


class TimeClass : public ESP32Time {
public:
    // class status codes
    enum Status {
        READY,
        READY_NO_RTC,
        READY_NO_NTP,
        NO_TIME,
        UNKNOWN_ERROR,
    };

    // operation status codes
    enum OpStatus {
        SUCCESS,
        STATUS_NO_RTC,
        STATUS_NO_NTP,
        STATUS_NO_TIME,
        STATUS_ERROR,
    };


private:
    enum TimeClassParams {
        TIMEZONE_HOUR_OFFSET = 7,
        TIMEZONE_OFFSET = TIMEZONE_HOUR_OFFSET * 3600,
        YEAR_OFFSET = 1900,
        MONTH_OFFSET = 1,
    };


private:
    RTC& m_rtc;
    Submitter& m_submitter;
    
    Status m_status = UNKNOWN_ERROR;
    bool m_RTCAvailable=false, m_NTPAvailable=false;


public:
    TimeClass(RTC& rtc, Submitter& submitter);
    OpStatus update_RTC();
    OpStatus get_date_time(RtcDateTime& time);
    Status status();


};


#endif