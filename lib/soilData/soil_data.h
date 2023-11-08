#ifndef _SOIL_DATA_H_
#define _SOIL_DATA_H_


class SoilData {
private:


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


};


#endif