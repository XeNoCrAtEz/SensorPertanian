#include "submitter.h"


Submitter::Submitter(const int sensorID) 
        : SENSOR_ID(sensorID)
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
    return;
}


int Submitter::submit_table(SoilDataTable& dataTable) {
    if (!is_connected()) return 0;

    Serial.println("Sending data to server...");
    
    String Link;    // untuk menyimpan link HTTP
    // LINK: http://raspberrypi.local/Sensor/kirimdata.php
    Link = "http://" + String(SERVERNAME) + "/kirimdata.php";

    HTTPClient http;            // untuk melakukan HTTP
    http.begin(Link);
    http.addHeader("Content-Type", "application/json");

    JSONVar data;
    data["ID"] = SENSOR_ID;
    data["data"] = JSONVar();
    int i = 0;
    while (!dataTable.is_empty()) {
        SoilReading row;
        dataTable.pop(row);
        data["data"][i++] = JSON.parse(row.to_json_string());
    }

    data.printTo(Serial);

    int responseCode = http.POST(JSON.stringify(data));

    if ( responseCode != HTTP_CODE_OK ) {
        Serial.println("Error when sending data to server");
        Serial.println(http.errorToString(responseCode));
    }

    http.end();

    return responseCode;
}

unsigned long Submitter::get_curr_epoch()
{
    return 0;
}

void Submitter::test() {
    SoilData dummyData = {
        111, 123, 255,
        7.53, 17.1, 38.9,
        2345
    };

    //tampilkan di serial monitor
    Serial.println("Suhu: " + String(dummyData.temperature) );
    Serial.println("Kelembaban: " + String(dummyData.humidity) );
    Serial.println("NPK: " + String(dummyData.nitrogen) + ", " + String(dummyData.phosphorus) + ", " + String(dummyData.kalium));
    Serial.println("pH: " + String(dummyData.pH));
    Serial.println("EC: " + String(dummyData.EC));
    Serial.println();

    //Kirim Data ke Database
    //Cek KOneksi nodemcu ke web server
    WiFiClient client;
    const int httpPort = 80;
    if(!client.connect(SERVERNAME, httpPort)){
        Serial.println("Gagal Terkoneksi ke Web Server");
        return;
    }

    //Apabila terkoneksi ke web server, maka kirim Data
    HTTPClient http;

    //Siapkan variabel Link URL untuk kirim Data
    // LINK: http://raspberrypi.local/Sensor/kirimdata.php?Suhu=18.5&Kelembaban=41.5&pH=5.3&Nitrogen=112&Kalium=245&Phosphorus=123&Konduktivitas_Listrik=2133
    String Link = "http://" + String(SERVERNAME) + "/Sensor/kirimdata.php?" +
                    "Suhu=" + String(dummyData.temperature) + "&" +
                    "Kelembaban=" + String(dummyData.humidity) + "&" + 
                    "pH=" + String(dummyData.pH) + "&" +
                    "Nitrogen=" + String(dummyData.nitrogen) + "&" +
                    "Kalium=" + String(dummyData.kalium) + "&" +
                    "Phosphorus=" + String(dummyData.phosphorus) + "&" +
                    "Konduktivitas_Listrik=" + String(dummyData.EC);
    Serial.print(Link);

    //Eksekusi Link URL
    http.begin(Link);
    http.GET();

    //Tangkap Respon kirimdata
    String respon = http.getString();
    Serial.println(respon);

    delay(1000);
}


bool Submitter::is_connected() {
    return connected;
}