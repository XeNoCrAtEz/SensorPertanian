#ifndef _SOIL_DATA_H_
#define _SOIL_DATA_H_


// struct for storing soil data
struct SoilData {
    int nitrogen, phosphorus, kalium;
    float pH, temperature, humidity;
    int EC;
};


#endif