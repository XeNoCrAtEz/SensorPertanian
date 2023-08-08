#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJSON.h>
#include <NTPClient.h>
#include "time.h"

#include "soil_data.h"
#include "dataTable.h"


class Submitter {
private:
    const int MAX_REATTEMPT = 10;
    const int REATTEMPT_DELAY = 1000;   // ms

    // NTP server to request epoch time
    const char* NTP_SERVER = "pool.ntp.org";

    const char SERVERNAME[18] = "raspberrypi.local";

    const char WIFI_SSID[11] = "KHDTK_JAYA";
    const char WIFI_PASS[11] = "khdtk_jaya";

    bool connected = false;
    const int SENSOR_ID;

public:
    Submitter(const int sensorID);
    int submit_table(SoilDataTable& dataTable);

    unsigned long get_curr_epoch();
    String to_timestamp(unsigned long epoch);

    bool is_connected();
};


#endif