#include "dataTable.h"

String Link;                // untuk menyimpan link HTTP
HTTPClient http;            // untuk melakukan HTTP

SoilDataTable::SoilDataTable()
        : idx_in{-1}, idx_out{-1}, count{0} {
}


bool SoilDataTable::push(const SoilReading& soilData) {
    inc_idx(idx_in);
    if (is_empty()) {
        inc_idx(idx_out);
        count++;
    } else if (!is_empty() && !is_full()) {
        count++;
    } else if (is_full()) {
        inc_idx(idx_out);
    }
    readings[idx_in] = soilData;
    return true;
}


bool SoilDataTable::pop(SoilReading& soilData) {
    if (is_empty()) return false;
    else {
        soilData = readings[idx_out];
        if (count == 1) {
            count = 0;
            idx_in = -1;
            idx_out = -1;
        } else {
            count--;
            inc_idx(idx_out);
        }
    }
    return true;
}


inline bool SoilDataTable::is_empty() {
    return (!count) ? true : false;
}


inline bool SoilDataTable::is_full() {
    return (count == MAX_SIZE) ? true : false;
}


inline int SoilDataTable::get_count() {
    return count;
}


inline void SoilDataTable::clear() {
    idx_in = -1;
    idx_out = -1;
    count = 0;
}


inline void SoilDataTable::inc_idx(int& idx) {
    idx = (idx+1) % MAX_SIZE;
}


inline void SoilDataTable::dec_idx(int& idx) {
    idx = (idx-1) % MAX_SIZE;
}


bool SoilDataTable::connect_wifi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);   // connect ke router
    Serial.print("Connecting to ");
    Serial.print(WIFI_SSID);
    while ( WiFi.status() != WL_CONNECTED ) {
        delay (1000);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("\nConnection Successful!\n");
#ifdef DEBUG
    Serial.print("Sensor IP Address : ");
    Serial.println(WiFi.localIP());
	Serial.println();
#endif
    delay(1000);
    
    return true;
}


int SoilDataTable::submit_table() {
    Serial.println("Sending data to server...");
    // TODO: setup link
    Link = "http://" + IP_SERVER.toString() + "/data.php";

    JSONVar data;
    SoilReading row;
    pop(row);
    data[0] = row.to_json_string();

    http.begin(Link);
    http.addHeader("Content-Type", "application/json");
    int responseCode = http.POST(data);

    if ( responseCode != HTTP_CODE_OK ) {
        Serial.println(http.getString());
        Serial.println("Error when sending data to server");
        Serial.println(http.errorToString(responseCode));
        return responseCode;
    }

    http.end();

    return 0;
}


void SoilDataTable::test() {
    //Baca nilai suhu
    float Suhu = 25.2;
    //baca nilai kelembaban
    float Kelembaban = 41.2;

    //tampilkan di serial monitor
    Serial.println("Suhu:" + String(Suhu) );
    Serial.println("Kelembaban:" + String(Kelembaban) );
    Serial.println();

    //Kirim Data ke Database
    //Cek KOneksi nodemcu ke web server
    WiFiClient client;
    const int httpPort = 80;
    if(!client.connect(IP_SERVER, httpPort)){
        Serial.println("Gagal Terkoneksi ke Web Server");
        return;
    }

    //Apabila terkoneksi ke web server, maka kirim Data
    HTTPClient http;

    //Siapkan variabel Link URL untuk kirim Data
    String Link = "http://" + IP_SERVER.toString() + "/Sensor/kirimdata.php?Suhu=" + String(Suhu) + "&Kelembaban=" + String(Kelembaban);
    Serial.print(Link);

    //Eksekusi Link URL
    http.begin(Link);
    http.GET();

    //Tangkap Respon kirimdata
    String respon = http.getString();
    Serial.println(respon);

    delay(1000);
}