#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "soil_data.h"
#include "timeClass.h"


class Logger {
public:
    // class status codes
    enum Status {
        READY,
        LITTLEFS_FAILED,
        PRINT_MODE,
        NO_DEBUG,
        UNKNOWN_ERROR,
    };

    // operation status codes
    enum OpStatus {
        SUCCESS,
        OPEN_FAILED,
        WRITE_FAILED,
        READ_FAILED,
        EMPTY_FILE,
        DEBUG_INACTIVE,
        STATUS_PRINT_MODE,
        STATUS_ERROR,
    };


private:
    enum LoggerParams {
        MAX_COUNT = 10000,
        JSON_ENTRY_SIZE = 512,
    };


// CLASS ATTRIBUTES AND METHODS
private:
    static const char m_filename[];
    fs::LittleFSFS& m_filesystem = LittleFS;
    TimeClass& m_timekeeper;
    
    Status m_status = UNKNOWN_ERROR;
    // bool m_printMode = false;

    void print(const String& time, const String& level, const String& msg);


public:
    Logger(TimeClass& timeClass, bool printMode=false);
    OpStatus show();
    OpStatus clear();
    Status status();
    bool is_print_mode();
    
    OpStatus log(const String& level, const String& msg);
    OpStatus log_E(const String& msg);
    OpStatus log_W(const String& msg);
    OpStatus log_I(const String& msg);
    OpStatus log_D(const String& msg);
    OpStatus log_V(const String& msg);


};


extern Logger logger;


#endif