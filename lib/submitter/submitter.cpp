#include "submitter.h"


void populate(DynamicJsonDocument& json, const SoilReading& soilReading) {
    json["ID"] = SENSOR_ID;
    
    JsonArray dataArr = json.createNestedArray("data");
    
    JsonObject rowJson = dataArr.createNestedObject();
    rowJson["timestamp"] = RtcDateTime_to_Str(RtcDateTime(soilReading.m_epoch));
    rowJson["N"] = soilReading.m_soilData.nitrogen;
    rowJson["P"] = soilReading.m_soilData.phosphorus;
    rowJson["K"] = soilReading.m_soilData.kalium;
    rowJson["pH"] = soilReading.m_soilData.pH;
    rowJson["temp"] = soilReading.m_soilData.temperature;
    rowJson["hum"] = soilReading.m_soilData.humidity;
    rowJson["EC"] = soilReading.m_soilData.EC;
    rowJson["salt"] = soilReading.m_soilData.salt;
}


void populate(DynamicJsonDocument& json, SoilDataTable& dataTable) {
    json["ID"] = SENSOR_ID;
    
    JsonArray dataArr = json.createNestedArray("data");
    
    uint16_t totalData = 0;
    SoilReading* soilReadings = nullptr;
    dataTable.load_all(soilReadings, totalData);
    for (int i = 0; i < totalData; i++) {
        SoilReading row = soilReadings[i];

        JsonObject rowJson = dataArr.createNestedObject();
        rowJson["timestamp"] = RtcDateTime_to_Str(RtcDateTime(row.m_epoch));
        rowJson["N"] = row.m_soilData.nitrogen;
        rowJson["P"] = row.m_soilData.phosphorus;
        rowJson["K"] = row.m_soilData.kalium;
        rowJson["pH"] = row.m_soilData.pH;
        rowJson["temp"] = row.m_soilData.temperature;
        rowJson["hum"] = row.m_soilData.humidity;
        rowJson["EC"] = row.m_soilData.EC;
        rowJson["salt"] = row.m_soilData.salt;
    }

    delete[] soilReadings;
}


bool Submitter::is_time_available() {
    return m_timeAvailable;
}


Submitter::Status Submitter::status() {
    return m_status;
}


// ---------------------------- Submitter WiFi ------------------------------
SubmitterWiFi::SubmitterWiFi()
{
    WiFi.mode(WIFI_STA);
    
    if (WiFi.SSID() != WIFI_SSID) {
        WiFi.persistent(true);
        WiFi.setAutoConnect(true);
        WiFi.setAutoReconnect(true);
        WiFi.begin(WIFI_SSID, WIFI_PASS);   // connect ke router
    }

    log_i("Connecting to %s", WIFI_SSID);
    for(int attempts = 0; WiFi.status() != WL_CONNECTED; attempts++) {
        if (attempts >= MAX_REATTEMPT) {
            log_e("Cannot connect to WiFi!");
            m_status = NO_CONNECTION;
            return;
        }
        WiFi.disconnect();
        WiFi.reconnect();
        delay(REATTEMPT_DELAY);
    }
    log_i("Connection Successful!");

    m_status = READY;

    RtcDateTime testTime;
    if (get_current_time(testTime) == SUCCESS) m_timeAvailable = true;
}


Submitter::OpStatus SubmitterWiFi::submit_reading(SoilReading& soilReading, int& responseCode) {
    if (m_status == NO_CONNECTION) return STATUS_NO_CONNECTION;
    if (m_status != READY) return STATUS_ERROR;

    String Link;
    Link = "https://" + String(SERVERNAME) + String(SUBMIT_RESOURCE);
    HTTPClient http;
    http.begin(Link);
    http.addHeader("Content-Type", "application/json");

    DynamicJsonDocument data(32 + 1 * 256);     // based on this calculator https://arduinojson.org/v6/assistant/
    populate(data, soilReading);
    String dataStr;
    serializeJson(data, dataStr);
    
#ifdef DEBUG
    serializeJsonPretty(data, Serial);
#endif

    responseCode = http.POST(dataStr);

    http.end();

    if (responseCode == 200) return SUCCESS;

    return UPLOAD_FAILED;
}


Submitter::OpStatus SubmitterWiFi::submit_reading(SoilDataTable& dataTable, int& responseCode) {
    if (m_status == NO_CONNECTION) return STATUS_NO_CONNECTION;
    if (m_status != READY) return STATUS_ERROR;

    String Link;
    Link = "https://" + String(SERVERNAME) + String(SUBMIT_RESOURCE);
    HTTPClient http;
    http.begin(Link);
    http.addHeader("Content-Type", "application/json");

    DynamicJsonDocument data(32 + dataTable.get_count() * 256);     // based on this calculator https://arduinojson.org/v6/assistant/
    populate(data, dataTable);
    String dataStr;
    serializeJson(data, dataStr);
    
#ifdef DEBUG
    serializeJsonPretty(data, Serial);
#endif

    responseCode = http.POST(dataStr);

    http.end();

    if (responseCode == 200) {
        dataTable.clear();
        return SUCCESS;
    }

    return UPLOAD_FAILED;
}


Submitter::OpStatus SubmitterWiFi::get_current_time(RtcDateTime& time) {
    if (m_status == NO_CONNECTION) return STATUS_NO_CONNECTION;
    if (m_status != READY) return STATUS_ERROR;

    WiFiUDP ntpUDP;
    NTPClient timeClient(ntpUDP, "pool.ntp.org", 7*3600);
    timeClient.begin();

    for(int attempts = 0; !timeClient.update(); attempts++) {
        Serial.println("Cannot update time from network!");
        if (attempts >= MAX_REATTEMPT) {
            time = RtcDateTime();
            return STATUS_NO_TIME;
        }
        Serial.println("Re-attempt time update... (re-attempt: " + String(attempts) + ")");
        
        timeClient.forceUpdate();
    }
    
    uint32_t epoch = timeClient.getEpochTime();
    time.InitWithUnix32Time(epoch);
    return SUCCESS;
}


