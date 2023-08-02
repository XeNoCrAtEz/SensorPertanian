#include "main.h"


RTC_DATA_ATTR SoilDataTable dataTable;


void setup() {
    // begin USB Serial
    Serial.begin(115200);

    Display display;
    display.display_splash_screen();

    Submitter submitter(SENSOR_ID);

    ProbeDefault probe(PROBE_RX_PIN, PROBE_TX_PIN);

    SoilData soilData;
	// unsigned long epoch = submitter.get_timestamp();
    soilData = probe.sample();
    display.display_data(soilData);
    dataTable.push(soilData, 123456);   // TODO: Change to epoch
    
#ifdef DEBUG
    info_soil_data(soilData);
#endif

    if (submitter.is_connected()) submitter.submit_table(dataTable);
    
    submitter.test();

    // sleep()
}


void loop()
{
}


void sleep() {
}