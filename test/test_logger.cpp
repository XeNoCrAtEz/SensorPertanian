#include <unity.h>

#include "debug.h"


void test_single_msg() {
    String expectedMsg = "1 [E] - Error! JK just a normal message :)";

    auto testDebug = Logger();
    TEST_ASSERT_EQUAL(true, testDebug.is_ready());

    Logger::ErrorCodes errCode = testDebug.log_E(1, "Error! JK just a normal message :)");
    TEST_ASSERT_EQUAL(Logger::SUCCESS, errCode);

    errCode = testDebug.show();
    TEST_ASSERT_EQUAL(Logger::SUCCESS, errCode);

    testDebug.clear();
}


void test_logger() {
    RUN_TEST(test_single_msg);
}