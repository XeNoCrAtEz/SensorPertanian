#include "debug.h"


const char Logger::filename[] = "/logging.jsonl";


#ifdef DEBUG
Logger::Logger() {
    if (!filesystem.begin()) {
        Serial.println("LittleFS Mount Failed!");
        return;
    }

    File file = filesystem.open(filename, FILE_READ, true);
    if (!file) Serial.println("File not found! \"logging.jsonl\" created!");

    ready = true;
}


Logger::ErrorCodes Logger::show() {
    if (!is_ready()) return LITTLEFS_FAILED;
    
    File file = filesystem.open(filename, FILE_READ);
    if (!file) return OPEN_FAILED;

    if (!file.size()) return EMPTY_FILE;

    while (true) {
        StaticJsonDocument<JSON_ENTRY_SIZE> logEntry;

        DeserializationError err = deserializeJson(logEntry, file);
        if (err) break;

        Serial.print(logEntry["time"].as<int>());
        Serial.print(" [");
        Serial.print(logEntry["level"].as<const char*>());
        Serial.print("] - ");
        Serial.println(logEntry["msg"].as<const char*>());
    }

    return SUCCESS;
}


Logger::ErrorCodes Logger::clear() {
    if (!is_ready()) return LITTLEFS_FAILED;

    File file = filesystem.open(filename, FILE_WRITE);
    if (!file) return OPEN_FAILED;

    return SUCCESS;
}


bool Logger::is_ready() {
    return ready;
}


Logger::ErrorCodes Logger::log(unsigned long time, const char* level, const char* msg) {
    if (!is_ready()) return LITTLEFS_FAILED;

    File file = filesystem.open(filename, FILE_APPEND);
    if (!file) return OPEN_FAILED;

    StaticJsonDocument<JSON_ENTRY_SIZE> logEntry;
    logEntry["time"] = time;
    logEntry["level"] = level;
    logEntry["msg"] = msg;

    if(serializeJson(logEntry, file)) {
        file.println();
        return SUCCESS;
    }
    else return WRITE_FAILED;
}


Logger::ErrorCodes Logger::log_E(unsigned long time, const char* msg) {
    return log(time, "E", msg);
}


Logger::ErrorCodes Logger::log_W(unsigned long time, const char* msg) {
    return log(time, "W", msg);
}


Logger::ErrorCodes Logger::log_I(unsigned long time, const char* msg) {
    return log(time, "I", msg);
}


Logger::ErrorCodes Logger::log_D(unsigned long time, const char* msg) {
    return log(time, "D", msg);
}


Logger::ErrorCodes Logger::log_V(unsigned long time, const char* msg) {
    return log(time, "V", msg);
}


#else
Logger::Logger() {}
Logger::ErrorCodes Logger::show() { return SUCCESS; }
Logger::ErrorCodes Logger::clear() { return SUCCESS; }
bool Logger::is_ready() { return false; }

Logger::ErrorCodes Logger::log(unsigned long time, const char* level, const char* msg) { return SUCCESS; }
Logger::ErrorCodes Logger::log_E(unsigned long time, const char* msg) { return SUCCESS; }
Logger::ErrorCodes Logger::log_W(unsigned long time, const char* msg) { return SUCCESS; }
Logger::ErrorCodes Logger::log_I(unsigned long time, const char* msg) { return SUCCESS; }
Logger::ErrorCodes Logger::log_D(unsigned long time, const char* msg) { return SUCCESS; }
Logger::ErrorCodes Logger::log_V(unsigned long time, const char* msg) { return SUCCESS; }


#endif


void info_soil_data(const SoilData& soilData) {
    const uint16_t& nitrogen = soilData.nitrogen;
    const uint16_t& phosphorus = soilData.phosphorus;
    const uint16_t& kalium = soilData.kalium;
    const float& pH = soilData.pH;
    const float& temperature = soilData.temperature;
    const float& humidity = soilData.humidity;
    const uint16_t& EC = soilData.EC;
    const float& salt = soilData.salt; 

    // output to USB Serial
    Serial.print("Nitrogen: ");
    Serial.print(nitrogen);
    Serial.println(" mg/kg");

    Serial.print("Phosphorous: ");
    Serial.print(phosphorus);
    Serial.println(" mg/kg");

    Serial.print("Potassium: ");
    Serial.print(kalium);
    Serial.println(" mg/kg");

    Serial.print("pH: ");
    Serial.println(pH);

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");
    
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    Serial.print("EC: ");
    Serial.print(EC);
    Serial.println(" uS/cm");

    Serial.println("Salt");
    Serial.print(salt);
    Serial.println("g/kg");

    Serial.println();
}

