#ifndef __SLEEP_H__
#define __SLEEP_H__

#include <Arduino.h>
#include "display.h"

#include "timeClass.h"
#include "debug.h"


void sleep(TimeClass& time, Logger& logger);
uint64_t get_sleep_seconds(TimeClass& time);


#endif