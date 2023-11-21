#include "batteryMonitor.h"


BatteryMonitor::BatteryMonitor(uint8_t pin, uint16_t min, uint16_t max, uint16_t vref, float ratio)
        : Battery{min, max, pin} {
    begin(vref, ratio);
}