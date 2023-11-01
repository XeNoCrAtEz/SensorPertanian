#ifndef _SOIL_DATA_H_
#define _SOIL_DATA_H_


class SoilData {
private:


public:
    uint16_t nitrogen, phosphorus, kalium;
    float pH, temperature, humidity, salt;
    uint16_t EC;
};


#endif