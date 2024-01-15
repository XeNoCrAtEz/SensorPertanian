#ifndef CONNECTION_H_
#define CONNECTION_H_


#include <Arduino.h>
#include "soil_data.h"
#include "dataTable.h"
#include "RTCModule.h"


#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJSON.h>
#include <NTPClient.h>


class Submitter {
public:
    // class status codes
    enum Status {
        READY,
        NO_CONNECTION,
        UNKNOWN_ERROR,
    };

    // operation status codes
    enum OpStatus {
        SUCCESS,
        STATUS_NO_CONNECTION,
        STATUS_NO_TIME,
        UPLOAD_FAILED,
        STATUS_ERROR,
    };


protected:
    enum SubmitterParams {
        MAX_REATTEMPT = 10,
        REATTEMPT_DELAY = 1000,   // ms
    };
    

    const char SERVERNAME[16] = "tsth2.del.ac.id";
    const uint16_t PORT = 443;
    const char SUBMIT_RESOURCE[25] = "/Sensor/store_Sensor.php";

    Status m_status = UNKNOWN_ERROR;
    bool m_timeAvailable = false;


public:
    virtual OpStatus submit_reading(SoilReading& soilReading) = 0;
    virtual OpStatus submit_reading(SoilDataTable& dataTable) = 0;

    virtual OpStatus get_current_time(RtcDateTime& time) = 0;
    bool is_time_available();

    Status status();


};


class SubmitterWiFi : public Submitter {
private:
    const char WIFI_SSID[11] = "KHDTK_JAYA";
    const char WIFI_PASS[11] = "khdtk_jaya";


public:
    SubmitterWiFi();

    OpStatus submit_reading(SoilReading& soilReading) override;
    OpStatus submit_reading(SoilDataTable& dataTable) override;

    OpStatus get_current_time(RtcDateTime& time) override;


};


#define TINY_GSM_MODEM_SIM800
#include <TinyGSM.h>


class SubmitterGSM : public Submitter {
private:
    enum SubmitterParams {
        BAUDRATE = 115200,
    };

    const char APN[14] = "internet"; // APN Telkomsel IoT
    const char GPRS_USER[1] = "";
    const char GPRS_PASS[1] = "";

    HardwareSerial m_serialAT;
    TinyGsm m_modem;


public:
    SubmitterGSM(int rx, int tx, int HWSerialNum=1);

    OpStatus submit_reading(SoilReading& soilReading) override;
    OpStatus submit_reading(SoilDataTable& dataTable) override;

    OpStatus get_current_time(RtcDateTime& time) override;


};


#endif