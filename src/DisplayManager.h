#pragma once
#include <Arduino.h>
#include <VFD_8MD06INKM.h>

class DisplayManager
{
public:
    DisplayManager(uint8_t rstPin, uint8_t csPin);
    void begin();
    void setText(const String &text);
    void setTextScroll(const String &startText, const String &endText);
    void cancelAnimation();
    void updateAnimation();
    bool isAnimationActive() const { return scrollActive; }
    String getLastDisplayedText() const { return lastDisplayed; }
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

    // Scroll animation state
    static const uint8_t SCROLL_FRAMES = 8;
    static const uint16_t SCROLL_FRAME_MS = 50;

    bool scrollActive = false;
    uint8_t scrollFrame = 0;
    uint32_t scrollLastTime = 0;
    String scrollEndText;
    bool scrollPositions[8] = {false};
    uint8_t scrollOldFont[8][5];
    uint8_t scrollNewFont[8][5];

    void loadFontData(uint8_t dest[5], char c);
    void startScroll(const String &startText, const String &endText);
};
