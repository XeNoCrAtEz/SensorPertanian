#include "main.h"


void setup() {
    // begin USB Serial
    Serial.begin(115200);

    SoilDataTable dataTable;
    if (!dataTable.is_ready()) logger.log_E(time, "Error! dataTable not ready!");
    else logger.log_I(time, "dataTable is ready!");

#if defined(USE_WIFI)
    SubmitterWiFi submitter;
    if (!submitter.is_ready()) logger.log_E(time, "Error! WiFi submitter is not ready!");
    else logger.log_I(time, "WiFi submitter is ready!");
#elif defined(USE_GSM)
    SubmitterGSM submitter(PIN_GSM_RX, PIN_GSM_TX);
    if (!submitter.is_ready()) logger.log_E(time, "Error! GSM submitter is not ready!");
    else logger.log_I(time, "GSM submitter is ready!");
#endif

    logger.log_I(time, "Initializing probe...")
#if defined(PROBE_DEFAULT)
    ProbeDefault probe(PIN_PROBE_RX, PIN_PROBE_TX);
    logger.log_I(time, "Probe type: DEFAULT");
#elif defined(PROBE_KHDTK)
    ProbeKHDTK probe(PIN_PROBE_RX, PIN_PROBE_TX);
    logger.log_I(time, "Probe type: KHDTK");
#elif defined(PROBE_NEW)
    ProbeNew probe(PIN_PROBE_RX, PIN_PROBE_TX);
    logger.log_I(time, "Probe type: NEW");
#endif

    Display display(PIN_SCREEN_SDA, PIN_SCREEN_SCL);
    if (!display.isOK()) logger.log_E(time, "Error! Display is not ready!");
    else logger.log_I(time, "Display is ready!");
    display.display_splash_screen();
    logger.log_I(time, "Splash screen displayed.");

    logger.log_I(time, "Sampling soilData...");
    SoilData soilData;
    Probe::ErrorCodes probeErr = probe.sample(soilData);
    if (probeErr == Probe::PROBE_ERROR) logger.log_E(time, "Error! Internal probe error!");
    else if(probeErr == Probe::NO_PROBE) logger.log_E(time, "Error! No probe connected!");
    else logger.log_I(time, "Sampled soil data: ---- please implement!");
    display.display_data(soilData);
    logger.log_I(time, "Sampled soil data displayed.");

    // TODO: use RTC for storing and retreiving time
    // int resetInfo = esp_reset_reason();
    // if (resetInfo == ESP_RST_POWERON) {
    //     preferences.begin(epochNamespace, true);
    //     currentEpoch = preferences.getLong(epochKeyname, 0);
    //     preferences.end();
    // }
    
    // if (submitter.is_ready())
    //     currentEpoch = submitter.get_curr_epoch();

    dataTable.push(SoilReading(soilData, currentEpoch));
    logger.log_I(time, "Sampled soil data pushed to file.");

#ifdef DEBUG
    info_soil_data(soilData);
#endif

    logger.log_I(time, "Sending data to server...");
    int responseCode = submitter.submit_reading(dataTable);
    if (responseCode == HTTP_CODE_OK) {
        logger.log_I(time, "Data send successful!\n");
    } else {
        logger.log_E(time, "Error! Data send failed! Error code: ---- please implement!");
    }

    logger.log_I(time, "Sleeping...");
    sleep(display, submitter);
}


void loop()
{
}

