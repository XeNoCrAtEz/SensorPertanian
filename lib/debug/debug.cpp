#include "debug.h"

void info_soil_data(const SoilData& soilData) {
    const uint16_t& nitrogen = soilData.nitrogen;
    const uint16_t& phosphorus = soilData.phosphorus;
    const uint16_t& kalium = soilData.kalium;
    const float& pH = soilData.pH;
    const float& temperature = soilData.temperature;
    const float& humidity = soilData.humidity;
    const uint16_t& EC = soilData.EC;
    const float& salt = soilData.salt; 

    // output to USB Serial
    Serial.print("Nitrogen: ");
    Serial.print(nitrogen);
    Serial.println(" mg/kg");

    Serial.print("Phosphorous: ");
    Serial.print(phosphorus);
    Serial.println(" mg/kg");

    Serial.print("Potassium: ");
    Serial.print(kalium);
    Serial.println(" mg/kg");

    Serial.print("pH: ");
    Serial.println(pH);

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");
    
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    Serial.print("EC: ");
    Serial.print(EC);
    Serial.println(" uS/cm");

    Serial.println("Salt");
    Serial.print(salt);
    Serial.println("g/kg");

    Serial.println();
}

