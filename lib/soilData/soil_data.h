#ifndef _SOIL_DATA_H_
#define _SOIL_DATA_H_


class SoilData {
private:


public:
    uint16_t nitrogen, phosphorus, kalium;
    float pH, temperature, humidity, salt;
    uint16_t EC;

    SoilData() : nitrogen{0}, phosphorus{0}, kalium{0}, pH{0}, temperature{0}, humidity{0}, salt{0}, EC{0} {}
};


#endif