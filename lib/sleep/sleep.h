#ifndef __SLEEP_H__
#define __SLEEP_H__

#include <Arduino.h>
#include "display.h"

#include "timeClass.h"


void sleep(TimeClass& time);
uint64_t get_sleep_seconds(TimeClass& time);


#endif