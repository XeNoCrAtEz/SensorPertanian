#include "voltageMonitor.h"


VoltageMonitor::VoltageMonitor(uint8_t sensePin, uint16_t refVoltage, float dividerRatio) 
		: m_sensePin(sensePin), m_refVoltage(refVoltage), m_dividerRatio(dividerRatio) {
	pinMode(m_sensePin, INPUT);
}


uint16_t VoltageMonitor::voltage() const {
	analogRead(m_sensePin);   delay(2);   // allow the ADC to stabilize

	int adc = 0;
	for (int i = 0; i < NUM_SAMPLES; i++) adc += analogRead(m_sensePin);
    adc /= NUM_SAMPLES;

	return adc * m_dividerRatio * m_refVoltage / ADC_MAX_VALUE;
}


// ----------------------------------------- BATTERY MONITOR ------------------------------------
BatteryMonitor::BatteryMonitor(
	uint8_t sensePin, uint16_t refVoltage, float dividerRatio,
	uint16_t minVoltage, uint16_t maxVoltage) 
        : VoltageMonitor(sensePin, refVoltage, dividerRatio),
		  m_minVoltage(minVoltage), m_maxVoltage(maxVoltage) {}


uint8_t BatteryMonitor::level() const {
    return level(voltage());
}


uint8_t BatteryMonitor::level(uint16_t voltage) const {
    if (voltage <= m_minVoltage) {
		return 0;
	} else if (voltage >= m_maxVoltage) {
		return 100;
	} else {
		return (float) (voltage - m_minVoltage) / (m_maxVoltage - m_minVoltage) * 100;
	}
}