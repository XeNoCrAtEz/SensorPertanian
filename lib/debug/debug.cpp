#include "debug.h"


const char Logger::filename[] = "/logging.jsonl";


#ifdef DEBUG
Logger::Logger(RTC& rtc)
        : timekeeper(rtc)
{
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


Logger::ErrorCodes Logger::log(const char* level, const char* msg) {
    if (!is_ready()) return LITTLEFS_FAILED;

    File file = filesystem.open(filename, FILE_APPEND);
    if (!file) return OPEN_FAILED;

    StaticJsonDocument<JSON_ENTRY_SIZE> logEntry;
    logEntry["time"] = timekeeper.get_date_time().Epoch32Time();
    logEntry["level"] = level;
    logEntry["msg"] = msg;

    if(serializeJson(logEntry, file)) {
        file.println();
        return SUCCESS;
    }
    else return WRITE_FAILED;
}


Logger::ErrorCodes Logger::log_E(const char* msg) {
    return log("E", msg);
}


Logger::ErrorCodes Logger::log_W(const char* msg) {
    return log("W", msg);
}


Logger::ErrorCodes Logger::log_I(const char* msg) {
    return log("I", msg);
}


Logger::ErrorCodes Logger::log_D(const char* msg) {
    return log("D", msg);
}


Logger::ErrorCodes Logger::log_V(const char* msg) {
    return log("V", msg);
}


#else
Logger::Logger(RTC& rtc) {}
Logger::ErrorCodes Logger::show() { return SUCCESS; }
Logger::ErrorCodes Logger::clear() { return SUCCESS; }
bool Logger::is_ready() { return false; }

Logger::ErrorCodes Logger::log(const char* level, const char* msg) { return SUCCESS; }
Logger::ErrorCodes Logger::log_E(const char* msg) { return SUCCESS; }
Logger::ErrorCodes Logger::log_W(const char* msg) { return SUCCESS; }
Logger::ErrorCodes Logger::log_I(const char* msg) { return SUCCESS; }
Logger::ErrorCodes Logger::log_D(const char* msg) { return SUCCESS; }
Logger::ErrorCodes Logger::log_V(const char* msg) { return SUCCESS; }


#endif