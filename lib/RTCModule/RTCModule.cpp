#include "RTCModule.h"


RTC::RTC(uint8_t dataPin, uint8_t clkPin, uint8_t rstPin)
        : m_RTCWire(dataPin, clkPin, rstPin), m_RTC(m_RTCWire) {
    m_RTC.Begin();

    if (!m_RTC.IsDateTimeValid()) {
        log_e("Error! Invalid Date Time! Continuing...");
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
}
            

RtcDateTime RTC::get_date_time() {
    return m_RTC.GetDateTime();
}


void RTC::set_date_time(
            uint16_t year, uint8_t month, uint8_t dayOfMonth,
            uint8_t hour, uint8_t minute, uint8_t second
        ) {
    m_RTC.SetDateTime(RtcDateTime(year, month, dayOfMonth, hour, minute, second));
}


void RTC::set_date_time(const RtcDateTime& dateTime) {
    m_RTC.SetDateTime(dateTime);
}


void print_date_time(const RtcDateTime& dt) {
    Serial.println(RtcDateTime_to_Str(dt));
}


String RtcDateTime_to_Str(const RtcDateTime& dt) {
    return String(dt.Year()) + "-" + String(dt.Month()) + "-" + String(dt.Day()) + " " +
        String(dt.Hour()) + ":" + String(dt.Minute()) + ":" + String(dt.Second());
}