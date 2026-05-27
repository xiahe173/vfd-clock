#pragma once
#include <Arduino.h>
#include <VFD_8MD06INKM.h>

enum class ScrollDirection { Up, Down };
enum class Easing { Linear, EaseInOut };

class DisplayManager
{
public:
    DisplayManager(uint8_t rstPin, uint8_t csPin);
    void begin();
    void setText(const String &text);
    void setTextScroll(const String &startText, const String &endText,
                       ScrollDirection direction = ScrollDirection::Up,
                       Easing easing = Easing::Linear);
    void cancelAnimation();
    void updateAnimation();
    bool isAnimationActive() const { return scrollActive; }
    String getLastDisplayedText() const { return lastDisplayed; }
    void setAlert(const String &text, uint16_t duration = 800);
    void show();

    void setBrightness(uint8_t brightness);
    void stepBrightness(bool increase, uint8_t increment = 32);

private:
    VFD_8MD06INKM vfd;
    String lastDisplayed;
    String displayText;
    void updateDisplay(const String &text);

    uint8_t currentBrightness = 128;

    // Scroll animation state
    static const uint8_t SCROLL_FRAMES = 8;

    bool scrollActive = false;
    uint8_t scrollFrame = 0;
    uint32_t scrollLastTime = 0;
    String scrollEndText;
    bool scrollPositions[8] = {false};
    uint8_t scrollOldFont[8][5];
    uint8_t scrollNewFont[8][5];

    ScrollDirection scrollDirection = ScrollDirection::Up;
    uint8_t scrollDelays[8];

    // Alert state
    bool alertActive = false;
    uint32_t alertEndTime = 0;
    uint16_t alertDuration = 0;
    String alertDisplayText;
    String bufferedText;
    bool alertEntryActive = false;
    bool alertExitActive = false;

    void loadFontData(uint8_t dest[5], char c);
    void startScroll(const String &startText, const String &endText,
                     ScrollDirection direction, Easing easing);
    void setEasingDelays(Easing easing);
    void renderScrollFrame(uint8_t f);
};
