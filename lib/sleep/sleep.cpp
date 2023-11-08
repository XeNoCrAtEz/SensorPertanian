#include "sleep.h"


Preferences preferences;
RTC_DATA_ATTR time_t currentEpoch = 0;

const int SCHEDULE_LENGTH = 6;
struct tm schedule[SCHEDULE_LENGTH] = {
    {0, 0, 4, 0, 0, 0, 0, 0, 0},        // 04.00
    {0, 0, 8, 0, 0, 0, 0, 0, 0},        // 08.00
    {0, 0, 12, 0, 0, 0, 0, 0, 0},       // 12.00
    {0, 0, 16, 0, 0, 0, 0, 0, 0},       // 16.00
    {0, 0, 20, 0, 0, 0, 0, 0, 0},       // 20.00
    {0, 0, 0, 1, 0, 0, 0, 0, 0},        // 24.00 or 1.00.00
};

const int uS_TO_S_FACTOR = 1000000;
const char epochNamespace[] = "epoch";
const char epochKeyname[] = "epoch";

void sleep(Display& display, Submitter &submitter) {
    display.clear_display();

    auto sleepTime = get_sleep_seconds(submitter);
    currentEpoch += sleepTime;      // predicted epoch for next sleep cycle

    preferences.begin(epochNamespace, false);
    preferences.putLong(epochKeyname, currentEpoch);
    preferences.end();

#ifdef DEBUG
    Serial.print("Time to sleep: ");
    Serial.println(sleepTime);
#endif

    esp_deep_sleep(sleepTime * uS_TO_S_FACTOR);
}


uint64_t get_sleep_seconds(Submitter &submitter) {
    struct tm *current_tm = localtime(&currentEpoch);   // TODO: get current time code
    struct tm current_H_M = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    current_H_M.tm_hour = current_tm->tm_hour;
    current_H_M.tm_min = current_tm->tm_min;
    current_H_M.tm_sec = current_tm->tm_sec;

    double min_sec = INFINITY;
    for (int i = 0; i < SCHEDULE_LENGTH; i++) {
        double diff = difftime(mktime(&schedule[i]), mktime(&current_H_M));
        if (diff < 0) continue;                         // skip jadwal yang sudah lewat
        else if (diff < min_sec) min_sec = diff;
    }
    // jika tidak ditemukan jadwal selanjutnya, paksa tidur 4 jam
    if (min_sec == 0 || min_sec == INFINITY)
        min_sec = 4 * 3600;

    return min_sec;
}