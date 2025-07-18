#pragma once
#include <Arduino.h>
#include <VFD_8MD06INKM.h>

class DisplayManager
{
public:
    DisplayManager(uint8_t rstPin, uint8_t csPin);
    void begin();
    void setText(const String &text);
    void setAlert(const String &text, uint16_t duration = 800); // TODO
    void show();

    void setBrightness(uint8_t brightness);
    void stepBrightness(bool increase, uint8_t increment = 32);

private:
    VFD_8MD06INKM vfd;
    String lastDisplayed;
    String displayText;
    void updateDisplay(const String &text);

    uint8_t currentBrightness = 128;

    bool alertFlag = false;
    uint32_t alertStartTime = 0;
    char alertText[9] = {0};
};
