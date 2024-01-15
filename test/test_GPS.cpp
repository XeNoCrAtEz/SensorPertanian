#include <Arduino.h>
#include <unity.h>

#include "gps.h"


const uint8_t PIN_GPS_RX = 19;
const uint8_t PIN_GPS_TX = 18;

// test with GPS unconnected
void test_gps_unconnected() {
    GPS gps(PIN_GPS_RX, PIN_GPS_TX);

    TEST_ASSERT_EQUAL(GPS::NO_GPS, gps.status());
}


void test_gps_connected_no_fix() {
    GPS gps(PIN_GPS_RX, PIN_GPS_TX);

    double lat, lng;
    GPS::OpStatus resultCode = gps.get_location(lat, lng);
    TEST_ASSERT_EQUAL(GPS::STATUS_NO_FIX, resultCode);
    TEST_ASSERT_EQUAL(0.0, lat);
    TEST_ASSERT_EQUAL(0.0, lng);
}


void test_gps_connected_fix() {
    GPS gps(PIN_GPS_RX, PIN_GPS_TX);

    double lat, lng;
    uint8_t resultCode = gps.get_location(lat, lng);
    TEST_ASSERT_EQUAL(GPS::SUCCESS, resultCode);
    
    if (resultCode == GPS::SUCCESS) {
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