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
    Display testDisplay(PIN_SCREEN_SDA, PIN_SCREEN_SCL);

    RTC testRTC(PIN_RTC_DATA, PIN_RTC_CLK, PIN_RTC_RST);

    sleep(testRTC, testDisplay);
}


void test_sleep() {
    RUN_TEST(test_sleep_at_current_time);
}