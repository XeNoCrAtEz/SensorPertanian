#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <WiFi.h>
#include <HTTPClient.h>
#include <JSON.h>
#include <NTPClient.h>

#include "soil_data.h"
#include "dataTable.h"


class Submitter {
private:
    const int MAX_REATTEMPT = 10;
    const int REATTEMPT_DELAY = 1000;   // ms

    const char SERVERNAME[18] = "raspberrypi.local";

    const char WIFI_SSID[11] = "KHDTK_JAYA";
    const char WIFI_PASS[11] = "khdtk_jaya";

    // const char WIFI_SSID[] = "XeNoCrAtEz_";
    // const char WIFI_PASS[] = "aptxsxfc5";

    // const char WIFI_SSID[] = "Aku&Kamu";
    // const char WIFI_PASS[] = "122333444455555";

    bool connected = false;
    const int SENSOR_ID;

public:
    Submitter(const int sensorID);
    int submit_table(SoilDataTable& dataTable);

    unsigned long get_curr_epoch();

    void test();

    bool is_connected();
};


#endif