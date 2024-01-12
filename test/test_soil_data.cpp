#include <unity.h>

#include "soil_data.h"


void test_print_soil_data() {
    auto testSoilData = SoilData(1, 1, 1, 1, 1, 1, 1, 1);

    Serial.println(testSoilData);
}


void test_soil_data() {
    RUN_TEST(test_print_soil_data);
}