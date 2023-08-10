#include "submitter.h"


Submitter::Submitter(const int sensor_id) 
        : sensorID(sensor_id)
{
    WiFi.mode(WIFI_STA);

    // 7*3600 set timezone to Jakarta
    configTime(7*3600, 0, NTP_SERVER);
    

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
    return;
}


int Submitter::submit_table(SoilDataTable& dataTable) {
    Serial.println("Sending data to server...");

    if (!is_connected()) return 0;
    
    String Link;
    // LINK: http://raspberrypi.local/Sensor/kirimdata.php
    Link = "http://" + String(SERVERNAME) + "/Sensor/kirimdata.php";

    // try ping first
    HTTPClient http;
    http.begin(Link);
    int responseCode = http.GET();
    if ( responseCode != HTTP_CODE_OK ) {
        Serial.println("Server not available! Abort sending data.");
        Serial.println(http.errorToString(responseCode));
        http.end();
        return responseCode;
    }

    http.addHeader("Content-Type", "application/json");

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
    }

    String dataStr;
    serializeJson(data, dataStr);
    serializeJsonPretty(data, Serial);

    responseCode = http.POST(dataStr);
    if ( responseCode != HTTP_CODE_OK ) {
        Serial.println("Error when sending data to server");
        Serial.println(http.errorToString(responseCode));
    }

    http.end();

    return responseCode;
}


unsigned long Submitter::get_curr_epoch() {
    time_t now;
    time(&now);
    return now;
}


String Submitter::to_timestamp(unsigned long epoch) {
    struct tm *timeinfo;
    time_t rawtime = epoch;

    timeinfo = localtime(&rawtime);

    char timeStringBuff[50];
    strftime(timeStringBuff, sizeof(timeStringBuff), "%Y-%m-%d %H:%M:%S", timeinfo);
    return timeStringBuff;
}


bool Submitter::is_connected() {
    return connected;
}