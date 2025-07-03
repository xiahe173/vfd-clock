#include "ModeManager.h"

void ModeManager::begin()
{
    currentMode = Mode::Standalone;
}

void ModeManager::nextMode()
{
    int modeInt = static_cast<int>(currentMode);
    modeInt = (modeInt + 1) % static_cast<int>(Mode::MODE_COUNT);
    currentMode = static_cast<Mode>(modeInt);
}

Mode ModeManager::getCurrentMode() const
{
    return currentMode;
}
