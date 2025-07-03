#include "RTCManager.h"

void RTCManager::begin()
{
    rtc.begin();
    // rtc.switch12or24h(true);
}

void RTCManager::update()
{
    if (millis() - lastReadTime >= READ_INTERVAL)
    {
        rtc.readTime(tf24, ampm, h, m, s, y, mon, d, w);
        lastReadTime = millis();
    }
}

String RTCManager::getTimeString()
{
    char buf[9];
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d", h, m, s);
    return String(buf);
}
