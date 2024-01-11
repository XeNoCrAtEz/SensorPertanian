#include <unity.h>

#include "debug.h"


const int PIN_RTC_RST  = 12;
const int PIN_RTC_DATA = 13;
const int PIN_RTC_CLK  = 15;


void test_show() {
    RTC testRTC = RTC(PIN_RTC_DATA, PIN_RTC_CLK, PIN_RTC_RST);

    auto testDebug = Logger(testRTC);
    TEST_ASSERT_EQUAL(true, testDebug.is_ready());

    auto errCode = testDebug.show();
    TEST_ASSERT_EQUAL(Logger::SUCCESS, errCode);
}


void test_single_msg() {
    String expectedMsg = "1 [E] - Error! JK just a normal message :)";

    RTC testRTC = RTC(PIN_RTC_DATA, PIN_RTC_CLK, PIN_RTC_RST);

    auto testDebug = Logger(testRTC);
    TEST_ASSERT_EQUAL(true, testDebug.is_ready());

    Logger::ErrorCodes errCode = testDebug.log_E("Error! JK just a normal message :)");
    TEST_ASSERT_EQUAL(Logger::SUCCESS, errCode);

    errCode = testDebug.show();
    TEST_ASSERT_EQUAL(Logger::SUCCESS, errCode);

    testDebug.clear();
}


void test_single_msg_print_mode() {
    String expectedMsg = "1 [E] - Error! JK just a normal message :)";

    RTC testRTC = RTC(PIN_RTC_DATA, PIN_RTC_CLK, PIN_RTC_RST);

    auto testDebug = Logger(testRTC, true);
    TEST_ASSERT_EQUAL(true, testDebug.is_ready());
    TEST_ASSERT_EQUAL(true, testDebug.is_print_mode());

    Logger::ErrorCodes errCode = testDebug.log_E("Error! JK just a normal message :)");
    TEST_ASSERT_EQUAL(Logger::PRINT_MODE, errCode);

    errCode = testDebug.show();
    TEST_ASSERT_EQUAL(Logger::PRINT_MODE, errCode);

    testDebug.clear();
}


// NOTE: run this without -DDEBUG compilation flag
#ifndef DEBUG
void test_debug_disabled() {
    String expectedMsg = "1 [E] - Error! JK just a normal message :)";

    RTC testRTC = RTC(PIN_RTC_DATA, PIN_RTC_CLK, PIN_RTC_RST);

    auto testDebug = Logger(testRTC, true);
    TEST_ASSERT_EQUAL(false, testDebug.is_ready());
    TEST_ASSERT_EQUAL(false, testDebug.is_print_mode());

    Logger::ErrorCodes errCode = testDebug.log_E("Error! JK just a normal message :)");
    TEST_ASSERT_EQUAL(Logger::DEBUG_INACTIVE, errCode);

    errCode = testDebug.show();
    TEST_ASSERT_EQUAL(Logger::DEBUG_INACTIVE, errCode);

    testDebug.clear();
}
#endif


void test_clear_logger() {
    RTC testRTC = RTC(PIN_RTC_DATA, PIN_RTC_CLK, PIN_RTC_RST);

    auto testDebug = Logger(testRTC);
    TEST_ASSERT_EQUAL(true, testDebug.is_ready());

    auto errCode = testDebug.clear();
    TEST_ASSERT_EQUAL(Logger::SUCCESS, errCode);
}


void test_logger() {
    RUN_TEST(test_show);
    // RUN_TEST(test_single_msg);
    // RUN_TEST(test_single_msg_print_mode);
#ifndef DEBUG
    // RUN_TEST(test_debug_disabled);
#endif
    // RUN_TEST(test_clear);
}