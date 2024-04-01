#include <unity.h>

#include "sleep.h"


const uint8_t PIN_RTC_RST  = 14;
const uint8_t PIN_RTC_DATA = 27;
const uint8_t PIN_RTC_CLK  = 26;

#ifdef USE_GSM
const uint8_t PIN_GSM_RX = 2;
const uint8_t PIN_GSM_TX = 4;
#endif


void test_sleep_at_current_time() {
    RTC testRTC = RTC(PIN_RTC_DATA, PIN_RTC_CLK, PIN_RTC_RST);

#if defined(USE_WIFI)
    SubmitterWiFi testSubmitter;
log_i("Using WiFi.");
#elif defined(USE_GSM)
    SubmitterGSM testSubmitter(PIN_GSM_RX, PIN_GSM_TX);
log_i("Using GSM.");
#endif

    TimeClass testTimeClass = TimeClass(testRTC, testSubmitter);

    Logger logger = Logger(testTimeClass);

    sleep(testTimeClass, logger);
}


void test_sleep() {
    RUN_TEST(test_sleep_at_current_time);
}