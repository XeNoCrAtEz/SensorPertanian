#include <Arduino.h>
#include <unity.h>


void test_flash_memory();
void test_gps();
void test_voltage_monitor();
void test_oled();
void test_probe();
void test_rtc();
void test_submitter();
void test_logger();
void test_sleep();
void test_soil_data();
void test_time_class();


void setUp() {
}


void tearDown() {
}


void setup() {
    Serial.begin(115200);
    
    UNITY_BEGIN();

    // test_flash_memory();
    // test_gps();
    // test_voltage_monitor();
    // test_oled();
    // test_probe();
    // test_rtc();
    // test_submitter();
    // test_logger();
    // test_soil_data();
    test_time_class();
    // test_sleep();

    UNITY_END();
}


void loop() {
}