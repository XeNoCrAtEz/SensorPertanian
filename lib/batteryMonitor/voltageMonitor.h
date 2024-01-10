#ifndef __BATT_MON_H__
#define __BATT_MON_H__


#include <Arduino.h>


class VoltageMonitor  {
private:
    enum VoltMonParams {
        ADC_MAX_VALUE = 4096,
    };


public:
    VoltageMonitor(uint8_t sensePin);
    void begin(uint16_t refVoltage, float dividerRatio);
    uint16_t voltage();


protected:
    uint16_t refVoltage;        // refVoltage is in mV, typically the value of max ADC voltage
    float dividerRatio;         // value of (R1 + R2) / R2 in a typical voltage divider circuit
    uint8_t sensePin;


};

class BatteryMonitor : public VoltageMonitor {
public:
    BatteryMonitor(uint8_t sensePin, uint16_t minVoltage, uint16_t maxVoltage);
    uint8_t level();
    uint8_t level(uint16_t voltage);


private:
    uint16_t minVoltage;        // value of measured voltage (in mV) that corresponds to 0%
    uint16_t maxVoltage;        // value of measured voltage (in mV) that corresponds to 100%

};


#endif