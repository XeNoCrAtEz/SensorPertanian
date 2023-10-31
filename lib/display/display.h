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
    
    Display(uint8_t sda, uint8_t scl, uint8_t w=SCREEN_WIDTH, uint8_t h=SCREEN_HEIGHT, TwoWire *twi=&Wire, int8_t rstpin=-1);
    ErrorCodes isOK();
    ErrorCodes display_splash_screen();
    ErrorCodes display_data(const SoilData& soilData);
    ErrorCodes clear_display();


private:
    enum ScreenParam {
        SCREEN_WIDTH = 128,     // OLED display width, in pixels
        SCREEN_HEIGHT = 64,     // OLED display height, in pixels
        SCREEN_ADDR = 0x3C,
    };

    Adafruit_SSD1306 m_disp;
    ErrorCodes m_status;
};


#endif