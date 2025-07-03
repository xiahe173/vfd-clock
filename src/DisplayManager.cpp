#include "DisplayManager.h"

DisplayManager::DisplayManager(uint8_t rstPin, uint8_t csPin) : vfd(rstPin, csPin) {}

void DisplayManager::begin()
{
    vfd.hardReset();
    vfd.begin();
    vfd.setBrightness(current_brightness);
}

void DisplayManager::showText(const String &text)
{
    updateDisplay(text);
}

void DisplayManager::setBrightness(uint8_t brightness)
{
    if (brightness > 255)
        brightness = 255;
    else if (brightness < 0)
        brightness = 0;
    if (brightness != current_brightness)
    {
        current_brightness = brightness;
        vfd.setBrightness(current_brightness);
    }
}

void DisplayManager::stepBrightness(bool increase, uint8_t increment)
{
    if (increase)
    {
        if (current_brightness + increment <= 255)
        {
            current_brightness += increment;
        }
    }
    else
    {
        if (current_brightness >= increment)
        {
            current_brightness -= increment;
        }
    }
    vfd.setBrightness(current_brightness);
}

void DisplayManager::updateDisplay(const String &text)
{
    if (text != lastDisplayed)
    {
        // if length is less than 8, pad with spaces
        String paddedText = text;
        while (paddedText.length() < 8)
        {
            paddedText += ' ';
        }

        vfd.printString(paddedText.c_str());
        lastDisplayed = text;
    }
}
