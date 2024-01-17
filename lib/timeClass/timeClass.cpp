#include "timeClass.h"


TimeClass::TimeClass(RTC& rtc, Submitter& submitter) 
        : ESP32Time(), m_rtc(rtc), m_submitter(submitter) {
    m_RTCAvailable = rtc.status();
    m_NTPAvailable = submitter.is_time_available();

    if      (!m_RTCAvailable && m_NTPAvailable)  {
        RtcDateTime now;
        m_submitter.get_current_time(now);
        setTime(now.Second(), now.Minute(), now.Hour(), now.Day(), now.Month(), now.Year(), 0);

        m_status = READY_NO_RTC;
    }
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
        struct tm tm_now = getTimeStruct();
        now = RtcDateTime(
            tm_now.tm_year+1900, tm_now.tm_mon+1, tm_now.tm_mday,
            tm_now.tm_hour, tm_now.tm_min, tm_now.tm_sec);

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