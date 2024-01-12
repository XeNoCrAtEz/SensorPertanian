#include "voltageMonitor.h"


VoltageMonitor::VoltageMonitor(uint8_t sensePin, uint16_t refVoltage, float dividerRatio) 
        : sensePin(sensePin) {
    this->refVoltage = refVoltage;
	this->dividerRatio = dividerRatio;
	pinMode(this->sensePin, INPUT);
}


uint16_t VoltageMonitor::voltage() {
	analogRead(sensePin);   delay(2);   // allow the ADC to stabilize
	uint16_t reading = analogRead(sensePin) * dividerRatio * refVoltage / ADC_MAX_VALUE;
	return reading;
}


// ----------------------------------------- BATTERY MONITOR ------------------------------------
BatteryMonitor::BatteryMonitor(
	uint8_t sensePin, uint16_t refVoltage, float dividerRatio,
	uint16_t minVoltage, uint16_t maxVoltage) 
        : VoltageMonitor(sensePin, refVoltage, dividerRatio),
		  minVoltage(minVoltage), maxVoltage(maxVoltage) {}
    
    
uint8_t BatteryMonitor::level() {
    return level(voltage());
}


uint8_t BatteryMonitor::level(uint16_t voltage) {
    if (voltage <= minVoltage) {
		return 0;
	} else if (voltage >= maxVoltage) {
		return 100;
	} else {
		return (float) (voltage - minVoltage) / (maxVoltage - minVoltage) * 100;
	}
}