#include <unity.h>

#include "dataTable.h"
#include "debug.h"


void test_clear() {
    SoilDataTable testTable;
    
    testTable.clear();

    TEST_ASSERT_TRUE(testTable.is_empty());
}


void test_push_one_data() {
    SoilDataTable testTable;

    SoilReading testReading = SoilReading(SoilData(1, 1, 1, 1, 1, 1, 1, 1), 1);
    uint8_t retCode = testTable.push(testReading);
    TEST_ASSERT_EQUAL(SoilDataTable::SUCCESS, retCode);

    testTable.clear();
    TEST_ASSERT_TRUE(testTable.is_empty());
}


void test_push_pop_one_data() {
    SoilDataTable testTable;

    SoilReading testReading = SoilReading(SoilData(5, 5, 5, 5, 5, 5, 5, 5), 5);
    uint8_t retCode = testTable.push(testReading);
    TEST_ASSERT_EQUAL(SoilDataTable::SUCCESS, retCode);

    SoilReading poppedReading;
    retCode = testTable.pop(poppedReading);
    TEST_ASSERT_EQUAL(SoilDataTable::SUCCESS, retCode);

    TEST_ASSERT_TRUE(testReading == poppedReading);

    testTable.clear();
    TEST_ASSERT_TRUE(testTable.is_empty());
}


void test_push_pop_three_data() {
    SoilDataTable testTable;

    SoilReading testReading0 = SoilReading(SoilData(7, 7, 7, 7, 7, 7, 7, 7), 7);
    uint8_t retCode = testTable.push(testReading0);
    TEST_ASSERT_EQUAL(SoilDataTable::SUCCESS, retCode);

    SoilReading testReading1 = SoilReading(SoilData(8, 8, 8, 8, 8, 8, 8, 8), 8);
    retCode = testTable.push(testReading1);
    TEST_ASSERT_EQUAL(SoilDataTable::SUCCESS, retCode);

    SoilReading testReading2 = SoilReading(SoilData(9, 9, 9, 9, 9, 9, 9, 9), 9);
    retCode = testTable.push(testReading2);
    TEST_ASSERT_EQUAL(SoilDataTable::SUCCESS, retCode);

    uint16_t size = 0;
    SoilReading* poppedReadings = nullptr;
    retCode = testTable.pop_all(poppedReadings, size);
    TEST_ASSERT_EQUAL(SoilDataTable::SUCCESS, retCode);
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