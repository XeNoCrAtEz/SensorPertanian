#include <Arduino.h>
#include <unity.h>

#include "batteryMonitor.h"


const uint8_t BATT_MON_PIN = 34;


void test_battery() {
    BatteryMonitor batt(BATT_MON_PIN);

    Serial.print("Battery voltage is ");
	Serial.print(batt.voltage());
	Serial.print(" (");
	Serial.print(batt.level());
	Serial.println("%)");
}