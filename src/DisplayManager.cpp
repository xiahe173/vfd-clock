#include "DisplayManager.h"

DisplayManager::DisplayManager(uint8_t rstPin, uint8_t csPin) : vfd(rstPin, csPin) {}

void DisplayManager::begin()
{
    vfd.hardReset();
    vfd.begin();
    vfd.setBrightness(currentBrightness);
}

void DisplayManager::setText(const String &text)
{
    updateDisplay(text);
}

void DisplayManager::setBrightness(uint8_t brightness)
{
    if (brightness > 255)
        brightness = 255;
    else if (brightness < 0)
        brightness = 0;
    if (brightness != currentBrightness)
    {
        currentBrightness = brightness;
        vfd.setBrightness(currentBrightness);
    }
}

void DisplayManager::setAlert(const String &text, uint16_t duration)
{
    // TODO
}

void DisplayManager::show()
{
    // TODO
}

void DisplayManager::stepBrightness(bool increase, uint8_t increment)
{
    if (increase)
    {
        if (currentBrightness + increment <= 255)
        {
            currentBrightness += increment;
        }
    }
    else
    {
        if (currentBrightness >= increment)
        {
            currentBrightness -= increment;
        }
    }
    vfd.setBrightness(currentBrightness);
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
