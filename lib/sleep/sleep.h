#ifndef __SLEEP_H__
#define __SLEEP_H__

#include <Arduino.h>
#include "display.h"

#include "time.h"
#include "RTCModule.h"


void sleep(RTC& rtc, Display& display);
uint64_t get_sleep_seconds(RTC& rtc);


#endif