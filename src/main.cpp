#include "main.h"


void setup() {
    // begin USB Serial
    Serial.begin(115200);

    SoilDataTable dataTable;
    Submitter submitter(SENSOR_ID);

#if defined(PROBE_DEFAULT)
    ProbeDefault probe(PIN_PROBE_RX, PIN_PROBE_TX);
#elif defined(PROBE_KHDTK)
    ProbeKHDTK probe(PIN_PROBE_RX, PIN_PROBE_TX);
#elif defined(PROBE_NEW)
    ProbeNew probe(PIN_PROBE_RX, PIN_PROBE_TX);
#endif

    Display display(PIN_SCREEN_SDA, PIN_SCREEN_SCL);
    display.display_splash_screen();

    SoilData soilData;
    probe.sample(soilData);
    display.display_data(soilData);

    int resetInfo = esp_reset_reason();
    if (resetInfo == ESP_RST_POWERON) {
        preferences.begin(epochNamespace, true);
        currentEpoch = preferences.getLong(epochKeyname, 0);
        preferences.end();
    }
    
    if (submitter.is_connected())
        currentEpoch = submitter.get_curr_epoch();

    dataTable.push(SoilReading(soilData, currentEpoch));

#ifdef DEBUG
    info_soil_data(soilData);
#endif

    int responseCode = submitter.submit_table(dataTable);
    if (responseCode == HTTP_CODE_OK) Serial.println("Data send successful!\n");
    else Serial.println("Data send failed!\n");

    Serial.println("Sensor now sleep...");
    sleep(display, submitter);
}


void loop()
{
}

