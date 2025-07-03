#include "ButtonHandler.h"
#include "PCF8574Handler.h"

ButtonHandler::ButtonHandler()
{
    for (uint8_t i = 0; i < BUTTON_COUNT; i++)
    {
        state[i] = RELEASED;
        previousState[i] = RELEASED;
        lastChangeTime[i] = 0;
        shortPressFlag[i] = false;
        longPressFlag[i] = false;
    }
}

void ButtonHandler::begin()
{
    Serial.println("ButtonHandler initialized");
}

void ButtonHandler::update()
{
    lastBuzzerFlag = buzzerFlag;
    buzzerFlag = false;

    for (uint8_t i = 0; i < BUTTON_COUNT; i++)
    {
        bool pressed = !pcfHandler.readPin(i + 4); // P4 to P7 are buttons

        switch (state[i])
        {
        case RELEASED:
            if (pressed)
            {
                state[i] = PRESS_DEBOUNCE;
                lastChangeTime[i] = millis();
            }
            break;

        case PRESS_DEBOUNCE:
            if (millis() - lastChangeTime[i] >= DEBOUNCE_DELAY)
            {
                if (pressed)
                {
                    state[i] = PRESSED;
                    lastChangeTime[i] = millis();
                }
                else
                {
                    state[i] = RELEASED;
                }
            }
            break;

        case PRESSED:
            buzzerFlag = true;
            if (!pressed)
            {
                previousState[i] = PRESSED;
                state[i] = RELEASE_DEBOUNCE;
                lastChangeTime[i] = millis();
            }
            else if (millis() - lastChangeTime[i] >= LONG_PRESS_TIME)
            {
                state[i] = LONG_PRESSED;
                longPressFlag[i] = true;
            }
            break;

        case LONG_PRESSED:
            if (!pressed)
            {
                previousState[i] = LONG_PRESSED;
                state[i] = RELEASE_DEBOUNCE;
                lastChangeTime[i] = millis();
            }
            break;

        case RELEASE_DEBOUNCE:
            if (millis() - lastChangeTime[i] >= DEBOUNCE_DELAY)
            {
                if (!pressed)
                {
                    if (previousState[i] == PRESSED)
                    {
                        shortPressFlag[i] = true;
                    }
                    state[i] = RELEASED;
                }
                else
                {
                    state[i] = PRESSED;
                }
            }
            break;
        }
    }

    if (buzzerFlag && !lastBuzzerFlag)
    {
        tone(BUZZER_PIN, 4100);
    }
    else if (!buzzerFlag && lastBuzzerFlag)
    {
        noTone(BUZZER_PIN);
    }
}

bool ButtonHandler::wasShortPressed(uint8_t index)
{
    if (index >= BUTTON_COUNT)
        return false;
    if (shortPressFlag[index])
    {
        shortPressFlag[index] = false;
        return true;
    }
    return false;
}

bool ButtonHandler::wasLongPressed(uint8_t index)
{
    if (index >= BUTTON_COUNT)
        return false;
    if (longPressFlag[index])
    {
        longPressFlag[index] = false;
        return true;
    }
    return false;
}
