#include "main.h"

#define PROBE_RX_PIN 0
#define PROBE_TX_PIN 16


RTC_DATA_ATTR SoilDataTable dataTable;


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

    dataTable.push(soilData);
    
#ifdef DEBUG
    info_soil_data(soilData);
#endif

    SoilData prevSoilData;
    dataTable.pop(prevSoilData);

    display_data(prevSoilData);
}
