#ifndef __MAIN_H__
#define __MAIN_H__


#include <Arduino.h>

#include "display.h"
#include "probe.h"
#include "dataTable.h"
#include "submitter.h"
#include "sleep.h"
#include "voltageMonitor.h"
#include "switch.h"

#include "pin_assignment.h"

#ifdef TAHAP_1
const float VOLT_MON_DIVIDER_RATIO = 2.0;
#else
const float VOLT_MON_DIVIDER_RATIO = 3.2;
#endif

const float MEASUREMENT_UNCERTAINTY = 1;
const float ESTIMATION_UNCERTAINTY = 1;
const float PROCESS_NOISE = 0.01;

const uint8_t GSM_HW_SERIAL_NUM = 1;
const uint8_t OTHER_HW_SERIAL_NUM = 2;


// pre-define DEBUG when compiling to enable debug
#ifdef DEBUG

#include "debug.h"

#endif


#endif