#include "timeClass.h"


TimeClass::TimeClass(RTC& rtc, Submitter& submitter) 
        : rtc(rtc), submitter(submitter) {
    RTCAvailable = rtc.is_ready();
    NTPAvailable = submitter.is_time_available();
    
    if (NTPAvailable && RTCAvailable) update_RTC();
}


TimeClass::ErrorCodes TimeClass::update_RTC() {
    if (!RTCAvailable) return RTC_UNAVAILABLE;
    else if (!NTPAvailable) return NTP_UNAVAILABLE;

    rtc.set_date_time(submitter.get_current_time());
    return SUCCESS;
}


RtcDateTime TimeClass::get_date_time() {
    if (RTCAvailable) return rtc.get_date_time();
    else if (NTPAvailable) return submitter.get_current_time();
    else return RtcDateTime();
}


TimeClass::ErrorCodes TimeClass::availability() {
    if (NTPAvailable && RTCAvailable) return SUCCESS;
    else if (!NTPAvailable) return NTP_UNAVAILABLE;
    else if (!RTCAvailable) return RTC_UNAVAILABLE;
    else return NO_TIME;
}