#include <unity.h>

#include "RTCModule.h"


const uint8_t PIN_RTC_RST  = 14;
const uint8_t PIN_RTC_DATA = 27;
const uint8_t PIN_RTC_CLK  = 26;


void test_rtc_get_date_time() {
    RTC testRTC(PIN_RTC_DATA, PIN_RTC_CLK, PIN_RTC_RST);

    RtcDateTime now;
    RTC::OpStatus errCode = testRTC.get_date_time(now);
    TEST_ASSERT_EQUAL(RTC::SUCCESS, errCode);

    Serial.println(RtcDateTime_to_Str(now));
}


void test_rtc_set_date_time_compiled() {
    RTC testRTC(PIN_RTC_DATA, PIN_RTC_CLK, PIN_RTC_RST);

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    
    testRTC.set_date_time(compiled);

    RtcDateTime now;
    RTC::OpStatus errCode = testRTC.get_date_time(now);
    TEST_ASSERT_EQUAL(RTC::SUCCESS, errCode);
    
    Serial.println(RtcDateTime_to_Str(now));
}


void test_rtc() {
    RUN_TEST(test_rtc_get_date_time);
    RUN_TEST(test_rtc_set_date_time_compiled);
}