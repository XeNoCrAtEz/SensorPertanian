#ifndef __BATT_MON_H__
#define __BATT_MON_H__


#include <Arduino.h>
#include <Battery.h>

class BatteryMonitor : public Battery {
public:
    BatteryMonitor(uint8_t pin, uint16_t min=3700, uint16_t max=4200, uint16_t vref=900, float ratio=2);
};


#endif