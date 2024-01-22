#ifndef __BATT_MON_H__
#define __BATT_MON_H__


#include <Arduino.h>
#include "SimpleKalmanFilter.h"


const uint16_t ESP32_REF_VOLTAGE = 3300;

const uint16_t MIN_VOLT_LIPO = 3000;
const uint16_t MAX_VOLT_LIPO = 4200;


class VoltageMonitor : public SimpleKalmanFilter {
private:
    enum VoltMonParams {
        NUM_SAMPLES = 10,
        ADC_MAX_VALUE = 4096,
    };


public:
    VoltageMonitor(uint8_t sensePin, uint16_t refVoltage, float dividerRatio, float mea_e, float est_e, float q);
    uint16_t voltage();


protected:
    const uint16_t m_refVoltage;        // refVoltage is in mV, typically the value of max ADC voltage
    const float m_dividerRatio;         // value of (R1 + R2) / R2 in a typical voltage divider circuit
    const uint8_t m_sensePin;


};


class BatteryMonitor : public VoltageMonitor {
public:
    BatteryMonitor(
        uint8_t sensePin, uint16_t refVoltage, float dividerRatio, 
        uint16_t minVoltage, uint16_t maxVoltage,
        float mea_e, float est_e, float q);
    uint8_t level();
    uint8_t level(uint16_t voltage);


private:
    const uint16_t m_minVoltage;        // value of measured voltage (in mV) that corresponds to 0%
    const uint16_t m_maxVoltage;        // value of measured voltage (in mV) that corresponds to 100%

};


#endif