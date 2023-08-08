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


// Screen constants parameters
const int SCREEN_WIDTH = 128;   // OLED display width, in pixels
const int SCREEN_HEIGHT = 64;   // OLED display height, in pixels
const int SCREEN_SDA = 5;
const int SCREEN_SCL = 4;
const int SCREEN_ADDR = 0x3C;

void setup_display();

void display_splash_screen();

void display_data(const SoilData& soilData);

void clear_display();

#endif