#include "PCF8574Handler.h"

// Global instance of PCF8574Handler
PCF8574Handler pcfHandler(PCF_ADDRESS);

PCF8574Handler::PCF8574Handler(uint8_t address) : pcf(address) {}

void PCF8574Handler::begin()
{
    pcf.pinMode(HV_EN_PIN, OUTPUT);
    pcf.pinMode(F_EN_PIN, OUTPUT);

    pcf.pinMode(P4, INPUT);
    pcf.pinMode(P5, INPUT);
    pcf.pinMode(P6, INPUT);
    pcf.pinMode(P7, INPUT);

    Serial.print("Init pcf8574...");
    if (pcf.begin())
        Serial.println("OK");
    else
        Serial.println("Failed");

    pcf.digitalWrite(HV_EN_PIN, HIGH); // enable 24v boost
    pcf.digitalWrite(F_EN_PIN, LOW);   // enable filament

    inputCache = 0xFF;
    lastReadTime = 0;
}

void PCF8574Handler::update()
{
    if (millis() - lastReadTime >= READ_INTERVAL)
    {
        inputCache = pcf.digitalReadAll();
        lastReadTime = millis();
    }
}

bool PCF8574Handler::readPin(uint8_t index)
{
    if (index > 7)
        return false;
    return (inputCache & (1 << index)) != 0;
}

void PCF8574Handler::setPin(uint8_t index, bool state)
{
    if (index > 7)
        return;
    pcf.digitalWrite(index, state ? HIGH : LOW);
}
