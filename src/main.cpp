#include "main.h"


void setup() {
    // begin USB Serial
    Serial.begin(115200);

    Switch modulesSwitch(PIN_MODULES_SW);
    modulesSwitch.on();

    RTC rtc = RTC(PIN_RTC_DATA, PIN_RTC_CLK, PIN_RTC_RST);

#if defined(USE_WIFI)
    SubmitterWiFi submitter;
#elif defined(USE_GSM)
    SubmitterGSM submitter(PIN_GSM_RX, PIN_GSM_TX, GSM_HW_SERIAL_NUM);
#endif

    TimeClass timeClass = TimeClass(rtc, submitter);

    Logger logger = Logger(timeClass, true);

    BatteryMonitor battMon(
        PIN_VOLT_BAT, ESP32_REF_VOLTAGE, VOLT_MON_DIVIDER_RATIO,
        MIN_VOLT_LIPO, MAX_VOLT_LIPO,
        MEASUREMENT_UNCERTAINTY, ESTIMATION_UNCERTAINTY, PROCESS_NOISE);
    VoltageMonitor solarCellMon(
        PIN_VOLT_SC, ESP32_REF_VOLTAGE, VOLT_MON_DIVIDER_RATIO,
        MEASUREMENT_UNCERTAINTY, ESTIMATION_UNCERTAINTY, PROCESS_NOISE);

#ifdef USE_DISPLAY
    Display display(PIN_SCREEN_SDA, PIN_SCREEN_SCL);
#endif

    SoilDataTable dataTable;

    // MOSFET

    if      (logger.status() == Logger::PRINT_MODE) logger.log_I("Logging in print mode.");
    else if (logger.status() != Logger::READY) Serial.println("Error! Logger not ready! Status: " + String(logger.status()));
    else logger.log_I("Logger is ready!");

    logger.log_I("Starting Sensor-" + String(SENSOR_ID));

    if (rtc.status() != RTC::READY) logger.log_E("Error! RTC not ready! Status: " + String(rtc.status()));
    else logger.log_I("RTC ready!");

#if defined(USE_WIFI)
    logger.log_I("Using WiFi.");
#elif defined(USE_GSM)
    logger.log_I("Using GSM.");
#endif
    if (submitter.status() != Submitter::READY) logger.log_E("Error! Submitter not ready! Status: " + String(submitter.status()));
    else logger.log_I("Submitter ready!");

    if      (timeClass.status() == TimeClass::READY) logger.log_I("NTP and RTC ready.");
    else if (timeClass.status() == TimeClass::READY_NO_RTC) logger.log_E("Error! RTC not available!");
    else if (timeClass.status() == TimeClass::READY_NO_NTP) logger.log_E("Error! Network time not available!");
    else if (timeClass.status() == TimeClass::NO_TIME) logger.log_E("Error! No time available!");

    logger.log_I("Battery voltage: " + String(battMon.voltage()) + " mV (" + String(battMon.level()) + "%)");
    logger.log_I("Solar cell voltage: " + String(solarCellMon.voltage()) + " mV");
    if (battMon.level() == 0) {
        logger.log_E("Error! Battery depleted! Sleeping...");
        sleep(timeClass, logger);
    }

#ifdef USE_DISPLAY
    if (display.status() != Display::READY) logger.log_E("Error! Display is not ready! Status: " + String(display.status()));
    else logger.log_I("Display is ready!");
#endif

    if (dataTable.status() != SoilDataTable::READY) logger.log_E("Error! dataTable not ready! Status: " + String(dataTable.status()));
    else logger.log_I("dataTable is initialized and ready!");

#if defined(PROBE_DEFAULT)
    ProbeDefault probe(PIN_PROBE_RX, PIN_PROBE_TX, OTHER_HW_SERIAL_NUM);
    logger.log_I("Initialized probe type: DEFAULT");
#elif defined(PROBE_KHDTK)
    ProbeKHDTK probe(PIN_PROBE_RX, PIN_PROBE_TX, OTHER_HW_SERIAL_NUM);
    logger.log_I("Initialized probe type: KHDTK");
#elif defined(PROBE_NEW)
    ProbeNew probe(PIN_PROBE_RX, PIN_PROBE_TX, OTHER_HW_SERIAL_NUM);
    logger.log_I("Initialized probe type: NEW");
#endif
    if (probe.status() != Probe::READY) logger.log_E("Error! Probe not ready! Status: " + String(probe.status()));
    else logger.log_I("Probe ready!");

    SoilData soilData;
    Probe::OpStatus probeErr = probe.sample(soilData);
    logger.log_I("Sampled soil data:\n" + soilData.toString());

    RtcDateTime now;
    timeClass.get_date_time(now);
    SoilReading currentReading = SoilReading(soilData, now.TotalSeconds());

    if (dataTable.is_empty()) {
        logger.log_I("Table is empty. Sending current sample");
        int responseCode = -1;
        if (submitter.submit_reading(currentReading, responseCode) != Submitter::SUCCESS) {
            logger.log_E("Error! Data upload failed! HTTP Code: " + String(responseCode));
            dataTable.push(currentReading);
        }
        else logger.log_I("Data send successful!\n");
    } else {
        logger.log_I("Saved data available. Pushing sampled data...");
        dataTable.push(currentReading);

        int responseCode = -1;
        if (submitter.submit_reading(dataTable, responseCode) != Submitter::SUCCESS) logger.log_E("Error! Data upload failed! HTTP Code: " + String(responseCode));
        else logger.log_I("Data send successful!");
    }

#ifdef USE_DISPLAY
    display.display_splash_screen();
    logger.log_I("Splash screen displayed.");

    display.display_data(soilData);
    logger.log_I("Sampled soil data displayed.");

    display.clear_display();
    logger.log_I("Display cleared.");
#endif

    logger.log_I("Sleeping...");
    sleep(timeClass, logger);
}


void loop()
{
}