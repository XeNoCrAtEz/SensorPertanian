#ifndef _DISPLAY_H_
#define _DISPLAY_H_


#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "soil_data.h"

// pre-define DEBUG when compiling to enable debug
#ifdef DEBUG

#include "debug.h"

#endif


class Display {
public:
    enum ErrorCodes {
        DISPLAY_FAILED,
        DISPLAY_OK,
    };


private:
    const uint8_t SCREEN_WIDTH;         // OLED display width, in pixels
    const uint8_t SCREEN_HEIGHT;        // OLED display height, in pixels
    const uint8_t SCREEN_ADDR;

    Adafruit_SSD1306 m_disp;
    ErrorCodes m_status;


public:    
    Display(
            uint8_t sda, uint8_t scl,
            uint8_t w=128, uint8_t h=64, uint8_t addr=0x3C,
            TwoWire *twi=&Wire, int8_t rstpin=-1
        );
    ErrorCodes isOK();
    ErrorCodes display_splash_screen();
    ErrorCodes display_data(const SoilData& soilData);
    ErrorCodes clear_display();
};


#endif