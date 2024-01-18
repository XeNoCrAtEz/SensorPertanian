#include <unity.h>

#include "sleep.h"


const int PIN_RTC_RST  = 12;
const int PIN_RTC_DATA = 13;
const int PIN_RTC_CLK  = 15;

const int PIN_SCREEN_SDA = 5;
const int PIN_SCREEN_SCL = 4;

#ifdef USE_GSM
const int PIN_GSM_RX = 1;
const int PIN_GSM_TX = 1;
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