#ifndef __SLEEP_H__
#define __SLEEP_H__

#include <Arduino.h>
#include "submitter.h"

#include "time.h"
#include <Preferences.h>

extern Preferences preferences;

extern time_t currentEpoch;


extern const char epochNamespace[];
extern const char epochKeyname[];


void sleep(Submitter &submitter);
uint64_t get_sleep_seconds(Submitter &submitter);


#endif