#include "main.h"


void setup() {
    // begin USB Serial
    Serial.begin(115200);
    
    setup_display();

    delay(2000);
    
    display_splash_screen();
}

void loop() {
    SoilData soilData(1);
    
    soilData.sample();
    
#ifdef DEBUG
    info_soil_data(
        soilData.nitrogen, soilData.phosphorus, soilData.kalium,
        soilData.pH, soilData.temperature, soilData.humidity,
        soilData.EC
    );
#endif

    display_data(soilData.nitrogen, soilData.phosphorus, soilData.kalium,
        soilData.pH, soilData.temperature, soilData.humidity,
        soilData.EC);
}
