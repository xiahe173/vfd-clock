#pragma once
#include <Arduino.h>

class SerialManager
{
public:
    void begin(unsigned long baud = 115200);
    void update();
    String getLatestData();

private:
    char buffer[9];
    int bufferIndex;
    char latestData[9];
};
