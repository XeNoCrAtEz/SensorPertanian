#include "RTCModule.h"


RTC::RTC(uint8_t dataPin, uint8_t clkPin, uint8_t rstPin)
        : m_RTCWire(dataPin, clkPin, rstPin), m_RTC(m_RTCWire) {
    m_RTC.Begin();

    if (!m_RTC.IsDateTimeValid()) {
        log_e("Error! Invalid Date Time!");
        m_status = NO_RTC;
        return;
    }

    if (m_RTC.GetIsWriteProtected()) {
        log_e("Error! RTC is write protected! Enabling now...");
        m_RTC.SetIsWriteProtected(false);
        log_i("OK");
    }

    if (!m_RTC.GetIsRunning()) {
        log_e("Error! RTC not running! Starting RTC...");
        m_RTC.SetIsRunning(true);
        log_i("OK");
    }

    m_status = READY;
}
            

RTC::OpStatus RTC::get_date_time(RtcDateTime& time) {
    if (status() != READY) return STATUS_ERROR;
    if (status() == NO_RTC) return STATUS_NO_RTC;
    
    time = m_RTC.GetDateTime();
    return SUCCESS;
}


RTC::OpStatus RTC::set_date_time(
            uint16_t year, uint8_t month, uint8_t dayOfMonth,
            uint8_t hour, uint8_t minute, uint8_t second
        ) {
    if (status() != READY) return STATUS_ERROR;
    if (status() == NO_RTC) return STATUS_NO_RTC;
            
    m_RTC.SetDateTime(RtcDateTime(year, month, dayOfMonth, hour, minute, second));
    return SUCCESS;
}


RTC::OpStatus RTC::set_date_time(const RtcDateTime& dateTime) {
    if (status() != READY) return STATUS_ERROR;
    if (status() == NO_RTC) return STATUS_NO_RTC;

    m_RTC.SetDateTime(dateTime);
    return SUCCESS;
}


RTC::Status RTC::status() {
    return m_status;
}


String RtcDateTime_to_Str(const RtcDateTime& dt) {
    return String(dt.Year()) + "-" + String(dt.Month()) + "-" + String(dt.Day()) + " " +
        String(dt.Hour()) + ":" + String(dt.Minute()) + ":" + String(dt.Second());
}