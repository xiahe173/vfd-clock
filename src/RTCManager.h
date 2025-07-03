#pragma once
#include <Arduino.h>
#include <SD3078.h>

class RTCManager
{
public:
    void begin();
    void update();
    String getTimeString();

private:
    SD3078 rtc;
    uint8_t h, m, s, y, mon, d, w;
    bool tf24, ampm;
    uint32_t lastReadTime = 0;
    static const uint16_t READ_INTERVAL = 200;
};
