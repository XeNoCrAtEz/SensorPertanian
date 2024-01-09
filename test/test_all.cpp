#include <Arduino.h>
#include <unity.h>


void test_flash_memory();
void test_gps();
void test_battery();
void test_oled();
void test_probe();
void test_rtc();
void test_submitter();
void test_logger();
void test_sleep();
void test_soil_data();


void setUp() {
}


void tearDown() {
}


void setup() {
    Serial.begin(115200);
    
    UNITY_BEGIN();

    // test_flash_memory();
    // test_gps();
    // test_battery();
    // test_oled();
    // test_probe();
    // test_rtc();
    // test_submitter();
    test_logger();
    // test_sleep();
    // test_soil_data();

    UNITY_END();
}


void loop() {
}