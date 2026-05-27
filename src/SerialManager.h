#pragma once
#include <Arduino.h>

class SerialManager
{
public:
    void begin(unsigned long baud = 115200);
    void update();
    String getLatestData();
    void clear();

private:
    static const uint8_t INPUT_SIZE = 128;
    static const uint8_t DISPLAY_WIDTH = 8;

    char inputBuffer[INPUT_SIZE];
    uint8_t inputIndex;
    char displayBuffer[DISPLAY_WIDTH + 1];

    void finalizeLine();
};
