#include "timeClass.h"


TimeClass::TimeClass(RTC& rtc, Submitter& submitter) 
        : m_rtc(rtc), m_submitter(submitter) {
    m_RTCAvailable = rtc.is_ready();
    m_NTPAvailable = submitter.is_time_available();
    
    if (m_NTPAvailable && m_RTCAvailable) update_RTC();
}


TimeClass::ErrorCodes TimeClass::update_RTC() {
    if (!m_RTCAvailable) return RTC_UNAVAILABLE;
    else if (!m_NTPAvailable) return NTP_UNAVAILABLE;

    m_rtc.set_date_time(m_submitter.get_current_time());
    return SUCCESS;
}


RtcDateTime TimeClass::get_date_time() {
    if (m_RTCAvailable) return m_rtc.get_date_time();
    else if (m_NTPAvailable) return m_submitter.get_current_time();
    else return RtcDateTime();
}


TimeClass::ErrorCodes TimeClass::availability() {
    if (m_NTPAvailable && m_RTCAvailable) return SUCCESS;
    else if (!m_NTPAvailable) return NTP_UNAVAILABLE;
    else if (!m_RTCAvailable) return RTC_UNAVAILABLE;
    else return NO_TIME;
}