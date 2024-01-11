#include "submitter.h"


bool Submitter::is_time_available() {
    return timeAvailable;
}


bool Submitter::is_ready() {
    return ready;
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

    Serial.print("Connecting to ");
    Serial.print(WIFI_SSID);
    for(int attempts = 0; WiFi.status() != WL_CONNECTED; attempts++) {
        Serial.print(".");
        if (attempts >= MAX_REATTEMPT) {
            Serial.println("\nCannot connect to WiFi!");
            ready = false;
            return;
        }
        WiFi.disconnect();
        WiFi.reconnect();
        delay(REATTEMPT_DELAY);
    }
    Serial.println("\nConnection Successful!\n");

#ifdef DEBUG
    Serial.print("Sensor IP Address : ");
    Serial.println(WiFi.localIP());
	Serial.println();
#endif

    ready = true;

    if (get_current_time().TotalSeconds() != 0) timeAvailable = true;
}


int SubmitterWiFi::submit_reading(SoilReading& soilReading) {
    if (!is_ready()) return 0;
    
    String Link;
    Link = "https://" + String(SERVERNAME) + String(SUBMIT_RESOURCE);
    HTTPClient http;
    http.begin(Link);
    http.addHeader("Content-Type", "application/json");

    DynamicJsonDocument data(32 + 1 * 170);     // based on this calculator https://arduinojson.org/v6/assistant/
    
    data["ID"] = SENSOR_ID;
    
    JsonArray dataArr = data.createNestedArray("data");
    
    JsonObject rowJson = dataArr.createNestedObject();
    rowJson["timestamp"] = RtcDateTime_to_Str(RtcDateTime(soilReading.epoch));
    rowJson["N"] = soilReading.soilData.nitrogen;
    rowJson["P"] = soilReading.soilData.phosphorus;
    rowJson["K"] = soilReading.soilData.kalium;
    rowJson["pH"] = soilReading.soilData.pH;
    rowJson["temp"] = soilReading.soilData.temperature;
    rowJson["hum"] = soilReading.soilData.humidity;
    rowJson["EC"] = soilReading.soilData.EC;
    rowJson["salt"] = soilReading.soilData.salt;

    String dataStr;
    serializeJson(data, dataStr);
    
#ifdef DEBUG
    serializeJsonPretty(data, Serial);
#endif

    int responseCode = http.POST(dataStr);

    http.end();

    return responseCode;
}


int SubmitterWiFi::submit_reading(SoilDataTable& dataTable) {
    if (!is_ready()) return 0;
    
    String Link;
    Link = "https://" + String(SERVERNAME) + String(SUBMIT_RESOURCE);
    HTTPClient http;
    http.begin(Link);
    http.addHeader("Content-Type", "application/json");

    DynamicJsonDocument data(32 + dataTable.get_count() * 170);     // based on this calculator https://arduinojson.org/v6/assistant/
    
    data["ID"] = SENSOR_ID;
    
    JsonArray dataArr = data.createNestedArray("data");
    
    uint16_t totalData = 0;
    SoilReading* soilReadings;
    dataTable.pop_all(soilReadings, totalData);
    for (int i = 0; i < totalData; i++) {
        SoilReading row = soilReadings[i];

        JsonObject rowJson = dataArr.createNestedObject();
        rowJson["timestamp"] = RtcDateTime_to_Str(RtcDateTime(row.epoch));
        rowJson["N"] = row.soilData.nitrogen;
        rowJson["P"] = row.soilData.phosphorus;
        rowJson["K"] = row.soilData.kalium;
        rowJson["pH"] = row.soilData.pH;
        rowJson["temp"] = row.soilData.temperature;
        rowJson["hum"] = row.soilData.humidity;
        rowJson["EC"] = row.soilData.EC;
        rowJson["salt"] = row.soilData.salt;
    }

    delete[] soilReadings;

    String dataStr;
    serializeJson(data, dataStr);
    
#ifdef DEBUG
    serializeJsonPretty(data, Serial);
#endif

    int responseCode = http.POST(dataStr);

    http.end();

    return responseCode;
}


RtcDateTime SubmitterWiFi::get_current_time() {
    WiFiUDP ntpUDP;
    NTPClient timeClient(ntpUDP, "pool.ntp.org", 7*3600);
    timeClient.begin();

    for(int attempts = 0; !timeClient.update(); attempts++) {
        Serial.println("Cannot update time from network!");
        if (attempts >= MAX_REATTEMPT) {
            return RtcDateTime();
        }
        Serial.print("Re-attempt time update... (re-attempt: " + String(attempts) + ")");
        
        timeClient.forceUpdate();
    }
    
    uint32_t epoch = timeClient.getEpochTime();
    RtcDateTime now;
    now.InitWithUnix32Time(epoch);
    return now;
}


