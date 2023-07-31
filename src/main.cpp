#include "main.h"


RTC_DATA_ATTR SoilDataTable dataTable;


void setup() {
    // begin USB Serial
    Serial.begin(115200);

    setup_display();

    delay(2000);
    
    display_splash_screen();

    dataTable.connect_wifi();
}

void loop() {
    ProbeDefault probe(PROBE_RX_PIN, PROBE_TX_PIN);
    
    SoilReading soilReading;
	// readings[idx_in].timestamp = get_time_stamp();
    soilReading.soilData = probe.sample();

    dataTable.push(soilReading);
    
#ifdef DEBUG
    info_soil_data(soilReading.soilData);
#endif

    SoilReading prevSoilData;
    dataTable.pop(prevSoilData);

    display_data(prevSoilData.soilData);


    dataTable.test();
}
