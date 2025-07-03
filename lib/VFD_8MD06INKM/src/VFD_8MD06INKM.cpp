#include "VFD_8MD06INKM.h"

VFD_8MD06INKM::VFD_8MD06INKM(uint8_t rstPin, uint8_t csPin)
    : _rstPin(rstPin), _csPin(csPin) {}

void VFD_8MD06INKM::begin()
{
    pinMode(_rstPin, OUTPUT);
    pinMode(_csPin, OUTPUT);

    digitalWrite(_rstPin, HIGH);
    digitalWrite(_csPin, HIGH);

    SPI.begin();
    SPI.setBitOrder(LSBFIRST);

    delay(100);

    sendDataBatch(0xE0, 0x07); // Initialization command
    sendDataBatch(0xE8, 0x00);
}

void VFD_8MD06INKM::hardReset()
{
    SPI.end();
    pinMode(_rstPin, OUTPUT);

    digitalWrite(_rstPin, LOW);
    delay(50);
    digitalWrite(_rstPin, HIGH);
    delay(2);

    SPI.begin();
    SPI.setBitOrder(LSBFIRST);
}

void VFD_8MD06INKM::setBrightness(uint8_t brightness)
{
    sendDataBatch(0xE4, brightness);
}

void VFD_8MD06INKM::sendDataBatch(uint8_t command, uint8_t data)
{
    digitalWrite(_csPin, LOW);
    sendData(command);
    delay(1);
    sendData(data);
    delay(1);
    digitalWrite(_csPin, HIGH);
}

void VFD_8MD06INKM::sendData(uint8_t data)
{
    SPI.transfer(data);
}

void VFD_8MD06INKM::printChar(uint8_t position, char c)
{
    printCode(position, (uint8_t)c);
}

void VFD_8MD06INKM::printCode(uint8_t position, uint8_t code)
{
    digitalWrite(_csPin, LOW);
    sendData(0x20 + position);
    delay(1);
    sendData(code);
    digitalWrite(_csPin, HIGH);
}

void VFD_8MD06INKM::printBits(uint8_t position, const uint8_t bits[5])
{
    writeCustomizeToRam(position, bits);
    printCode(position, position);
}

void VFD_8MD06INKM::clearDisplay()
{
    for (uint8_t i = 0; i < 8; ++i)
    {
        printCode(i, 0x00);
    }
}

void VFD_8MD06INKM::printString(const char *str)
{
    for (uint8_t i = 0; i < 8 && str[i] != '\0'; ++i)
    {
        printChar(i, str[i]);
    }
}

void VFD_8MD06INKM::printString(const char *str, uint8_t startPos, uint8_t endPos)
{
    for (uint8_t i = startPos; i <= endPos && str[i - startPos] != '\0'; ++i)
    {
        printChar(i, str[i - startPos]);
    }
}

void VFD_8MD06INKM::writeCustomizeToRam(uint8_t position, const uint8_t data[5])
{
    digitalWrite(_csPin, LOW);
    sendData(0x40 + position);
    delay(1);
    for (int i = 0; i < 5; ++i)
    {
        sendData(data[i]);
        delay(1);
    }
    digitalWrite(_csPin, HIGH);
}
