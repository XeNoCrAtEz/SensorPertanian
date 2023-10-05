#include "main.h"


RTC_DATA_ATTR SoilDataTable dataTable;

Preferences preferences;

RTC_DATA_ATTR time_t currentEpoch = 0;


void setup() {
    // begin USB Serial
    Serial.begin(115200);

    Submitter submitter(SENSOR_ID);

#if defined(PROBE_DEFAULT)
    ProbeDefault probe(PROBE_RX_PIN, PROBE_TX_PIN);
#elif defined(PROBE_KHDTK)
    ProbeKHDTK probe(PROBE_RX_PIN, PROBE_TX_PIN);
#elif defined(PROBE_NEW)
    ProbeNew probe(PROBE_RX_PIN, PROBE_TX_PIN);
#endif

    setup_display();
    display_splash_screen();

    SoilData soilData = probe.sample();
    display_data(soilData);

    int resetInfo = esp_reset_reason();
    if (resetInfo == ESP_RST_POWERON) {
        preferences.begin(epochNamespace, true);
        currentEpoch = preferences.getLong(epochKeyname, 0);
        preferences.end();
    }
    
    if (submitter.is_connected())
        currentEpoch = submitter.get_curr_epoch();

    dataTable.push(soilData, currentEpoch);

#ifdef DEBUG
    info_soil_data(soilData);
#endif

    int responseCode = submitter.submit_table(dataTable);
    if (responseCode == HTTP_CODE_OK) Serial.println("Data send successful!\n");
    else Serial.println("Data send failed!\n");

    Serial.println("Sensor now sleep...");
    sleep(submitter);
}


void loop()
{
}


void sleep(Submitter &submitter) {
    clear_display();

    auto sleepTime = get_sleep_seconds(submitter);
    currentEpoch += sleepTime;      // predicted epoch for next sleep cycle

    preferences.begin(epochNamespace, false);
    preferences.putLong(epochKeyname, currentEpoch);
    preferences.end();

#ifdef DEBUG
    Serial.print("Time to sleep: ");
    Serial.println(sleepTime);
#endif

    esp_deep_sleep(sleepTime * uS_TO_S_FACTOR);
}


uint64_t get_sleep_seconds(Submitter &submitter) {
    struct tm *current_tm = localtime(&currentEpoch);
    struct tm current_H_M = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    current_H_M.tm_hour = current_tm->tm_hour;
    current_H_M.tm_min = current_tm->tm_min;
    current_H_M.tm_sec = current_tm->tm_sec;

    double min_sec = INFINITY;
    for (int i = 0; i < SCHEDULE_LENGTH; i++) {
        double diff = difftime(mktime(&schedule[i]), mktime(&current_H_M));
        if (diff < 0) continue;                         // skip jadwal yang sudah lewat
        else if (diff < min_sec) min_sec = diff;
    }
    // jika tidak ditemukan jadwal selanjutnya, paksa tidur 4 jam
    if (min_sec == 0 || min_sec == INFINITY)
        min_sec = 4 * 3600;

    return min_sec;
}