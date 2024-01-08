#include <unity.h>

#include "debug.h"


void test_single_msg() {
    String expectedMsg = "1 [E] - Error! JK just a normal message :)";

    auto testDebug = Debugger();
    TEST_ASSERT_EQUAL(true, testDebug.isReady());

    Debugger::ErrorCodes errCode = testDebug.log_E(1, "Error! JK just a normal message :)");
    TEST_ASSERT_EQUAL(Debugger::SUCCESS, errCode);

    errCode = testDebug.show();
    TEST_ASSERT_EQUAL(Debugger::SUCCESS, errCode);

    testDebug.clear();
}


void test_logger() {
    RUN_TEST(test_single_msg);
}