#include <Arduino.h>
#include <unity.h>

#include "probe.h"
#include "display.h"


const uint8_t PIN_PROBE_RX = 16;
const uint8_t PIN_PROBE_TX = 17;
const uint8_t PIN_SDA = 21;
const uint8_t PIN_SCL = 22;


void test_ProbeKHDTK_sample() {
    ProbeKHDTK testProbe = ProbeKHDTK(PIN_PROBE_RX, PIN_PROBE_TX);

    SoilData sampledData = SoilData();
    TEST_ASSERT_EQUAL(Probe::SUCCESS, testProbe.sample(sampledData));

    Display display(PIN_SDA, PIN_SCL);
    display.display_data(sampledData);
}


void test_ProbeDefault_sample() {
    ProbeDefault testProbe = ProbeDefault(PIN_PROBE_RX, PIN_PROBE_TX);

    SoilData sampledData = SoilData();
    TEST_ASSERT_EQUAL(Probe::SUCCESS, testProbe.sample(sampledData));

    Display display(PIN_SDA, PIN_SCL);
    display.display_data(sampledData);
}

void test_ProbeNew_sample() {
    ProbeNew testProbe = ProbeNew(PIN_PROBE_RX, PIN_PROBE_TX);

    SoilData sampledData = SoilData();
    TEST_ASSERT_EQUAL(Probe::SUCCESS, testProbe.sample(sampledData));

    Display display(PIN_SDA, PIN_SCL);
    display.display_data(sampledData);
}

#ifndef NO_CALIB
void test_probe_calibration() {
    SoilData testNegativeData = SoilData(-1, -1, -1, 0, 0, 0, 0, 0);

    ProbeDefault testProbe(PIN_PROBE_RX, PIN_PROBE_TX);

    testProbe.calibrateNPK(testNegativeData);

    SoilData testZeroData = SoilData(0, 0, 0, 0, 0, 0, 0, 0);
    TEST_ASSERT_TRUE(testNegativeData==testZeroData);
}
#endif


void test_probe() {
    RUN_TEST(test_ProbeKHDTK_sample);
    RUN_TEST(test_ProbeDefault_sample);
    RUN_TEST(test_ProbeNew_sample);
#ifndef NO_CALIB
    RUN_TEST(test_probe_calibration);
#endif
}