#include "sleep.h"


const int SCHEDULE_LENGTH = 6;
RtcDateTime schedule[SCHEDULE_LENGTH] = {
    RtcDateTime(0, 0, 0, 4, 0, 0),
    RtcDateTime(0, 0, 0, 8, 0, 0),
    RtcDateTime(0, 0, 0, 12, 0, 0),
    RtcDateTime(0, 0, 0, 16, 0, 0),
    RtcDateTime(0, 0, 0, 20, 0, 0),
    RtcDateTime(0, 0, 1, 0, 0, 0),
};

const int uS_TO_S_FACTOR = 1000000;


void sleep(RTC& rtc, Display& display) {
    display.clear_display();

    auto sleepTime = get_sleep_seconds(rtc);

    // logger.log_I("Time to sleep: --- please implement!");

    // logger.log_I("Sleeping.");
    Serial.print("Time to sleep: ");
    Serial.println(sleepTime);

    Serial.println("Sleeping.");

    esp_deep_sleep(sleepTime * uS_TO_S_FACTOR);
}


uint64_t get_sleep_seconds(RTC& rtc) {
    RtcDateTime current_time = rtc.get_date_time();
    unsigned long current_H_M_secs = RtcDateTime(0, 0, 0, current_time.Hour(), current_time.Minute(), current_time.Second()).TotalSeconds();

    double min_sec = INFINITY;
    for (int i = 0; i < SCHEDULE_LENGTH; i++) {
        double diff = schedule[i].TotalSeconds() - current_H_M_secs;
        if (diff < 0) continue;                         // skip jadwal yang sudah lewat
        else if (diff < min_sec) min_sec = diff;
    }
    // jika tidak ditemukan jadwal selanjutnya, paksa tidur 4 jam
    if (min_sec == 0 || min_sec == INFINITY)
        min_sec = 4 * 3600;

    return min_sec;
}