#pragma once
#include <Arduino.h>
#include "config.h"

class ButtonHandler
{
public:
    ButtonHandler();
    void begin();
    void update();

    bool wasShortPressed(uint8_t index);
    bool wasLongPressed(uint8_t index);

private:
    static const uint8_t BUTTON_COUNT = 4;
    static const uint16_t DEBOUNCE_DELAY = 50;
    static const uint16_t LONG_PRESS_TIME = 800;

    enum ButtonState
    {
        RELEASED,
        PRESS_DEBOUNCE,
        PRESSED,
        LONG_PRESSED,
        RELEASE_DEBOUNCE
    };

    ButtonState state[BUTTON_COUNT];
    ButtonState previousState[BUTTON_COUNT];
    uint32_t lastChangeTime[BUTTON_COUNT];
    bool shortPressFlag[BUTTON_COUNT];
    bool longPressFlag[BUTTON_COUNT];
    bool lastBuzzerFlag;
    bool buzzerFlag;
};