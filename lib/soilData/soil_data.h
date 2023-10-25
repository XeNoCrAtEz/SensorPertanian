#ifndef _SOIL_DATA_H_
#define _SOIL_DATA_H_


// struct for storing soil data
struct SoilData {
    int nitrogen, phosphorus, kalium;
    float pH, temperature, humidity, salt;
    int EC;

    bool operator==(const SoilData& other) {
        return nitrogen == other.nitrogen &&
                phosphorus == other.phosphorus &&
                kalium == other.kalium &&
                pH == other.pH &&
                temperature == other.temperature &&
                humidity == other.humidity &&
                salt == other.salt &&
                EC == other.EC;
    }
};


#endif