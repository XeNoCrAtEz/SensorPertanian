#ifndef __SLEEP_H__
#define __SLEEP_H__

#include <Arduino.h>
#include "display.h"

#include "timeClass.h"


void sleep(RTC& rtc);
uint64_t get_sleep_seconds(RTC& rtc);


#endif