#pragma once
#include <Arduino.h>
#include "PCF8574.h"
#include "config.h"

class PCF8574Handler
{
public:
    PCF8574Handler(uint8_t address);
    void begin();
    void update();

    bool readPin(uint8_t index);
    void setPin(uint8_t index, bool state);

private:
    PCF8574 pcf;
    uint8_t inputCache;
    uint32_t lastReadTime;
    static const uint16_t READ_INTERVAL = 20; // 20ms safe interval
};

extern PCF8574Handler pcfHandler;
