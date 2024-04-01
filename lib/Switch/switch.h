#ifndef __SWITCH_H__
#define __SWITCH_H__


#include <Arduino.h>


class Switch {
public:
    // class status codes
    enum Status {
        READY,
        RELEASED,
        UNKNOWN_ERROR,
    };
    
    // Operation status codes
    enum OpStatus {
        ON,
        OFF,
        STATUS_ERROR,
    };


private:
    Status m_status = UNKNOWN_ERROR;
    uint8_t m_level = LOW;
    uint8_t m_pin = -1;


public:
    Switch(uint8_t pin);
    OpStatus on();
    OpStatus off();
    Status release();
    Status status();


};


#endif