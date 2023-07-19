#include "main.h"


#define PROBE_RX_PIN 0
#define PROBE_TX_PIN 16


void setup() {
    // begin USB Serial
    Serial.begin(115200);
    
    setup_display();

    delay(2000);
    
    display_splash_screen();
}

void loop() {
    ProbeDefault probe(PROBE_RX_PIN, PROBE_TX_PIN);
    
    SoilData soilData = probe.sample();
    
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
