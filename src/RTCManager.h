#pragma once
#include <SD3078.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

class RTCManager
{
public:
    void begin();
    void update(bool online = false);
    String getTimeString();
    void syncWithNTP();

private:
    SD3078 rtc;
    WiFiUDP ntpUDP;
    NTPClient timeClient{ntpUDP, "0.cn.pool.ntp.org", 60 * 60 * 8, 1000 * 20};

    uint8_t h, m, s, y, mon, d, w;
    bool tf24, ampm;
    uint32_t lastReadTime = 0;
    uint32_t lastSyncTime = 0;
    static const uint16_t READ_INTERVAL = 1000;
    static const uint16_t SYNC_INTERVAL = 1000 * 60; // 1 minute
};
