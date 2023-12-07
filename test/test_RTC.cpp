#include "RTCModule.h"


const int PIN_RTC_RST  = 12;
const int PIN_RTC_DATA = 13;
const int PIN_RTC_CLK  = 15;


void test_rtc_get_date_time() {
    RTC testRTC(PIN_RTC_DATA, PIN_RTC_CLK, PIN_RTC_RST);

    RtcDateTime now = testRTC.get_date_time();

    print_date_time(now);
    Serial.println();
}


void test_rtc_set_date_time_compiled() {
    RTC testRTC(PIN_RTC_DATA, PIN_RTC_CLK, PIN_RTC_RST);

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    
    testRTC.set_date_time(compiled);

    RtcDateTime now = testRTC.get_date_time();
    
    print_date_time(now);
    Serial.println();
}


void test_rtc() {
    test_rtc_get_date_time();
    test_rtc_set_date_time_compiled();
}