#ifndef CONNECTION_H_
#define CONNECTION_H_


#include <Arduino.h>
#include "soil_data.h"
#include "dataTable.h"


#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJSON.h>
#include <NTPClient.h>
#include <time.h>


class Submitter {
protected:
    enum SubmitterParams {
        MAX_REATTEMPT = 10,
        REATTEMPT_DELAY = 1000,   // ms
    };
    

    const char SERVERNAME[16] = "tsth2.del.ac.id";
    const uint16_t PORT = 443;
    const char SUBMIT_RESOURCE[25] = "/Sensor/store_Sensor.php";

    bool connected = false;


public:
    virtual int submit_reading(SoilReading& soilReading) = 0;
    virtual int submit_reading(SoilDataTable& dataTable) = 0;

    virtual unsigned long get_curr_epoch() = 0;
    virtual String to_timestamp(unsigned long epoch) = 0;

    bool is_connected();


};


class SubmitterWiFi : public Submitter {
private:
    // NTP server to request epoch time
    const char* NTP_SERVER = "pool.ntp.org";

    const char WIFI_SSID[11] = "KHDTK_JAYA";
    const char WIFI_PASS[11] = "khdtk_jaya";


public:
    SubmitterWiFi();
    int submit_reading(SoilReading& soilReading);
    int submit_reading(SoilDataTable& dataTable);

    unsigned long get_curr_epoch();
    String to_timestamp(unsigned long epoch);


};


#define TINY_GSM_MODEM_SIM800
#include <TinyGSM.h>


class SubmitterGSM : public Submitter {
private:
    enum SubmitterParams {
        BAUDRATE = 115200,
    };

    const char apn[14] = "internet"; // APN Telkomsel IoT
    const char gprsUser[1] = "";
    const char gprsPass[1] = "";

    HardwareSerial serialAT;
    TinyGsm modem;


public:
    SubmitterGSM(int rx, int tx, int HWSerialNum=1);
    int submit_reading(SoilReading& soilReading);
    int submit_reading(SoilDataTable& dataTable);

    unsigned long get_curr_epoch();
    String to_timestamp(unsigned long epoch);


};


#endif