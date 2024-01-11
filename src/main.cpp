#include "main.h"

void setup() {
    // begin USB Serial
    Serial.begin(115200);

// logger.log_I("Initializing submitter...");
#if defined(USE_WIFI)
    SubmitterWiFi submitter;
// if (!submitter.is_ready()) logger.log_E("Error! WiFi submitter is not ready!");
// else logger.log_I("WiFi submitter is ready!");
#elif defined(USE_GSM)
    SubmitterGSM submitter(PIN_GSM_RX, PIN_GSM_TX);
// if (!submitter.is_ready()) logger.log_E("Error! GSM submitter is not ready!");
// else logger.log_I("GSM submitter is ready!");
#endif
RTC rtc = RTC(PIN_RTC_DATA, PIN_RTC_CLK, PIN_RTC_RST);
TimeClass timeClass = TimeClass(rtc, submitter);
Logger logger = Logger(timeClass, true);

logger.log_I("Starting... Sensor-" + String(SENSOR_ID));

logger.log_I("Checking battery and solar cell voltage...");
    BatteryMonitor battMon(PIN_VOLT_BAT, MIN_VOLT_LIPO, MAX_VOLT_LIPO);
    VoltageMonitor solarCellMon(PIN_VOLT_SC);
    battMon.begin(ESP32_REF_VOLTAGE, VOLT_MON_DIVIDER_RATIO);
    solarCellMon.begin(ESP32_REF_VOLTAGE, VOLT_MON_DIVIDER_RATIO);
logger.log_I("Battery voltage: " + String(battMon.voltage()) + " mV (" + String(battMon.level()) + "%)");
logger.log_I("Solar cell voltage: " + String(solarCellMon.voltage()) + " mV");
    if (battMon.level() == 0) {
logger.log_E("Error! Battery depleted! Sleeping...");
        sleep(timeClass);
    }

logger.log_I("Initializing data table...");
    SoilDataTable dataTable;
if (!dataTable.is_ready()) logger.log_E("Error! dataTable not ready!");
else logger.log_I("dataTable is ready!");


logger.log_I("Initializing probe...");
#if defined(PROBE_DEFAULT)
    ProbeDefault probe(PIN_PROBE_RX, PIN_PROBE_TX);
logger.log_I("Probe type: DEFAULT");
#elif defined(PROBE_KHDTK)
    ProbeKHDTK probe(PIN_PROBE_RX, PIN_PROBE_TX);
logger.log_I("Probe type: KHDTK");
#elif defined(PROBE_NEW)
    ProbeNew probe(PIN_PROBE_RX, PIN_PROBE_TX);
logger.log_I("Probe type: NEW");
#endif

logger.log_I("Initializing display...");
    Display display(PIN_SCREEN_SDA, PIN_SCREEN_SCL);
if (!display.isOK()) logger.log_E("Error! Display is not ready!");
else logger.log_I("Display is ready!");
logger.log_I("Displaying splash screen...");
    display.display_splash_screen();
logger.log_I("Splash screen displayed.");

logger.log_I("Sampling soilData...");
    SoilData soilData;
    Probe::ErrorCodes probeErr = probe.sample(soilData);
if (probeErr == Probe::PROBE_ERROR) logger.log_E("Error! Internal probe error!");
else if(probeErr == Probe::NO_PROBE) logger.log_E("Error! No probe connected!");
else logger.log_I("Sampled soil data:\n" + soilData.toString());

logger.log_I("Displaying soil data to display...");
    display.display_data(soilData);
logger.log_I("Sampled soil data displayed.");

logger.log_I("Getting time from network...");
    RtcDateTime currentDateTime = submitter.get_current_time();
    if (currentDateTime.TotalSeconds() == 0) {
logger.log_E("Error! Cannot get time from network!");
logger.log_I("Using time from RTC.");
        currentDateTime = rtc.get_date_time();
logger.log_I("Time from RTC: " + String(currentDateTime.TotalSeconds()) + " = " + RtcDateTime_to_Str(currentDateTime));
    } else {
logger.log_I("Time from network: " + String(currentDateTime.TotalSeconds()) + " = " + RtcDateTime_to_Str(currentDateTime));
        rtc.set_date_time(currentDateTime);
    }
logger.log_I("RTC time set to network time.");

logger.log_I("Uploading data...");
logger.log_I("Checking for saved data...");
    if (dataTable.is_empty()) {
logger.log_I("Table is empty.");
logger.log_I("Sending single data to server...");
        SoilReading currentReading = SoilReading(soilData, currentDateTime.TotalSeconds());
        int responseCode = submitter.submit_reading(currentReading);
        if (responseCode == HTTP_CODE_OK) {
logger.log_I("Data send successful!\n");
            } else {
logger.log_E("Error! Data send failed! Error code: " + String(responseCode));
        }
    } else {
logger.log_I("Saved data available.");
logger.log_I("Pushing sampled soil data to file...");
        dataTable.push(SoilReading(soilData, currentDateTime.TotalSeconds()));
logger.log_I("Sampled soil data pushed to file.");

logger.log_I("Sending data to server...");
        int responseCode = submitter.submit_reading(dataTable);
        if (responseCode == HTTP_CODE_OK) {
logger.log_I("Data send successful!\n");
        } else {
logger.log_E("Error! Data send failed! Error code: " + String(responseCode));
        }
    }

logger.log_I("Clearing display...");
    display.clear_display();
logger.log_I("Display cleared.");
logger.log_I("Sleeping...");
    sleep(timeClass);
}


void loop()
{
}

