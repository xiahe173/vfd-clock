#include <Arduino.h>
#include <Wire.h>
#include <SD3078.h>
#include <PCF8574.h>
#include <VFD_8MD06INKM.h>

#include "notes.h"

#define BUZZER_PIN 16

#define VFD_RST_PIN 12
#define VFD_CS_PIN 15

#define HV_EN_PIN P1
#define F_EN_PIN P0

VFD_8MD06INKM vfd(VFD_RST_PIN, VFD_CS_PIN);
PCF8574 pcf8574(0x20);
SD3078 rtc;

bool tf24, ampm;
uint8_t h, m, s, y, mon, d, w;

void setup()
{
    Serial.begin(115200);

    Serial.println("VFD Clock test");

    pcf8574.pinMode(HV_EN_PIN, OUTPUT);
    pcf8574.pinMode(F_EN_PIN, OUTPUT);

    Serial.print("Init pcf8574...");
    if (pcf8574.begin())
        Serial.println("OK");
    else
        Serial.println("Failed");

    pcf8574.digitalWrite(HV_EN_PIN, HIGH); // enable 24v boost
    pcf8574.digitalWrite(F_EN_PIN, LOW);   // enable filament

    // Init VFD
    vfd.hardReset();
    vfd.begin();
    vfd.setBrightness(128);

    vfd.printChar(0, 'H');
    vfd.printChar(1, 'e');
    vfd.printChar(2, 'l');
    vfd.printChar(3, 'l');
    vfd.printChar(4, 'o');
    vfd.printChar(5, '!');

    // Init RTC
    rtc.begin();

    // Play a series of notes
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
    int notes[] = {NE4, NB3, NGS4, NB3, NE4, NB3, NFS4, NB4};
    uint8_t lengths[] = {2, 2, 2, 2, 2, 2, 2, 4};

    for (uint8_t i = 0; i < sizeof(notes) / sizeof(notes[0]); i++)
    {
        tone(BUZZER_PIN, notes[i] * 2, 150 * lengths[i]);
        delay(150 * lengths[i]);
        noTone(BUZZER_PIN);
    }
}

void loop()
{
    // Display time on VFD
    rtc.readTime(tf24, ampm, h, m, s, y, mon, d, w);
    Serial.printf("20%02d-%02d-%02d %02d:%02d:%02d\n", y, mon, d, h, m, s);

    vfd.printChar(0, h / 10 + '0');
    vfd.printChar(1, h % 10 + '0');
    vfd.printChar(2, ':');
    vfd.printChar(3, m / 10 + '0');
    vfd.printChar(4, m % 10 + '0');
    vfd.printChar(5, ':');
    vfd.printChar(6, s / 10 + '0');
    vfd.printChar(7, s % 10 + '0');

    delay(200);
}