// ---------------------------- Submitter GSM ------------------------------
SubmitterGSM::SubmitterGSM(int rx, int tx, int HWSerialNum)
        : m_serialAT(HWSerialNum), m_modem(m_serialAT) {
    m_serialAT.begin(BAUDRATE, SERIAL_8N1, rx, tx);
    if (!m_modem.init()) {
        Serial.println("Fatal Error! Failed to init GSM module!");
        m_status = NO_CONNECTION;
        return;
    }
    if (!m_modem.waitForNetwork()) {
        Serial.println("Fatal Error! Cannot connect to Network!");
        m_status = NO_CONNECTION;
        return;
    }
    if (!m_modem.gprsConnect(APN, GPRS_USER, GPRS_PASS)) {
        Serial.println("Fatal Error! Cannot connect to GPRS!");
        m_status = NO_CONNECTION;
        return;
    }

    if (!m_modem.isGprsConnected()) {
        m_status = NO_CONNECTION;
        return;
    }

    m_status = READY;
    RtcDateTime testTime;
    if (get_current_time(testTime) == SUCCESS) m_timeAvailable = true;
}


Submitter::OpStatus SubmitterGSM::submit_reading(SoilReading& soilReading, int& responseCode) {
    if (m_status == NO_CONNECTION) return STATUS_NO_CONNECTION;
    if (m_status != READY) return STATUS_ERROR;
    
    String Link;
    Link = "https://" + String(SERVERNAME) + String(SUBMIT_RESOURCE);
    // Link = "http://" + String(SERVERNAME) + "/Sensor/kirimdata.php";

    // try ping first
    TinyGsmClientSecure client = TinyGsmClientSecure(m_modem);
    if (!client.connect(SERVERNAME, PORT)) {
        Serial.println("Fatal Error! Server is down!");
        return UPLOAD_FAILED;
    }

    DynamicJsonDocument data(32 + 1 * 256);     // based on this calculator https://arduinojson.org/v6/assistant/
    populate(data, soilReading);
    String dataStr;
    serializeJson(data, dataStr);

#ifdef DEBUG
    serializeJsonPretty(data, Serial);
#endif

    client.print(String("POST ") + SUBMIT_RESOURCE + " HTTP/1.1\r\n");
    client.print(String("Host: ") + SERVERNAME + "\r\n");
    client.println("Connection: keep-alive");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(dataStr.length());
    client.println();
    client.println(dataStr);

    responseCode = get_response_code(client);

    client.stop();

    if (responseCode == 200) return SUCCESS;

    return UPLOAD_FAILED;
}


Submitter::OpStatus SubmitterGSM::submit_reading(SoilDataTable& dataTable, int& responseCode) {
    if (m_status == NO_CONNECTION) return STATUS_NO_CONNECTION;
    if (m_status != READY) return STATUS_ERROR;
    
    String Link;
    Link = "https://" + String(SERVERNAME) + String(SUBMIT_RESOURCE);
    // Link = "http://" + String(SERVERNAME) + "/Sensor/kirimdata.php";

    // try ping first
    TinyGsmClientSecure client = TinyGsmClientSecure(m_modem);
    if (!client.connect(SERVERNAME, PORT)) {
        Serial.println("Fatal Error! Server is down!");
        return UPLOAD_FAILED;
    }

    DynamicJsonDocument data(32 + dataTable.get_count() * 256);     // based on this calculator https://arduinojson.org/v6/assistant/
    populate(data, dataTable);
    String dataStr;
    serializeJson(data, dataStr);
    
#ifdef DEBUG
    serializeJsonPretty(data, Serial);
#endif

    client.print(String("POST ") + SUBMIT_RESOURCE + " HTTP/1.1\r\n");
    client.print(String("Host: ") + SERVERNAME + "\r\n");
    client.println("Connection: keep-alive");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(dataStr.length());
    client.println();
    client.println(dataStr);

    responseCode = get_response_code(client);

    client.stop();

    if (responseCode == 200) {
        dataTable.clear();
        return SUCCESS;
    }

    return UPLOAD_FAILED;
}


Submitter::OpStatus SubmitterGSM::get_current_time(RtcDateTime& time) {
    if (m_status == NO_CONNECTION) return STATUS_NO_CONNECTION;
    if (m_status != READY) return STATUS_ERROR;

    int year, month, dayOfMonth, hour, minute, second;
    float timezone = 0;
    if (m_modem.getNetworkTime(&year, &month, &dayOfMonth, &hour, &minute, &second, &timezone)) {
        time = RtcDateTime(year, month, dayOfMonth, hour, minute, second);
        return SUCCESS;
    }

    time = RtcDateTime(0);
    return STATUS_NO_TIME;
}


int get_response_code(TinyGsmClientSecure& client) {
    // Wait for data to arrive
    uint32_t startS = millis();
    while (client.connected() && !client.available() && millis() - startS < 30000L) delay(100);

    // Read data
    char responseCodeStr[4] = "";
    if (client.connected() && client.available()) {
        char c = '\0';
        while(' ' != client.read());    // skip "HTTP/1.1 "
        for(int i = 0; i < 3; i++) responseCodeStr[i] = client.read();      // get 3 digit code
        responseCodeStr[3] = '\0';
    }

    return atoi(responseCodeStr);
}