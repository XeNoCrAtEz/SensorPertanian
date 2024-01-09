#ifndef _SOIL_DATA_H_
#define _SOIL_DATA_H_

#include <Arduino.h>

class SoilData : public Printable {
public:
    uint16_t nitrogen, phosphorus, kalium;
    float pH, temperature, humidity, salt;
    uint16_t EC;

    SoilData() : nitrogen{0}, phosphorus{0}, kalium{0}, pH{0}, temperature{0}, humidity{0}, salt{0}, EC{0} {}
    SoilData(uint16_t n, uint16_t p, uint16_t k, float ph, float temp, float hum, float salt, uint16_t ec)
            : nitrogen{n}, phosphorus{p}, kalium{k}, pH{ph}, temperature{temp}, humidity{hum}, salt{salt}, EC{ec} {}

    bool operator==(const SoilData& s2) {
        return EC == s2.EC &&
            humidity == s2.humidity &&
            kalium == s2.kalium &&
            nitrogen == s2.nitrogen &&
            pH == s2.pH &&
            phosphorus == s2.phosphorus &&
            salt == s2.salt &&
            temperature == s2.temperature;
    }

    size_t printTo(Print& p) const {
        size_t r = 0;

        // output to USB Serial
        r += p.println("Nitrogen   : " + String(nitrogen) + " mg/kg");
        r += p.println("Phosphorous: " + String(phosphorus) + " mg/kg");
        r += p.println("Potassium  : " + String(kalium) + " mg/kg");
        r += p.println("pH         : " + String(pH));
        r += p.println("Temperature: " + String(temperature) + " C");
        r += p.println("Humidity   : " + String(humidity) + " %");
        r += p.println("EC         : " + String(EC) + " uS/cm");
        r += p.println("Salt       : " + String(salt) + " g/kg");

        return r;
    }


};


#endif