// ---------------------------- Submitter GSM ------------------------------
SubmitterGSM::SubmitterGSM(int rx, int tx, int HWSerialNum)
        : serialAT(HWSerialNum), modem(serialAT) {
    serialAT.begin(BAUDRATE, SERIAL_8N1, rx, tx);
    if (!modem.init()) {
        Serial.println("Fatal Error! Failed to init GSM module!");
        return;
    }
    if (!modem.waitForNetwork()) {
        Serial.println("Fatal Error! Cannot connect to Network!");
        return;
    }
    if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
        Serial.println("Fatal Error! Cannot connect to GPRS!");
        return;
    }
    ready = modem.isGprsConnected();

    if (get_current_time().TotalSeconds() != 0) timeAvailable = true;
}


int SubmitterGSM::submit_reading(SoilReading& soilReading) {
    if (!is_ready()) return 0;
    
    String Link;
    Link = "https://" + String(SERVERNAME) + String(SUBMIT_RESOURCE);
    // Link = "http://" + String(SERVERNAME) + "/Sensor/kirimdata.php";

    // try ping first
    TinyGsmClientSecure client = TinyGsmClientSecure(modem);
    if (!client.connect(SERVERNAME, PORT)) {
        Serial.println("Fatal Error! Server is down!");
        return 0;
    }

    DynamicJsonDocument data(32 + 1 * 170);     // based on this calculator https://arduinojson.org/v6/assistant/
    
    data["ID"] = SENSOR_ID;
    
    JsonArray dataArr = data.createNestedArray("data");
    
    JsonObject rowJson = dataArr.createNestedObject();
    rowJson["timestamp"] = RtcDateTime_to_Str(RtcDateTime(soilReading.epoch));
    rowJson["N"] = soilReading.soilData.nitrogen;
    rowJson["P"] = soilReading.soilData.phosphorus;
    rowJson["K"] = soilReading.soilData.kalium;
    rowJson["pH"] = soilReading.soilData.pH;
    rowJson["temp"] = soilReading.soilData.temperature;
    rowJson["hum"] = soilReading.soilData.humidity;
    rowJson["EC"] = soilReading.soilData.EC;
    rowJson["salt"] = soilReading.soilData.salt;

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

    // Wait for data to arrive
    uint32_t startS = millis();
    while (client.connected() && !client.available() && millis() - startS < 30000L) {
        delay(100);
    };

    // Read data
    char responseCodeStr[4] = "";
    if (client.connected() && client.available()) {
        char c;
        for(int i = 0; i < 9; i++) c = client.read();   // skip "HTTP/1.1 "
        for(int i = 0; i < 3; i++) responseCodeStr[i] = client.read();
        responseCodeStr[3] = '\0';
    }

    int responseCode = atoi(responseCodeStr);

    client.stop();

    return responseCode;
}


int SubmitterGSM::submit_reading(SoilDataTable& dataTable) {
    if (!is_ready()) return 0;
    
    String Link;
    Link = "https://" + String(SERVERNAME) + String(SUBMIT_RESOURCE);
    // Link = "http://" + String(SERVERNAME) + "/Sensor/kirimdata.php";

    // try ping first
    TinyGsmClientSecure client = TinyGsmClientSecure(modem);
    if (!client.connect(SERVERNAME, PORT)) {
        Serial.println("Fatal Error! Server is down!");
        return 0;
    }

    DynamicJsonDocument data(32 + dataTable.get_count() * 170);     // based on this calculator https://arduinojson.org/v6/assistant/
    
    data["ID"] = SENSOR_ID;
    
    JsonArray dataArr = data.createNestedArray("data");
    
    uint16_t totalData = 0;
    SoilReading* soilReadings;
    dataTable.pop_all(soilReadings, totalData);
    for (int i = 0; i < totalData; i++) {
        SoilReading row = soilReadings[i];

        JsonObject rowJson = dataArr.createNestedObject();
        rowJson["timestamp"] = RtcDateTime_to_Str(RtcDateTime(row.epoch));
        rowJson["N"] = row.soilData.nitrogen;
        rowJson["P"] = row.soilData.phosphorus;
        rowJson["K"] = row.soilData.kalium;
        rowJson["pH"] = row.soilData.pH;
        rowJson["temp"] = row.soilData.temperature;
        rowJson["hum"] = row.soilData.humidity;
        rowJson["EC"] = row.soilData.EC;
        rowJson["salt"] = row.soilData.salt;
    }

    delete[] soilReadings;

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

    // Wait for data to arrive
    uint32_t startS = millis();
    while (client.connected() && !client.available() && millis() - startS < 30000L) {
        delay(100);
    };

    // Read data
    char responseCodeStr[4] = "";
    if (client.connected() && client.available()) {
        char c;
        for(int i = 0; i < 9; i++) c = client.read();   // skip "HTTP/1.1 "
        for(int i = 0; i < 3; i++) responseCodeStr[i] = client.read();
        responseCodeStr[3] = '\0';
    }

    int responseCode = atoi(responseCodeStr);

    client.stop();

    return responseCode;
}


RtcDateTime SubmitterGSM::get_current_time() {
    int year, month, dayOfMonth, hour, minute, second;
    float timezone = 0;
    if (modem.getNetworkTime(&year, &month, &dayOfMonth, &hour, &minute, &second, &timezone))
        return RtcDateTime(year, month, dayOfMonth, hour+timezone, minute, second);

    return RtcDateTime(0);
}