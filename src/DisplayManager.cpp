#include "DisplayManager.h"
#include "Font5x7.h"

DisplayManager::DisplayManager(uint8_t rstPin, uint8_t csPin) : vfd(rstPin, csPin) {}

void DisplayManager::begin()
{
    vfd.hardReset();
    vfd.begin();
    vfd.setBrightness(currentBrightness);
}

void DisplayManager::setText(const String &text)
{
    cancelAnimation();
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

void DisplayManager::cancelAnimation()
{
    scrollActive = false;
}

void DisplayManager::loadFontData(uint8_t dest[5], char c)
{
    const uint8_t *data = getFontData(c);
    memcpy(dest, data, 5);
}

void DisplayManager::startScroll(const String &startText, const String &endText)
{
    scrollEndText = endText;

    String paddedStart = startText;
    String paddedEnd = endText;
    while (paddedStart.length() < 8) paddedStart += ' ';
    while (paddedEnd.length() < 8) paddedEnd += ' ';

    for (uint8_t i = 0; i < 8; i++)
    {
        if (paddedStart[i] == paddedEnd[i])
        {
            scrollPositions[i] = false;
        }
        else
        {
            scrollPositions[i] = true;
            loadFontData(scrollOldFont[i], paddedStart[i]);
            loadFontData(scrollNewFont[i], paddedEnd[i]);
        }
    }

    scrollFrame = 0;
    scrollLastTime = millis();
    scrollActive = true;

    // Render frame 0 (all old text) for seamless transition from ROM chars
    for (uint8_t pos = 0; pos < 8; pos++)
    {
        if (scrollPositions[pos])
        {
            vfd.printBits(pos, scrollOldFont[pos]);
        }
    }
}

void DisplayManager::setTextScroll(const String &startText, const String &endText)
{
    if (startText == endText)
    {
        updateDisplay(endText);
        return;
    }

    startScroll(startText, endText);
}

void DisplayManager::updateAnimation()
{
    if (!scrollActive)
        return;

    uint32_t now = millis();
    if (now - scrollLastTime < SCROLL_FRAME_MS)
        return;

    scrollFrame++;
    scrollLastTime = now;

    if (scrollFrame >= SCROLL_FRAMES)
    {
        scrollActive = false;
        updateDisplay(scrollEndText);
        return;
    }

    uint8_t f = scrollFrame;
    uint8_t maskNew = ~((1 << (7 - f)) - 1) & 0x7F;
    uint8_t maskOld = (1 << (7 - f)) - 1;

    for (uint8_t pos = 0; pos < 8; pos++)
    {
        if (!scrollPositions[pos])
            continue;

        uint8_t frameBits[5];
        for (uint8_t col = 0; col < 5; col++)
        {
            frameBits[col] = ((scrollOldFont[pos][col] >> f) & maskOld)
                           | ((scrollNewFont[pos][col] << (7 - f)) & maskNew);
        }
        vfd.printBits(pos, frameBits);
    }
}
