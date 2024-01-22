#include <Arduino.h>
#include <unity.h>

#include "voltageMonitor.h"


const uint8_t SC_MON_PIN = 35;
const uint8_t BATT_MON_PIN = 34;


void test_battery_volt() {
    BatteryMonitor batt(BATT_MON_PIN, 3300, 2, 3000, 4200, 1, 1, 0.01);

    Serial.print("Battery voltage is: ");
	Serial.print(batt.voltage());
	Serial.print(" mV (");
	Serial.print(batt.level());
	Serial.println("%)");
}


void test_solar_panel_volt() {
	VoltageMonitor sc(SC_MON_PIN, 3300, 2, 1, 1, 0.01);

    Serial.print("Solar Panel voltage is: ");
	Serial.print(sc.voltage());
	Serial.println(" mV");
}


void test_voltage_monitor() {
	RUN_TEST(test_battery_volt);
	RUN_TEST(test_solar_panel_volt);
}