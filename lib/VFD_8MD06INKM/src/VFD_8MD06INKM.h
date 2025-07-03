#ifndef VFD_8MD06INKM_H
#define VFD_8MD06INKM_H

#include <Arduino.h>
#include <SPI.h>

class VFD_8MD06INKM
{
public:
    VFD_8MD06INKM(uint8_t rstPin, uint8_t csPin);
    void begin();
    void hardReset();
    void setBrightness(uint8_t brightness);
    void printChar(uint8_t position, char c);
    void printCode(uint8_t position, uint8_t code);
    void printBits(uint8_t position, const uint8_t bits[5]);

    void clearDisplay();
    void printString(const char *str);
    void printString(const char *str, uint8_t startPos, uint8_t endPos);

private:
    uint8_t _rstPin, _csPin;
    void sendDataBatch(uint8_t command, uint8_t data);
    void sendData(uint8_t data);
    void writeCustomizeToRam(uint8_t position, const uint8_t data[5]);
};

#endif
