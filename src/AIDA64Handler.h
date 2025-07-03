#pragma once
#include <Arduino.h>

class AIDA64Handler
{
public:
    void begin();
    void update();
    String getDisplayText();

private:
    String displayText;
};
