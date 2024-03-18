#ifndef __PIN_ASSIGNMENT_H__
#define __PIN_ASSIGNMENT_H__


#ifdef USE_DISPLAY
const uint8_t PIN_SCREEN_SDA = 5;
const uint8_t PIN_SCREEN_SCL = 4;
#endif

const uint8_t PIN_PROBE_RX = 25;
const uint8_t PIN_PROBE_TX = 33;

const uint8_t PIN_RTC_RST  = 14;
const uint8_t PIN_RTC_DATA = 27;
const uint8_t PIN_RTC_CLK  = 26;

const uint8_t PIN_GPS_RX = 13;
const uint8_t PIN_GPS_TX = -1;      // No GPS TX pin

const uint8_t PIN_MODULES_SW = 15;

const uint8_t PIN_VOLT_SC = 35;
const uint8_t PIN_VOLT_BAT = 32;

const uint8_t PIN_LORA_NSS = 5;
const uint8_t PIN_LORA_DIO0 = 17;
const uint8_t PIN_LORA_RST = 16;

#ifdef USE_GSM
const uint8_t PIN_GSM_RX = 2;
const uint8_t PIN_GSM_TX = 4;
#endif


#endif