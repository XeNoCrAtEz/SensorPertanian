#include "submitter.h"


bool Submitter::is_connected() {
    return connected;
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
            connected = false;
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

    connected = true;
    
    // 7*3600 set timezone to Jakarta
    configTime(7*3600, 0, NTP_SERVER);
    
    return;
}


int SubmitterWiFi::submit_reading(SoilReading& soilReading) {
    if (!is_connected()) return 0;
    
    String Link;
    Link = "https://" + String(SERVERNAME) + String(SUBMIT_RESOURCE);
    HTTPClient http;
    http.begin(Link);
    http.addHeader("Content-Type", "application/json");

    DynamicJsonDocument data(32 + 1 * 170);     // based on this calculator https://arduinojson.org/v6/assistant/
    
    data["ID"] = SENSOR_ID;
    
    JsonArray dataArr = data.createNestedArray("data");
    
    JsonObject rowJson = dataArr.createNestedObject();
    rowJson["timestamp"] = to_timestamp(soilReading.epoch);
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
    if (!is_connected()) return 0;
    
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
        rowJson["timestamp"] = to_timestamp(row.epoch);
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


unsigned long SubmitterWiFi::get_curr_epoch() {
    time_t now;
    time(&now);
    return now;
}


String SubmitterWiFi::to_timestamp(unsigned long epoch) {
    struct tm *timeinfo;
    time_t rawtime = epoch;

    timeinfo = localtime(&rawtime);

    char timeStringBuff[50];
    strftime(timeStringBuff, sizeof(timeStringBuff), "%Y-%m-%d %H:%M:%S", timeinfo);
    return timeStringBuff;
}


// ---------------------------- Submitter GSM ------------------------------
SubmitterGSM::SubmitterGSM(int rx, int tx, int HWSerialNum)
        : serialAT(HWSerialNum), modem(serialAT) {
    serialAT.begin(BAUDRATE, SERIAL_8N1, rx, tx, false);
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
    connected = modem.isGprsConnected();
}


// TODO: submit a single reading for GSM
int SubmitterGSM::submit_reading(SoilReading& soilReading) {
    return 0;
}


int SubmitterGSM::submit_reading(SoilDataTable& dataTable) {
    if (!is_connected()) return 0;
    
    String Link;
    Link = "http://" + String(SERVERNAME) + String(SUBMIT_RESOURCE);
    // Link = "http://" + String(SERVERNAME) + "/Sensor/kirimdata.php";

    // try ping first
    TinyGsmClient client = TinyGsmClient(modem);
    if (!client.connect(SERVERNAME, PORT)) {
        Serial.println("Fatal Error! Server is down!");
        return 0;
    }

    DynamicJsonDocument data(dataTable.get_count() * 200);      // dunno 200 is a good number
    data["ID"] = SENSOR_ID;
    JsonArray dataArr = data.createNestedArray("data");
    
    while (!dataTable.is_empty()) {
        SoilReading row;
        dataTable.pop(row);

        JsonObject rowJson = dataArr.createNestedObject();
        rowJson["timestamp"] = to_timestamp(row.epoch);
        rowJson["N"] = row.soilData.nitrogen;
        rowJson["P"] = row.soilData.phosphorus;
        rowJson["K"] = row.soilData.kalium;
        rowJson["pH"] = row.soilData.pH;
        rowJson["temp"] = row.soilData.temperature;
        rowJson["hum"] = row.soilData.humidity;
        rowJson["EC"] = row.soilData.EC;
        rowJson["salt"] = row.soilData.salt;
    }

    String dataStr;
    serializeJson(data, dataStr);
    
#ifdef DEBUG
    serializeJsonPretty(data, Serial);
#endif

    client.print(String("POST ") + SUBMIT_RESOURCE + " HTTP/1.1\r\n");
    client.print(String("Host: ") + SERVERNAME + "\r\n");
    client.println("Connection: close");
    client.println("Content-Type: application/json");
    client.println();
    client.println(dataStr);

    // Wait for data to arrive
    uint32_t startS = millis();
    while (client.connected() && !client.available() && millis() - startS < 30000L) {
        delay(100);
    };

    // Read data
    startS          = millis();
    char responseCodeStr[4] = {
        '\0',
    };
    uint8_t read_charsS = 0;
    while (client.connected() && read_charsS < 4 && millis() - startS < 10000L) {
        while (client.available()) {
            char c = client.read();
            if (isDigit(c)) {
                responseCodeStr[read_charsS]     = c;
                responseCodeStr[read_charsS + 1] = '\0';
                read_charsS++;
                startS = millis();
            }
        }
    }
    Serial.println(responseCodeStr);

    int responseCode = atoi(responseCodeStr);

    client.stop();

    return responseCode;
}


unsigned long SubmitterGSM::get_curr_epoch() {
    int   year     = 0;
    int   month    = 0;
    int   day      = 0;
    int   hour     = 0;
    int   min      = 0;
    int   sec      = 0;
    float timezone = 0;
    struct tm currentTime = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    if (modem.getNetworkTime(
            &currentTime.tm_year, &currentTime.tm_mon, &currentTime.tm_mday,
            &currentTime.tm_hour, &currentTime.tm_min, &currentTime.tm_sec,
            &timezone
    )) {
        return mktime(&currentTime);
    }
    return 0;
}


String SubmitterGSM::to_timestamp(unsigned long epoch) {
    struct tm *timeinfo;
    time_t rawtime = epoch;

    timeinfo = localtime(&rawtime);

    char timeStringBuff[50];
    strftime(timeStringBuff, sizeof(timeStringBuff), "%Y-%m-%d %H:%M:%S", timeinfo);
    return timeStringBuff;
}