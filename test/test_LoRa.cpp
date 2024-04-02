// #include <unity.h>

// #include "LoRa_module.h"


// const uint8_t PIN_RTC_RST  = 14;
// const uint8_t PIN_RTC_DATA = 27;
// const uint8_t PIN_RTC_CLK  = 26;

// #ifdef USE_GSM
// const uint8_t PIN_GSM_RX = 2;
// const uint8_t PIN_GSM_TX = 4;
// #endif


// void test_show() {
// #if defined(USE_WIFI)
//     SubmitterWiFi testSubmitter;
// log_i("Using WiFi.");
// #elif defined(USE_GSM)
//     SubmitterGSM testSubmitter(PIN_GSM_RX, PIN_GSM_TX);
// log_i("Using GSM.");
// #endif
//     RTC testRTC = RTC(PIN_RTC_DATA, PIN_RTC_CLK, PIN_RTC_RST);
//     TimeClass testTimeClass = TimeClass(testRTC, testSubmitter);

//     auto testDebug = Logger(testTimeClass);
//     TEST_ASSERT_EQUAL(Logger::READY, testDebug.status());

//     auto errCode = testDebug.show();
//     TEST_ASSERT_EQUAL(Logger::SUCCESS, errCode);
// }


// void test_single_msg_no_clear() {
//     String expectedMsg = "1 [E] - Error! JK just a normal message :)";

// #if defined(USE_WIFI)
//     SubmitterWiFi testSubmitter;
// log_i("Using WiFi.");
// #elif defined(USE_GSM)
//     SubmitterGSM testSubmitter(PIN_GSM_RX, PIN_GSM_TX);
// log_i("Using GSM.");
// #endif
//     RTC testRTC = RTC(PIN_RTC_DATA, PIN_RTC_CLK, PIN_RTC_RST);
//     TimeClass testTimeClass = TimeClass(testRTC, testSubmitter);

//     auto testDebug = Logger(testTimeClass);
//     TEST_ASSERT_EQUAL(Logger::READY, testDebug.status());

//     Logger::OpStatus errCode = testDebug.log_E("Error! JK just a normal message :)");
//     TEST_ASSERT_EQUAL(Logger::SUCCESS, errCode);

//     errCode = testDebug.show();
//     TEST_ASSERT_EQUAL(Logger::SUCCESS, errCode);
// }


// void test_single_msg() {
//     String expectedMsg = "1 [E] - Error! JK just a normal message :)";

// #if defined(USE_WIFI)
//     SubmitterWiFi testSubmitter;
// log_i("Using WiFi.");
// #elif defined(USE_GSM)
//     SubmitterGSM testSubmitter(PIN_GSM_RX, PIN_GSM_TX);
// log_i("Using GSM.");
// #endif
//     RTC testRTC = RTC(PIN_RTC_DATA, PIN_RTC_CLK, PIN_RTC_RST);
//     TimeClass testTimeClass = TimeClass(testRTC, testSubmitter);

//     auto testDebug = Logger(testTimeClass);
//     TEST_ASSERT_EQUAL(Logger::READY, testDebug.status());

//     Logger::OpStatus errCode = testDebug.log_E("Error! JK just a normal message :)");
//     TEST_ASSERT_EQUAL(Logger::SUCCESS, errCode);

//     errCode = testDebug.show();
//     TEST_ASSERT_EQUAL(Logger::SUCCESS, errCode);

//     testDebug.clear();
// }


// void test_single_msg_print_mode() {
//     String expectedMsg = "1 [E] - Error! JK just a normal message :)";

// #if defined(USE_WIFI)
//     SubmitterWiFi testSubmitter;
// log_i("Using WiFi.");
// #elif defined(USE_GSM)
//     SubmitterGSM testSubmitter(PIN_GSM_RX, PIN_GSM_TX);
// log_i("Using GSM.");
// #endif
//     RTC testRTC = RTC(PIN_RTC_DATA, PIN_RTC_CLK, PIN_RTC_RST);
//     TimeClass testTimeClass = TimeClass(testRTC, testSubmitter);

//     auto testDebug = Logger(testTimeClass);
//     TEST_ASSERT_EQUAL(Logger::READY, testDebug.status());
//     TEST_ASSERT_EQUAL(true, testDebug.is_print_mode());

//     Logger::OpStatus errCode = testDebug.log_E("Error! JK just a normal message :)");
//     TEST_ASSERT_EQUAL(Logger::PRINT_MODE, errCode);

//     errCode = testDebug.show();
//     TEST_ASSERT_EQUAL(Logger::PRINT_MODE, errCode);

//     testDebug.clear();
// }


// // NOTE: run this without -DDEBUG compilation flag
// #ifndef DEBUG
// void test_debug_disabled() {
//     String expectedMsg = "1 [E] - Error! JK just a normal message :)";

// #if defined(USE_WIFI)
//     SubmitterWiFi testSubmitter;
// log_i("Using WiFi.");
// #elif defined(USE_GSM)
//     SubmitterGSM testSubmitter(PIN_GSM_RX, PIN_GSM_TX);
// log_i("Using GSM.");
// #endif
//     RTC testRTC = RTC(PIN_RTC_DATA, PIN_RTC_CLK, PIN_RTC_RST);
//     TimeClass testTimeClass = TimeClass(testRTC, testSubmitter);

//     auto testDebug = Logger(testTimeClass);
//     TEST_ASSERT_EQUAL(Logger::READY testDebug.status());
//     TEST_ASSERT_EQUAL(false, testDebug.is_print_mode());

//     Logger::OpStatus errCode = testDebug.log_E("Error! JK just a normal message :)");
//     TEST_ASSERT_EQUAL(Logger::DEBUG_INACTIVE, errCode);

//     errCode = testDebug.show();
//     TEST_ASSERT_EQUAL(Logger::DEBUG_INACTIVE, errCode);

//     testDebug.clear();
// }
// #endif


// void test_clear_logger() {
// #if defined(USE_WIFI)
//     SubmitterWiFi testSubmitter;
// log_i("Using WiFi.");
// #elif defined(USE_GSM)
//     SubmitterGSM testSubmitter(PIN_GSM_RX, PIN_GSM_TX);
// log_i("Using GSM.");
// #endif
//     RTC testRTC = RTC(PIN_RTC_DATA, PIN_RTC_CLK, PIN_RTC_RST);
//     TimeClass testTimeClass = TimeClass(testRTC, testSubmitter);

//     auto testDebug = Logger(testTimeClass);
//     TEST_ASSERT_EQUAL(Logger::READY, testDebug.status());

//     auto errCode = testDebug.clear();
//     TEST_ASSERT_EQUAL(Logger::SUCCESS, errCode);
// }


// void test_logger() {
//     RUN_TEST(test_show);
//     // RUN_TEST(test_single_msg_no_clear);
//     // RUN_TEST(test_single_msg);
//     // RUN_TEST(test_single_msg_print_mode);
// #ifndef DEBUG
//     // RUN_TEST(test_debug_disabled);
// #endif
//     // RUN_TEST(test_clear_logger);
// }