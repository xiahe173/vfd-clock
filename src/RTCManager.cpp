#include "RTCManager.h"

// RTCManager::RTCManager() : timeClient(ntpUDP, "pool.ntp.org", 0, 60000) {}

void RTCManager::begin()
{
    rtc.begin();
    rtc.switch12or24h(true); // Set to 24-hour format by default
    timeClient.begin();
}

void RTCManager::update(bool online)
{
    if (millis() - lastReadTime >= READ_INTERVAL)
    {
        rtc.readTime(tf24, ampm, h, m, s, y, mon, d, w);
        lastReadTime = millis();
    }

    if (online && millis() - lastSyncTime >= SYNC_INTERVAL)
    {
        syncWithNTP();
        lastSyncTime = millis();
    }
}

String RTCManager::getTimeString()
{
    char buf[9];
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d", h, m, s);
    return String(buf);
}

void RTCManager::syncWithNTP()
{
    if (timeClient.update())
    {
        time_t rawTime = timeClient.getEpochTime();
        struct tm *timeinfo = localtime(&rawTime);
        Serial.printf("NTP Time: %02d:%02d:%02d %02d/%02d/%04d\n",
                      timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,
                      timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);

        rtc.setTime(
            timeinfo->tm_year % 100,
            timeinfo->tm_mon + 1,
            timeinfo->tm_mday,
            timeinfo->tm_wday,
            timeinfo->tm_hour,
            timeinfo->tm_min,
            timeinfo->tm_sec,
            true);

        Serial.println("RTC synced with NTP");
    }
    else
    {
        Serial.println("Failed to sync with NTP");
    }
}
