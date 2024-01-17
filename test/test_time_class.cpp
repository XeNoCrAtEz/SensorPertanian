#include <unity.h>

#include "timeClass.h"


const uint8_t PIN_GSM_DTR = 35;
const uint8_t PIN_GSM_RX = 32;
const uint8_t PIN_GSM_TX = 33;

const int PIN_RTC_RST  = 12;
const int PIN_RTC_DATA = 13;
const int PIN_RTC_CLK  = 15;


void test_time_class_get_time_no_rtc() {
#if defined(USE_WIFI)
    SubmitterWiFi testSubmitter;
#elif defined(USE_GSM)
    SubmitterGSM testSubmitter(PIN_GSM_RX, PIN_GSM_TX);
#endif
    RTC testRTC(PIN_RTC_DATA, PIN_RTC_CLK, PIN_RTC_RST);
    TimeClass testTimeClass = TimeClass(testRTC, testSubmitter);

    RtcDateTime currentDateTime;
    TimeClass::OpStatus errCode = testTimeClass.get_date_time(currentDateTime);
    TEST_ASSERT_EQUAL(RTC::STATUS_NO_RTC, errCode);

    Serial.println("Time from network: " + String(currentDateTime.TotalSeconds()) + " = " + RtcDateTime_to_Str(currentDateTime));
}


void test_time_class_get_time() {
#if defined(USE_WIFI)
    SubmitterWiFi testSubmitter;
#elif defined(USE_GSM)
    SubmitterGSM testSubmitter(PIN_GSM_RX, PIN_GSM_TX);
#endif
    RTC testRTC(PIN_RTC_DATA, PIN_RTC_CLK, PIN_RTC_RST);
    TimeClass testTimeClass = TimeClass(testRTC, testSubmitter);

    RtcDateTime currentDateTime;
    TimeClass::OpStatus errCode = testTimeClass.get_date_time(currentDateTime);
    TEST_ASSERT_EQUAL(RTC::SUCCESS, errCode);

    Serial.println("Time from network: " + String(currentDateTime.TotalSeconds()) + " = " + RtcDateTime_to_Str(currentDateTime));
}


void test_time_class() {
    RUN_TEST(test_time_class_get_time_no_rtc);
    // RUN_TEST(test_time_class_get_time);
}