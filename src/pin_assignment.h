#ifndef __PIN_ASSIGNMENT_H__
#define __PIN_ASSIGNMENT_H__


const uint8_t PIN_SCREEN_SDA = 5;
const uint8_t PIN_SCREEN_SCL = 4;

const uint8_t PIN_PROBE_RX = 16;
const uint8_t PIN_PROBE_TX = 17;

const uint8_t PIN_RTC_RST  = 12;
const uint8_t PIN_RTC_DATA = 13;
const uint8_t PIN_RTC_CLK  = 15;

const uint8_t PIN_VOLT_SC = 35;
const uint8_t PIN_VOLT_BAT = 34;

#ifdef USE_GSM
const uint8_t PIN_GSM_RX = 1;
const uint8_t PIN_GSM_TX = 1;
#endif


#endif