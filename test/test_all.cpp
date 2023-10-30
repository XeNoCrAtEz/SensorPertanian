#include <Arduino.h>
#include <unity.h>

void test_flash_memory();
void test_gps();

void setUp() {
}


void tearDown() {
}


void setup() {
    Serial.begin(115200);
    
    UNITY_BEGIN();

    // test_flash_memory();
    // test_gps();

    UNITY_END();
}


void loop() {
}