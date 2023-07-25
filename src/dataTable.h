#ifndef _DATA_TABLE_H_
#define _DATA_TABLE_H_

#include "soil_data.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <JSON.h>

const IPAddress IP_SERVER(192, 168, 160, 76);

const char WIFI_SSID[] = "XeNoCrAtEz_";
const char WIFI_PASS[] = "aptxsxfc5";

// const char WIFI_SSID[] = "Aku&Kamu";
// const char WIFI_PASS[] = "122333444455555";

class SoilReading {
public:
    // int timestamp;
    SoilData soilData;

    String to_json_string() {
        return  String("{\"timestamp\":") + // timestamp + String(",")
                String("\"N\":") + String(soilData.nitrogen) + String(",") +
                String("\"P\":") + String(soilData.phosphorus) + String(",") +
                String("\"K\":") + String(soilData.kalium) + String(",") +
                String("\"pH\":") + String(soilData.pH) + String(",") +
                String("\"Temp\":") + String(soilData.temperature) + String(",") +
                String("\"Hum\":") + String(soilData.humidity) + String(",") +
                String("\"EC\":") + String(soilData.EC) + String("}");
    }
};


class SoilDataTable {
private:
    static const int MAX_SIZE = 250;

    SoilReading readings[MAX_SIZE];
    int idx_in;
    int idx_out;
    int count;

public:
    SoilDataTable();
    // TODO: NTP server, pengambilan timestamp
    bool push(const SoilReading& soilData);
    bool pop(SoilReading& soilData);
    bool is_empty();
    bool is_full();
    int get_count();
    void clear();
    void inc_idx(int& idx);
    void dec_idx(int& idx);

    // data submission functions
    bool connect_wifi();
    int submit_table();


    // DEBUG
    void test();
};


#endif