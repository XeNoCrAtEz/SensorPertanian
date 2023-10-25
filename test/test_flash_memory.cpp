// // TEST FLASH MEMORY

// #include "dataTable.h"
// #include <unity.h>


// const uint8_t PIN_SCK = 14;
// const uint8_t PIN_MOSI = 13;
// const uint8_t PIN_MISO = 12;
// const uint8_t PIN_CS = 32;


// void setUp(void) {
// }


// void tearDown(void) {
// }


// void test_init() {
//     SoilDataTable testTable(PIN_SCK, PIN_MOSI, PIN_MISO, PIN_CS);

//     TEST_ASSERT_TRUE(testTable.is_empty());
// }


// void test_push_one_data() {
//     SoilDataTable testTable(PIN_SCK, PIN_MOSI, PIN_MISO, PIN_CS);

//     testTable.push(SoilReading({1, 1, 1, 1, 1, 1, 1}, 1));

//     TEST_ASSERT_FALSE(testTable.is_empty());
//     TEST_ASSERT_EQUAL(testTable.get_count(), 1);
// }


// void test_pop_one_data() {
//     SoilDataTable testTable(PIN_SCK, PIN_MOSI, PIN_MISO, PIN_CS);

//     SoilReading soilReading = SoilReading({1, 1, 1, 1, 1, 1, 1}, 1);

//     // testTable.push(soilReading);
//     testTable.push(SoilReading({1, 1, 1, 1, 1, 1, 1}, 1));


//     SoilReading poppedReading;
//     // testTable.pop(poppedReading);

//     TEST_ASSERT_TRUE(soilReading == poppedReading);
// }


// void test_push_three_data() {
//     SoilDataTable testTable(PIN_SCK, PIN_MOSI, PIN_MISO, PIN_CS);

//     testTable.push(SoilReading({1, 1, 1, 1, 1, 1, 1}, 1));
//     testTable.push(SoilReading({2, 2, 2, 2, 2, 2, 2}, 2));
//     testTable.push(SoilReading({3, 3, 3, 3, 3, 3, 3}, 3));

//     TEST_ASSERT_FALSE(testTable.is_empty());
//     TEST_ASSERT_EQUAL(testTable.get_count(), 3);
// }


// void test_pop_three_data() {
//     SoilDataTable testTable(PIN_SCK, PIN_MOSI, PIN_MISO, PIN_CS);

//     SoilReading soilReading1 = SoilReading({1, 1, 1, 1, 1, 1, 1}, 1);
//     SoilReading soilReading2 = SoilReading({2, 2, 2, 2, 2, 2, 2}, 2);
//     SoilReading soilReading3 = SoilReading({3, 3, 3, 3, 3, 3, 3}, 3);

//     testTable.push(soilReading1);
//     testTable.push(soilReading2);
//     testTable.push(soilReading3);

//     SoilReading poppedReading;
//     testTable.pop(poppedReading);
//     TEST_ASSERT_TRUE(soilReading1 == poppedReading);

//     testTable.pop(poppedReading);
//     TEST_ASSERT_TRUE(soilReading2 == poppedReading);

//     testTable.pop(poppedReading);
//     TEST_ASSERT_TRUE(soilReading3 == poppedReading);
// }


// void test_clear() {
//     SoilDataTable testTable(PIN_SCK, PIN_MOSI, PIN_MISO, PIN_CS);

//     SoilReading soilReading1 = SoilReading({1, 1, 1, 1, 1, 1, 1}, 1);
//     SoilReading soilReading2 = SoilReading({2, 2, 2, 2, 2, 2, 2}, 2);
//     SoilReading soilReading3 = SoilReading({3, 3, 3, 3, 3, 3, 3}, 3);

//     testTable.push(soilReading1);
//     testTable.push(soilReading2);
//     testTable.push(soilReading3);

//     testTable.clear();

//     TEST_ASSERT_TRUE(testTable.is_empty());
// }


// // TODO:
// void test_fill_table() {
// }


// void setup() {
//     UNITY_BEGIN();

//     RUN_TEST(test_init);
//     RUN_TEST(test_push_one_data);
//     // RUN_TEST(test_pop_one_data);
//     RUN_TEST(test_push_three_data);
//     // RUN_TEST(test_pop_three_data);
//     RUN_TEST(test_clear);

//     UNITY_END();
// }

// void loop() {
// }