#include <unity.h>

#include "dataTable.h"
#include "debug.h"


bool operator==(const SoilData& s1, const SoilData& s2) {
    return s1.EC == s2.EC &&
            s1.humidity == s2.humidity &&
            s1.kalium == s2.kalium &&
            s1.nitrogen == s2.nitrogen &&
            s1.pH == s2.pH &&
            s1.phosphorus == s2.phosphorus &&
            s1.salt == s2.salt &&
            s1.temperature == s2.temperature;
}


bool operator==(const SoilReading& s1, const SoilReading& s2) {
    return s1.soilData == s2.soilData &&
            s1.epoch == s2.epoch;
}


void test_clear() {
    SoilDataTable testTable;
    
    testTable.clear();

    TEST_ASSERT_TRUE(testTable.is_empty());
}


void test_push_one_data() {
    SoilDataTable testTable;

    SoilReading testReading = SoilReading({1, 1, 1, 1, 1, 1, 1}, 1);
    uint8_t retCode = testTable.push(testReading);
    TEST_ASSERT_EQUAL(retCode, SoilDataTable::SUCCESS);

    testTable.clear();
    TEST_ASSERT_TRUE(testTable.is_empty());
}


void test_push_pop_one_data() {
    SoilDataTable testTable;

    SoilReading testReading = SoilReading({5, 5, 5, 5, 5, 5, 5}, 5);
    uint8_t retCode = testTable.push(testReading);
    TEST_ASSERT_EQUAL(retCode, SoilDataTable::SUCCESS);

    SoilReading poppedReading;
    retCode = testTable.pop(poppedReading);
    TEST_ASSERT_EQUAL(retCode, SoilDataTable::SUCCESS);

    TEST_ASSERT_TRUE(testReading == poppedReading);

    testTable.clear();
    TEST_ASSERT_TRUE(testTable.is_empty());
}


void test_push_pop_three_data() {
    SoilDataTable testTable;

    SoilReading testReading0 = SoilReading({7, 7, 7, 7, 7, 7, 7}, 7);
    uint8_t retCode = testTable.push(testReading0);
    TEST_ASSERT_EQUAL(retCode, SoilDataTable::SUCCESS);

    SoilReading testReading1 = SoilReading({8, 8, 8, 8, 8, 8, 8}, 8);
    retCode = testTable.push(testReading1);
    TEST_ASSERT_EQUAL(retCode, SoilDataTable::SUCCESS);

    SoilReading testReading2 = SoilReading({9, 9, 9, 9, 9, 9, 9}, 9);
    retCode = testTable.push(testReading2);
    TEST_ASSERT_EQUAL(retCode, SoilDataTable::SUCCESS);

    uint32_t size = 0;
    SoilReading* poppedReadings = nullptr;
    retCode = testTable.pop_all(poppedReadings, size);
    TEST_ASSERT_EQUAL(retCode, SoilDataTable::SUCCESS);
    TEST_ASSERT_NOT_EQUAL(nullptr, poppedReadings);

    TEST_ASSERT_TRUE(testReading0 == poppedReadings[0]);
    TEST_ASSERT_TRUE(testReading1 == poppedReadings[1]);
    TEST_ASSERT_TRUE(testReading2 == poppedReadings[2]);

    testTable.clear();
    delete[] poppedReadings;
    TEST_ASSERT_TRUE(testTable.is_empty());
}


void test_flash_memory() {
    RUN_TEST(test_clear);
    RUN_TEST(test_push_one_data);
    RUN_TEST(test_push_pop_one_data);
    RUN_TEST(test_push_pop_three_data);
}