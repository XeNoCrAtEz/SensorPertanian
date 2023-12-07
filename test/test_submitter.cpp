#include <unity.h>

#include "submitter.h"


const uint8_t PIN_GSM_DTR = 35;
const uint8_t PIN_GSM_RX = 32;
const uint8_t PIN_GSM_TX = 33;


void test_get_time() {
#if defined(USE_WIFI)
    SubmitterWiFi testSubmitter;
#elif defined(USE_GSM)
    SubmitterGSM testSubmitter(PIN_GSM_RX, PIN_GSM_TX);
#endif
    unsigned long epoch = testSubmitter.get_curr_epoch();
    Serial.print("Timestamp: ");
    Serial.println(testSubmitter.to_timestamp(epoch));
}


void test_submit_one_data() {
#if defined(USE_WIFI)
    SubmitterWiFi testSubmitter;
#elif defined(USE_GSM)
    SubmitterGSM testSubmitter(PIN_GSM_RX, PIN_GSM_TX);
#endif
    unsigned long epoch = testSubmitter.get_curr_epoch();

    SoilReading testSoilReading(SoilData(1, 1, 1, 1, 1, 1, 1, 1), epoch);
    
    int responseCode = testSubmitter.submit_reading(testSoilReading);

    TEST_ASSERT_EQUAL(200, responseCode);
}


void test_submit_table() {
#if defined(USE_WIFI)
    SubmitterWiFi testSubmitter;
#elif defined(USE_GSM)
    SubmitterGSM testSubmitter(PIN_GSM_RX, PIN_GSM_TX);
#endif
    SoilDataTable testTable;

    unsigned long epoch = testSubmitter.get_curr_epoch();

    testTable.push(SoilReading(SoilData(7, 7, 7, 7, 7, 7, 7, 7), epoch));
    testTable.push(SoilReading(SoilData(8, 8, 8, 8, 8, 8, 8, 8), epoch + 4*3600));
    testTable.push(SoilReading(SoilData(9, 9, 9, 9, 9, 9, 9, 9), epoch + 8*3600));
    
    int responseCode = testSubmitter.submit_reading(testTable);
    
    TEST_ASSERT_EQUAL(200, responseCode);
}


void test_submitter() {
    RUN_TEST(test_get_time);
    // RUN_TEST(test_submit_one_data);
    // RUN_TEST(test_submit_table);
}