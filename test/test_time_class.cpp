#include <unity.h>

#include "timeClass.h"


const uint8_t PIN_GSM_RX = 2;
const uint8_t PIN_GSM_TX = 4;

const uint8_t PIN_RTC_RST  = 14;
const uint8_t PIN_RTC_DATA = 27;
const uint8_t PIN_RTC_CLK  = 26;


void test_time_class_get_time_no_rtc() {
#if defined(USE_WIFI)
    SubmitterWiFi testSubmitter;
#elif defined(USE_GSM)
    SubmitterGSM testSubmitter(PIN_GSM_RX, PIN_GSM_TX);
#endif
    RTC testRTC(PIN_RTC_DATA, PIN_RTC_CLK, PIN_RTC_RST);
    TimeClass testTimeClass = TimeClass(testRTC, testSubmitter);
    TEST_ASSERT_EQUAL(TimeClass::READY_NO_RTC, testTimeClass.status());

    RtcDateTime currentDateTime;
    TimeClass::OpStatus errCode = testTimeClass.get_date_time(currentDateTime);
    TEST_ASSERT_EQUAL(TimeClass::STATUS_NO_RTC, errCode);

    Serial.println("Time from network: " + String(currentDateTime.TotalSeconds()) + " = " + RtcDateTime_to_Str(currentDateTime));
}


void test_time_class_get_time_no_ntp() {
#if defined(USE_WIFI)
    SubmitterWiFi testSubmitter;
#elif defined(USE_GSM)
    SubmitterGSM testSubmitter(PIN_GSM_RX, PIN_GSM_TX);
#endif
    RTC testRTC(PIN_RTC_DATA, PIN_RTC_CLK, PIN_RTC_RST);
    TimeClass testTimeClass = TimeClass(testRTC, testSubmitter);
    TEST_ASSERT_EQUAL(TimeClass::READY_NO_NTP, testTimeClass.status());

    RtcDateTime currentDateTime;
    TimeClass::OpStatus errCode = testTimeClass.get_date_time(currentDateTime);
    TEST_ASSERT_EQUAL(TimeClass::STATUS_NO_NTP, errCode);

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
    TEST_ASSERT_EQUAL(TimeClass::READY, testTimeClass.status());

    RtcDateTime currentDateTime;
    TimeClass::OpStatus errCode = testTimeClass.get_date_time(currentDateTime);
    TEST_ASSERT_EQUAL(RTC::SUCCESS, errCode);

    Serial.println("Time from network: " + String(currentDateTime.TotalSeconds()) + " = " + RtcDateTime_to_Str(currentDateTime));
}


void test_time_class() {
    // RUN_TEST(test_time_class_get_time_no_rtc);
    RUN_TEST(test_time_class_get_time_no_ntp);
    RUN_TEST(test_time_class_get_time);
}