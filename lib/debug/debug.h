#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "soil_data.h"
#include "RTCModule.h"


class Logger {
// CLASS PARAMETERS
public:
    enum ErrorCodes {
        SUCCESS,
        OPEN_FAILED,
        WRITE_FAILED,
        READ_FAILED,
        EMPTY_FILE,
        LITTLEFS_FAILED,
        UNKNOWN_ERROR,
    };


private:
    enum SoilDataTableParams {
        MAX_COUNT = 10000,
        JSON_ENTRY_SIZE = 256,
    };


// CLASS ATTRIBUTES AND METHODS
private:
    static const char filename[];
    fs::LittleFSFS filesystem = LittleFS;
    RTC& timekeeper;
    
    bool ready = false;


public:
    Logger(RTC& rtc);
    ErrorCodes show();
    ErrorCodes clear();
    bool is_ready();
    
    ErrorCodes log(const char* level, const char* msg);
    ErrorCodes log_E(const char* msg);
    ErrorCodes log_W(const char* msg);
    ErrorCodes log_I(const char* msg);
    ErrorCodes log_D(const char* msg);
    ErrorCodes log_V(const char* msg);


};


extern Logger logger;


#endif