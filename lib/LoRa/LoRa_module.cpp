// #include "debug.h"


// const char Logger::m_filename[] = "/logging.jsonl";


// #ifdef DEBUG
// void Logger::print(const String& time, const String& level, const String& msg) {
//     Serial.print(time);
//     Serial.print(" [");
//     Serial.print(level);
//     Serial.print("] - ");
//     Serial.println(msg);
// }


// Logger::Logger(TimeClass& time, bool printMode)
//         : m_timekeeper(time) {
//     if (printMode) {
//         m_status = PRINT_MODE;
//         return;
//     }

//     if (!m_filesystem.begin()) {
//         Serial.println("LittleFS Mount Failed!");
//         m_status = LITTLEFS_FAILED;
//         return;
//     }

//     File file = m_filesystem.open(m_filename, FILE_READ, true);
//     if (!file) Serial.println("File not found! \"logging.jsonl\" created!");

//     m_status = READY;
// }


// Logger::OpStatus Logger::show() {
//     if (status() == PRINT_MODE) return STATUS_PRINT_MODE;
//     if (status() != READY) return STATUS_ERROR;

//     File file = m_filesystem.open(m_filename, FILE_READ);
//     if (!file) return OPEN_FAILED;

//     if (!file.size()) return EMPTY_FILE;

//     while (true) {
//         StaticJsonDocument<JSON_ENTRY_SIZE> logEntry;

//         DeserializationError err = deserializeJson(logEntry, file);
//         if (err) break;

//         print(logEntry["time"].as<String>(), logEntry["level"].as<String>(), logEntry["msg"].as<String>());
//     }
    
//     return SUCCESS;
// }


// Logger::OpStatus Logger::clear() {
//     if (status() == PRINT_MODE) return STATUS_PRINT_MODE;
//     if (status() != READY) return STATUS_ERROR;

//     File file = m_filesystem.open(m_filename, FILE_WRITE);
//     if (!file) return OPEN_FAILED;

//     return SUCCESS;
// }


// Logger::Status Logger::status() {
//     return m_status;
// }


// bool Logger::is_print_mode() {
//     return (status() == PRINT_MODE);
// }


// Logger::OpStatus Logger::log(const String& level, const String& msg) {
//     RtcDateTime now;
//     m_timekeeper.get_date_time(now);

//     if (is_print_mode()) {    
//         print(RtcDateTime_to_Str(now), level, msg);
//         return STATUS_PRINT_MODE;
//     }

//     if (status() != READY) return STATUS_ERROR;

//     File file = m_filesystem.open(m_filename, FILE_APPEND);
//     if (!file) return OPEN_FAILED;

//     StaticJsonDocument<JSON_ENTRY_SIZE> logEntry;
//     logEntry["time"] = RtcDateTime_to_Str(now);
//     logEntry["level"] = level;
//     logEntry["msg"] = msg;

//     if(serializeJson(logEntry, file)) {
//         file.println();
//         return SUCCESS;
//     }
//     else return WRITE_FAILED;
// }


// Logger::OpStatus Logger::log_E(const String& msg) {
//     return log("E", msg);
// }


// Logger::OpStatus Logger::log_W(const String& msg) {
//     return log("W", msg);
// }


// Logger::OpStatus Logger::log_I(const String& msg) {
//     return log("I", msg);
// }


// Logger::OpStatus Logger::log_D(const String& msg) {
//     return log("D", msg);
// }


// Logger::OpStatus Logger::log_V(const String& msg) {
//     return log("V", msg);
// }


// #else
// Logger::Logger(TimeClass& time, bool printMode) : m_timekeeper(time), m_status(NO_DEBUG) {}
// Logger::OpStatus Logger::show() { return DEBUG_INACTIVE; }
// Logger::OpStatus Logger::clear() { return DEBUG_INACTIVE; }
// Logger::Status Logger::status() { return NO_DEBUG; }
// bool Logger::is_print_mode() { return false; }

// Logger::OpStatus Logger::log(const String& level, const String& msg) { return DEBUG_INACTIVE; }
// Logger::OpStatus Logger::log_E(const String& msg) { return DEBUG_INACTIVE; }
// Logger::OpStatus Logger::log_W(const String& msg) { return DEBUG_INACTIVE; }
// Logger::OpStatus Logger::log_I(const String& msg) { return DEBUG_INACTIVE; }
// Logger::OpStatus Logger::log_D(const String& msg) { return DEBUG_INACTIVE; }
// Logger::OpStatus Logger::log_V(const String& msg) { return DEBUG_INACTIVE; }

// void Logger::print(const String& time, const String& level, const String& msg) {}


// #endif