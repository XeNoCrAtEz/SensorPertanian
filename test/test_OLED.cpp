#include <Arduino.h>
#include <unity.h>

#include "display.h"


const uint8_t PIN_SDA = 21;
const uint8_t PIN_SCL = 22;


void test_oled_init() {
    Display display(PIN_SDA, PIN_SCL);

    TEST_ASSERT_EQUAL(display.isOK(), Display::DISPLAY_OK);
}


void test_oled_display_splash() {
    Display display(PIN_SDA, PIN_SCL);

    TEST_ASSERT_EQUAL(display.isOK(), Display::DISPLAY_OK);
    TEST_ASSERT_EQUAL(display.display_splash_screen(), Display::DISPLAY_OK);

    delay(1000);
}


void test_oled_display_data() {
    Display display(PIN_SDA, PIN_SCL);

    TEST_ASSERT_EQUAL(display.isOK(), Display::DISPLAY_OK);
    
    SoilData testData = {1, 1, 1, 1, 1, 1, 1, 1};
    TEST_ASSERT_EQUAL(display.display_data(testData), Display::DISPLAY_OK);
}


void test_oled() {
    test_oled_init();
    test_oled_display_splash();
    test_oled_display_data();
}