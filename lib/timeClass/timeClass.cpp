#include "timeClass.h"


TimeClass::TimeClass(RTC& rtc, Submitter& submitter) 
        : m_rtc(rtc), m_submitter(submitter) {
    m_RTCAvailable = rtc.status();
    m_NTPAvailable = submitter.is_time_available();

    if      (!m_RTCAvailable && m_NTPAvailable)  m_status = READY_NO_RTC;
    else if (m_RTCAvailable && !m_NTPAvailable)  m_status = READY_NO_NTP;
    else if (!m_RTCAvailable && !m_NTPAvailable) m_status = NO_TIME;
    else if (m_NTPAvailable && m_RTCAvailable) {
        update_RTC();

        m_status = READY;
    }
}


TimeClass::OpStatus TimeClass::update_RTC() {
    if (status() != READY) return STATUS_ERROR;

    RtcDateTime now;
    m_submitter.get_current_time(now);
    m_rtc.set_date_time(now);
    return SUCCESS;
}


TimeClass::OpStatus TimeClass::get_date_time(RtcDateTime& now) {
    if (status() == NO_TIME) return STATUS_ERROR;
    else if (status() == READY_NO_RTC) {
        m_submitter.get_current_time(now);
        return STATUS_NO_RTC;
    }
    else if (status() == READY_NO_NTP || status() == READY) {
        m_rtc.get_date_time(now);
        return (status() == READY ? SUCCESS : STATUS_NO_NTP);
    }
    return STATUS_ERROR;
}


TimeClass::Status TimeClass::status() {
    if (m_NTPAvailable && m_RTCAvailable) return READY;
    else if (!m_NTPAvailable) return READY_NO_NTP;
    else if (!m_RTCAvailable) return READY_NO_RTC;
    else return NO_TIME;
}