#include <Arduino.h>
#include "display.h"
#include "probe.h"
#include "dataTable.h"
#include "submitter.h"

// pre-define DEBUG when compiling to enable debug
#ifdef DEBUG

#include "debug.h"

#endif

const int PROBE_RX_PIN = 0;
const int PROBE_TX_PIN = 16;

const int SENSOR_ID = 1;

void sleep();