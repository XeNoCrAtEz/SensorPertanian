#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "soil_data.h"


void info_soil_data(const SoilData& soilData);


class Debugger {
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

    bool ready = false;


public:
    Debugger();
    ErrorCodes show();
    ErrorCodes clear();
    bool isReady();
    
    ErrorCodes log(unsigned long time, const char* level, const char* msg);
    ErrorCodes log_E(unsigned long time, const char* msg);
    ErrorCodes log_W(unsigned long time, const char* msg);
    ErrorCodes log_I(unsigned long time, const char* msg);
    ErrorCodes log_D(unsigned long time, const char* msg);
    ErrorCodes log_V(unsigned long time, const char* msg);


};


#endif