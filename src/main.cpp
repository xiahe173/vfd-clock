#include <Arduino.h>

#include "PCF8574Handler.h"
#include "ButtonHandler.h"
#include "modeManager.h"
#include "RTCManager.h"
#include "serialManager.h"
#include "AIDA64Handler.h"
#include "displayManager.h"
#include "notes.h"
#include "config.h"

ButtonHandler buttonHandler;
ModeManager modeManager;
RTCManager rtcManager;
SerialManager serialManager;
AIDA64Handler aida64Handler;
DisplayManager displayManager(VFD_RST_PIN, VFD_CS_PIN);

void setup()
{
    pcfHandler.begin();
    buttonHandler.begin();
    modeManager.begin();
    serialManager.begin(9600);
    aida64Handler.begin();
    displayManager.begin();
    rtcManager.begin();

    pinMode(BUZZER_PIN, OUTPUT);

    // COP - Summer Breeze
    // int notes[] = {ND4, NE4, NB4, NA4, ND5, NB4,
    //                ND4, NE4, NB4, NA4, ND5, NA4, NFS4,
    //                NG4, NE4};
    // uint8_t lengths[] = {2, 2, 3, 3, 2, 4,
    //                     2, 2, 3, 3, 2, 3, 3,
    //                     2, 8};

    // COP - Summer Breeze (Short)
    // int notes[] = {ND4, NE4, NB4, NA4, ND5, NA4, NFS4, NG4, NE4};
    // uint8_t lengths[] = {2, 2, 3, 3, 2, 3, 3, 2, 6};

    // MTR ring
    // int notes[] = {NE4, NB3, NGS4, NB3, NE4, NB3, NFS4, NB4};
    // uint8_t lengths[] = {2, 2, 2, 2, 2, 2, 2, 4};

    // for (uint8_t i = 0; i < sizeof(notes) / sizeof(notes[0]); i++)
    // {
    //     tone(BUZZER_PIN, notes[i] * 2, 150 * lengths[i]);
    //     delay(150 * lengths[i]);
    //     noTone(BUZZER_PIN);
    // }

    tone(BUZZER_PIN, 4100, 500);
    delay(300);
    noTone(BUZZER_PIN);

    displayManager.showText(" Hello! ");
}

void loop()
{
    pcfHandler.update();
    buttonHandler.update();
    if (buttonHandler.wasShortPressed(0))
    {
        modeManager.nextMode();
    }

    if (buttonHandler.wasShortPressed(2))
    {
        displayManager.stepBrightness(true);
    }
    if (buttonHandler.wasShortPressed(3))
    {
        displayManager.stepBrightness(false);
    }

    switch (modeManager.getCurrentMode())
    {
    case Mode::Standalone:
        rtcManager.update();
        displayManager.showText(rtcManager.getTimeString());
        break;
    case Mode::Serial:
        serialManager.update();
        displayManager.showText(serialManager.getLatestData());
        break;
    case Mode::AIDA64:
        aida64Handler.update();
        displayManager.showText(aida64Handler.getDisplayText());
        break;
    default:
        break;
    }
}
