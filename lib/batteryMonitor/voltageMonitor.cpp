#include "voltageMonitor.h"


VoltageMonitor::VoltageMonitor(
	uint8_t sensePin, uint16_t refVoltage, float dividerRatio,
	float mea_e, float est_e, float q) 
		: filter(mea_e, est_e, q), m_sensePin(sensePin), m_refVoltage(refVoltage), m_dividerRatio(dividerRatio) {
	pinMode(m_sensePin, INPUT);
}


uint16_t VoltageMonitor::voltage() {
	analogRead(m_sensePin);   delay(2);   // allow the ADC to stabilize

	float estVoltValue = 0;
	for (int i = 0; i < NUM_SAMPLES; i++) {
		int voltValue = analogRead(m_sensePin) * m_dividerRatio * m_refVoltage / ADC_MAX_VALUE;
		estVoltValue = filter.updateEstimate(voltValue);
	}

	return estVoltValue;
}


// ----------------------------------------- BATTERY MONITOR ------------------------------------
BatteryMonitor::BatteryMonitor(
	uint8_t sensePin, uint16_t refVoltage, float dividerRatio,
	uint16_t minVoltage, uint16_t maxVoltage,
	float mea_e, float est_e, float q)
        : VoltageMonitor(sensePin, refVoltage, dividerRatio, mea_e, est_e, q),
		  m_minVoltage(minVoltage), m_maxVoltage(maxVoltage) {}


uint8_t BatteryMonitor::level() {
    return level(voltage());
}


uint8_t BatteryMonitor::level(uint16_t voltage) {
    if (voltage <= m_minVoltage) {
		return 0;
	} else if (voltage >= m_maxVoltage) {
		return 100;
	} else {
		return (float) (voltage - m_minVoltage) / (m_maxVoltage - m_minVoltage) * 100;
	}
}