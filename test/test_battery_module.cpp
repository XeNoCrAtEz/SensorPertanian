#include <Arduino.h>
#include <unity.h>

#include "batteryMonitor.h"


void test_battery() {
    BatteryMonitor batt;

    Serial.print("Battery voltage is ");
	Serial.print(batt.voltage());
	Serial.print(" (");
	Serial.print(batt.level());
	Serial.println("%)");
}