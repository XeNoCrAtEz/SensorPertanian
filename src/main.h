#include <Arduino.h>
#include "display.h"
#include "probe.h"
#include "dataTable.h"
#include "submitter.h"

#include "time.h"
#include <Preferences.h>

// pre-define DEBUG when compiling to enable debug
#ifdef DEBUG

#include "debug.h"

#endif

const int uS_TO_S_FACTOR = 1000000;

const int PROBE_RX_PIN = 16;
const int PROBE_TX_PIN = 17;

// const int SENSOR_ID = 1;

const int SCHEDULE_LENGTH = 6;
struct tm schedule[SCHEDULE_LENGTH] = {
    {0, 0, 4, 0, 0, 0, 0, 0, 0},        // 04.00
    {0, 0, 8, 0, 0, 0, 0, 0, 0},        // 08.00
    {0, 0, 12, 0, 0, 0, 0, 0, 0},       // 12.00
    {0, 0, 16, 0, 0, 0, 0, 0, 0},       // 16.00
    {0, 0, 20, 0, 0, 0, 0, 0, 0},       // 20.00
    {0, 0, 0, 1, 0, 0, 0, 0, 0},        // 24.00 or 1.00.00
};

const char epochNamespace[] = "epoch";
const char epochKeyname[] = "epoch";

void sleep(Submitter &submitter);
uint64_t get_sleep_seconds(Submitter &submitter);