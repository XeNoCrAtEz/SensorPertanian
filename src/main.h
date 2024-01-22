#ifndef __MAIN_H__
#define __MAIN_H__


#include <Arduino.h>

#include "display.h"
#include "probe.h"
#include "dataTable.h"
#include "submitter.h"
#include "sleep.h"
#include "voltageMonitor.h"

#include "pin_assignment.h"


const float VOLT_MON_DIVIDER_RATIO = 2.0;

const float MEASUREMENT_UNCERTAINTY = 1;
const float ESTIMATION_UNCERTAINTY = 1;
const float PROCESS_NOISE = 0.01;


// pre-define DEBUG when compiling to enable debug
#ifdef DEBUG

#include "debug.h"

#endif


#endif