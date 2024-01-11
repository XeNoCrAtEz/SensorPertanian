#include "debug.h"


const char Logger::filename[] = "/logging.jsonl";


#ifdef DEBUG
void Logger::print(const String& time, const String& level, const String& msg) {
    Serial.print(time);
    Serial.print(" [");
    Serial.print(level);
    Serial.print("] - ");
    Serial.println(msg);
}


Logger::Logger(RTC& rtc, bool printMode)
        : timekeeper(rtc), printMode(printMode)
{
    if (printMode) {
        ready = true;
        return;
    }

    if (!filesystem.begin()) {
        Serial.println("LittleFS Mount Failed!");
        return;
    }

    File file = filesystem.open(filename, FILE_READ, true);
    if (!file) Serial.println("File not found! \"logging.jsonl\" created!");

    ready = true;
}


Logger::ErrorCodes Logger::show() {
    if (printMode) return PRINT_MODE;

    if (!ready) return LITTLEFS_FAILED;
    
    File file = filesystem.open(filename, FILE_READ);
    if (!file) return OPEN_FAILED;

    if (!file.size()) return EMPTY_FILE;

    while (true) {
        StaticJsonDocument<JSON_ENTRY_SIZE> logEntry;

        DeserializationError err = deserializeJson(logEntry, file);
        if (err) break;

        print(logEntry["time"].as<String>(), logEntry["level"].as<String>(), logEntry["msg"].as<String>());
    }
    
    return SUCCESS;
}


Logger::ErrorCodes Logger::clear() {
    if (printMode) return PRINT_MODE;

    if (!ready) return LITTLEFS_FAILED;

    File file = filesystem.open(filename, FILE_WRITE);
    if (!file) return OPEN_FAILED;

    return SUCCESS;
}


bool Logger::is_ready() {
    return ready;
}


bool Logger::is_print_mode() {
    return printMode;
}


Logger::ErrorCodes Logger::log(const String& level, const String& msg) {
    if (printMode) {
        print(RtcDateTime_to_Str(timekeeper.get_date_time()), level, msg);
        return PRINT_MODE;
    }

    if (!ready) return LITTLEFS_FAILED;

    File file = filesystem.open(filename, FILE_APPEND);
    if (!file) return OPEN_FAILED;

    StaticJsonDocument<JSON_ENTRY_SIZE> logEntry;
    logEntry["time"] = RtcDateTime_to_Str(timekeeper.get_date_time());
    logEntry["level"] = level;
    logEntry["msg"] = msg;

    if(serializeJson(logEntry, file)) {
        file.println();
        return SUCCESS;
    }
    else return WRITE_FAILED;
}


Logger::ErrorCodes Logger::log_E(const String& msg) {
    return log("E", msg);
}


Logger::ErrorCodes Logger::log_W(const String& msg) {
    return log("W", msg);
}


Logger::ErrorCodes Logger::log_I(const String& msg) {
    return log("I", msg);
}


Logger::ErrorCodes Logger::log_D(const String& msg) {
    return log("D", msg);
}


Logger::ErrorCodes Logger::log_V(const String& msg) {
    return log("V", msg);
}


#else
Logger::Logger(RTC& rtc, bool printMode) : timekeeper(rtc) {}
Logger::ErrorCodes Logger::show() { return DEBUG_INACTIVE; }
Logger::ErrorCodes Logger::clear() { return DEBUG_INACTIVE; }
bool Logger::is_ready() { return ready; }
bool Logger::is_print_mode() { return printMode; }

Logger::ErrorCodes Logger::log(const String& level, const String& msg) { return DEBUG_INACTIVE; }
Logger::ErrorCodes Logger::log_E(const String& msg) { return DEBUG_INACTIVE; }
Logger::ErrorCodes Logger::log_W(const String& msg) { return DEBUG_INACTIVE; }
Logger::ErrorCodes Logger::log_I(const String& msg) { return DEBUG_INACTIVE; }
Logger::ErrorCodes Logger::log_D(const String& msg) { return DEBUG_INACTIVE; }
Logger::ErrorCodes Logger::log_V(const String& msg) { return DEBUG_INACTIVE; }

void Logger::print(const String& time, const String& level, const String& msg) {}


#endif