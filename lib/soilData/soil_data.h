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

    String toString() const {
        return "Nitrogen   : " + String(nitrogen) + " mg/kg\n" +
            "Phosphorous: " + String(phosphorus) + " mg/kg\n" +
            "Potassium  : " + String(kalium) + " mg/kg\n" +
            "pH         : " + String(pH) + "\n" +
            "Temperature: " + String(temperature) + " C\n" +
            "Humidity   : " + String(humidity) + " %\n" +
            "EC         : " + String(EC) + " uS/cm\n" +
            "Salt       : " + String(salt) + " g/kg\n";
    }


};


class SoilReading {
public:
    uint32_t m_epoch;         // seconds from 2000
    SoilData m_soilData;


    SoilReading()
            : m_soilData{}, m_epoch{0} {}
    SoilReading(const SoilData& data, const uint32_t& epch)
            : m_soilData{data}, m_epoch{epch} { }

    bool operator==(const SoilReading& s2) {
        return m_soilData == s2.m_soilData &&
                m_epoch == s2.m_epoch;
    }
};


#endif