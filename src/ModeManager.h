#pragma once

enum class Mode
{
    Standalone,
    Serial,
    AIDA64,
    MODE_COUNT
};

class ModeManager
{
public:
    void begin();
    void nextMode();
    Mode getCurrentMode() const;

private:
    Mode currentMode = Mode::Standalone;
};
