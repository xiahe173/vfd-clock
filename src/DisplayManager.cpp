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
    if (alertActive)
    {
        bufferedText = text;
        return;
    }
    cancelAnimation();
    updateDisplay(text);
}

void DisplayManager::setTextScroll(const String &startText, const String &endText,
                                   ScrollDirection direction, Easing easing)
{
    if (alertActive)
    {
        bufferedText = endText;
        return;
    }
    if (startText == endText)
    {
        updateDisplay(endText);
        return;
    }
    startScroll(startText, endText, direction, easing);
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
    cancelAnimation();

    if (!alertActive)
    {
        bufferedText = lastDisplayed;
    }

    alertDisplayText = text;
    alertDuration = duration;
    alertActive = true;
    alertEntryActive = true;
    alertExitActive = false;
    alertEndTime = 0;

    startScroll(lastDisplayed, alertDisplayText, ScrollDirection::Up, Easing::Linear);
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

void DisplayManager::setEasingDelays(Easing easing)
{
    static const uint8_t delaysLinear[8] = {50, 50, 50, 50, 50, 50, 50, 50};
    static const uint8_t delaysEaseInOut[8] = {75, 55, 40, 30, 30, 40, 55, 75};

    const uint8_t *src = (easing == Easing::Linear) ? delaysLinear : delaysEaseInOut;
    memcpy(scrollDelays, src, 8);
}

void DisplayManager::renderScrollFrame(uint8_t f)
{
    uint8_t maskOld, maskNew;

    if (scrollDirection == ScrollDirection::Up)
    {
        maskOld = (1 << (7 - f)) - 1;
        maskNew = ~maskOld & 0x7F;

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
    else
    {
        maskNew = (1 << f) - 1;
        maskOld = ~maskNew & 0x7F;

        for (uint8_t pos = 0; pos < 8; pos++)
        {
            if (!scrollPositions[pos])
                continue;

            uint8_t frameBits[5];
            for (uint8_t col = 0; col < 5; col++)
            {
                frameBits[col] = ((scrollOldFont[pos][col] << f) & maskOld)
                               | (scrollNewFont[pos][col] & maskNew);
            }
            vfd.printBits(pos, frameBits);
        }
    }
}

void DisplayManager::startScroll(const String &startText, const String &endText,
                                 ScrollDirection direction, Easing easing)
{
    scrollEndText = endText;
    scrollDirection = direction;
    setEasingDelays(easing);

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

    renderScrollFrame(0);
}

void DisplayManager::updateAnimation()
{
    if (!scrollActive)
    {
        if (alertActive && !alertExitActive && alertEndTime > 0
            && millis() >= alertEndTime)
        {
            alertExitActive = true;
            String target = bufferedText.length() > 0 ? bufferedText : lastDisplayed;
            startScroll(alertDisplayText, target, ScrollDirection::Down, Easing::Linear);
        }
        return;
    }

    uint32_t now = millis();
    if (now - scrollLastTime < scrollDelays[scrollFrame])
        return;

    scrollFrame++;
    scrollLastTime = now;

    if (scrollFrame >= SCROLL_FRAMES)
    {
        scrollActive = false;

        if (alertEntryActive)
        {
            alertEntryActive = false;
            alertEndTime = millis() + alertDuration;
        }
        else if (alertExitActive)
        {
            alertExitActive = false;
            alertActive = false;
        }

        updateDisplay(scrollEndText);
        return;
    }

    renderScrollFrame(scrollFrame);
}
