#include <Arduino.h>
#include <unity.h>

#include "gps.h"


// test with GPS unconnected
void test_gps_unconnected() {
    GPS gps;

    uint8_t resultCode = gps.get_location_till_timeout();

    TEST_ASSERT_EQUAL(GPS_FAILED, resultCode);
}


void test_gps_connected_no_fix() {
    GPS gps;

    uint8_t resultCode = gps.get_location_till_timeout();

    TEST_ASSERT_EQUAL(GPS_NO_FIX, resultCode);
}


void test_gps_connected_fix() {
    GPS gps;

    uint8_t resultCode = gps.get_location_till_timeout();
    TEST_ASSERT_EQUAL(GPS_FIX, resultCode);
    
    double lat = 0, lng = 0;
    if (gps.get_location(lat, lng)) {
        Serial.print(F("- latitude: "));
        Serial.println(lat, 10);

        Serial.print(F("- longitude: "));
        Serial.println(lng, 10);
    }
}


void test_gps() {
    RUN_TEST(test_gps_unconnected);
    RUN_TEST(test_gps_connected_no_fix);
    RUN_TEST(test_gps_connected_fix);
}