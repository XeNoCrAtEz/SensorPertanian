#ifndef __BATT_MON_H__
#define __BATT_MON_H__


#include <Arduino.h>
#include <Battery.h>

class BatteryMonitor : public Battery {
private:
    const uint16_t MIN_VOLT;    // mV
    const uint16_t MAX_VOLT;    // mV
    const uint16_t VREF_VOLT;    // mV
    const uint8_t  RATIO;


    static const uint16_t DEFAULT_MIN_VOLT;    // mV
    static const uint16_t DEFAULT_MAX_VOLT;    // mV
    static const uint16_t DEFAULT_VREF_VOLT;    // mV
    static const uint8_t  DEFAULT_RATIO;


public:
    BatteryMonitor(uint8_t pin, uint16_t min=DEFAULT_MIN_VOLT, uint16_t max=DEFAULT_MAX_VOLT, uint16_t vref=DEFAULT_VREF_VOLT, float ratio=DEFAULT_RATIO);
};


#endif