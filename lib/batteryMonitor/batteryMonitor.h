#include <Arduino.h>
#include <Battery.h>

class BatteryMonitor : public Battery {
private:
    const uint8_t BATT_VOLT_PIN;
    const uint16_t VREF;
    const float RATIO;
    const uint16_t BATT_MIN;
    const uint16_t BATT_MAX;


public:
    BatteryMonitor(uint16_t min=3700, uint16_t max=4200, uint8_t pin=34, uint16_t vref=900, float ratio=2);
};
