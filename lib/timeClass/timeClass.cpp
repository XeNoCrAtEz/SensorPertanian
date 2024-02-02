#include "timeClass.h"


TimeClass::TimeClass(RTC& rtc, Submitter& submitter) 
        : ESP32Time(0), m_rtc(rtc), m_submitter(submitter) {
    if (rtc.status() == RTC::READY) m_RTCAvailable = true;
    m_NTPAvailable = submitter.is_time_available();

    if      (!m_RTCAvailable && m_NTPAvailable)  {
        m_status = READY_NO_RTC;

        RtcDateTime now;
        m_submitter.get_current_time(now);
        setTime(now.Second(), now.Minute(), now.Hour(), now.Day(), now.Month(), now.Year(), 0);
    }
    else if (m_RTCAvailable && !m_NTPAvailable)  m_status = READY_NO_NTP;
    else if (!m_RTCAvailable && !m_NTPAvailable) m_status = NO_TIME;
    else if (m_NTPAvailable && m_RTCAvailable) {
        m_status = READY;

        Serial.println("test.....");
        update_RTC();
    }
}


TimeClass::OpStatus TimeClass::update_RTC() {
    if (status() != READY) return STATUS_ERROR;

    RtcDateTime now;
    m_submitter.get_current_time(now);
    Serial.println(RtcDateTime_to_Str(now));
    m_rtc.set_date_time(now);
    return SUCCESS;
}


TimeClass::OpStatus TimeClass::get_date_time(RtcDateTime& now) {
    if (status() == NO_TIME) return STATUS_ERROR;
    else if (status() == READY_NO_RTC) {
        struct tm tm_now = getTimeStruct();
        now = RtcDateTime(
            getYear(), getMonth()+MONTH_OFFSET, getDay(),
            getHour(true), getMinute(), getSecond()
            );
        return STATUS_NO_RTC;
    }
    else if (status() == READY_NO_NTP || status() == READY) {
        m_rtc.get_date_time(now);
        return (status() == READY ? SUCCESS : STATUS_NO_NTP);
    }
    return STATUS_ERROR;
}


TimeClass::Status TimeClass::status() {
    return m_status;
}