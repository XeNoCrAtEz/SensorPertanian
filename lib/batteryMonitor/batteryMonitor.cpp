#include "batteryMonitor.h"


const uint16_t BatteryMonitor::DEFAULT_MIN_VOLT = 3700;    // mV
const uint16_t BatteryMonitor::DEFAULT_MAX_VOLT = 4200;    // mV
const uint16_t BatteryMonitor::DEFAULT_VREF_VOLT = 900;    // mV
const uint8_t  BatteryMonitor::DEFAULT_RATIO = 2;


BatteryMonitor::BatteryMonitor(uint8_t pin, uint16_t min, uint16_t max, uint16_t vref, float ratio)
        : Battery{min, max, pin},
          MIN_VOLT{min}, MAX_VOLT{max}, VREF_VOLT{vref}, RATIO{ratio} {
    begin(vref, ratio);
}