#include <Arduino.h>
#include <WiFiManager.h>

#include "PCF8574Handler.h"
#include "ButtonHandler.h"
#include "ModeManager.h"
#include "RTCManager.h"
#include "SerialManager.h"
#include "AIDA64Handler.h"
#include "DisplayManager.h"
#include "notes.h"
#include "config.h"

WiFiManager wm;
ButtonHandler buttonHandler;
ModeManager modeManager;
RTCManager rtcManager;
SerialManager serialManager;
AIDA64Handler aida64Handler;
DisplayManager displayManager(VFD_RST_PIN, VFD_CS_PIN);

void setup()
{
    serialManager.begin(9600);
    pcfHandler.begin();
    buttonHandler.begin();
    modeManager.begin();
    aida64Handler.begin();
    displayManager.begin();
    rtcManager.begin();

    WiFi.mode(WIFI_STA);
    wm.setConfigPortalBlocking(false);

    displayManager.setText("WiFi -? ");
    if (wm.autoConnect())
    {
        Serial.println("connected...yeey :)");
        displayManager.setText("WiFi OK ");
        rtcManager.syncWithNTP();
    }
    else
    {
        Serial.println("Configportal running");
        displayManager.setText("No WiFi ");
    }

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
    delay(2000);
}

void loop()
{
    wm.process();
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
    if (buttonHandler.wasLongPressed(0))
    {
        ESP.restart();
    }
    if (buttonHandler.wasLongPressed(1))
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            rtcManager.syncWithNTP();
        }
    }
    if (buttonHandler.wasLongPressed(2))
    {
        wm.erase();
        ESP.restart();
    }

    switch (modeManager.getCurrentMode())
    {
    case Mode::Standalone:
        rtcManager.update(WiFi.status() == WL_CONNECTED);
        displayManager.setText(rtcManager.getTimeString());
        break;
    case Mode::Serial:
        serialManager.update();
        displayManager.setText(serialManager.getLatestData());
        break;
    case Mode::AIDA64:
        aida64Handler.update();
        displayManager.setText(aida64Handler.getDisplayText());
        break;
    default:
        break;
    }
}

// #include <Arduino.h>
// #include <SD3078.h>

// SD3078 rtc;

// bool tf24, ampm;
// uint8_t h, m, s, y, mon, d, w;

// void setup()
// {
//     Serial.begin(115200);
//     Serial.println("SD3078 test");

//     rtc.begin();
//     // rtc.setTime(25, 6, 24, 1, 10, 51, 0, false, 1); // Set time to 24.06.2025 22:51:00 PM
//     rtc.switch12or24h(true); // Switch to 24 hour format

//     Serial.printf("Battery voltage: %.2fV, Temperature: %d°C\n",
//                   rtc.readBatteryVoltage(),
//                   rtc.readTemperature());
// }

// void loop()
// {
//     rtc.readTime(tf24, ampm, h, m, s, y, mon, d, w);
//     Serial.printf("Time: %02d:%02d:%02d, Date: %02d.%02d.%02d, Week: %d, Format: %s, AM/PM: %s\n",
//                   h, m, s, d, mon, y, w, tf24 ? "24h" : "12h", ampm ? "PM" : "AM");
//     delay(1000);
// }
