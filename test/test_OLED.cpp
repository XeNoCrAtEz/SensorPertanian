#include <Arduino.h>
#include <unity.h>

#include "display.h"


const uint8_t PIN_SDA = 21;
const uint8_t PIN_SCL = 22;


void test_oled_init() {
    Display display(PIN_SDA, PIN_SCL);

    TEST_ASSERT_EQUAL(Display::READY, display.status());
}


void test_oled_display_splash() {
    Display display(PIN_SDA, PIN_SCL);

    TEST_ASSERT_EQUAL(Display::READY, display.status());
    TEST_ASSERT_EQUAL(Display::SUCCESS, display.display_splash_screen());

    delay(1000);
}


void test_oled_display_data() {
    Display display(PIN_SDA, PIN_SCL);

    TEST_ASSERT_EQUAL(Display::READY, display.status());
    
    SoilData testData = {1, 1, 1, 1, 1, 1, 1, 1};
    TEST_ASSERT_EQUAL(Display::SUCCESS, display.display_data(testData));
}


void test_oled() {
    RUN_TEST(test_oled_init);
    RUN_TEST(test_oled_display_splash);
    RUN_TEST(test_oled_display_data);
}