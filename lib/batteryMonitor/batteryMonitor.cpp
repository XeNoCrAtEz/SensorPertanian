#include "batteryMonitor.h"


BatteryMonitor::BatteryMonitor(uint16_t min, uint16_t max, uint8_t pin, uint16_t vref, float ratio)
        : BATT_MIN{min}, BATT_MAX{max}, BATT_VOLT_PIN{pin},
          Battery{min, max, pin},
          VREF{vref}, RATIO{ratio} {
    begin(vref, ratio);
}