#pragma once
#include <Arduino.h>
#include <VFD_8MD06INKM.h>

class DisplayManager
{
public:
    DisplayManager(uint8_t rstPin, uint8_t csPin);
    void begin();
    void showText(const String &text);

    void setBrightness(uint8_t brightness);
    void stepBrightness(bool increase, uint8_t increment = 32);

private:
    VFD_8MD06INKM vfd;
    String lastDisplayed;
    void updateDisplay(const String &text);

    uint8_t current_brightness = 128;
};
