#include "SerialManager.h"
#include <string.h>

void SerialManager::begin(unsigned long baud)
{
    Serial.begin(baud);
    clear();
}

void SerialManager::clear()
{
    inputIndex = 0;
    inputBuffer[0] = '\0';
    displayBuffer[0] = '\0';

    while (Serial.available())
        Serial.read();
}

void SerialManager::update()
{
    while (Serial.available())
    {
        char c = Serial.read();

        if (c == '\n')
        {
            finalizeLine();
        }
        else if (c != '\r')
        {
            if (inputIndex >= INPUT_SIZE)
            {
                memmove(inputBuffer, inputBuffer + 1, INPUT_SIZE - 1);
                inputIndex--;
            }
            inputBuffer[inputIndex++] = c;
        }
    }
}

void SerialManager::finalizeLine()
{
    inputBuffer[inputIndex] = '\0';

    uint8_t len = inputIndex;

    if (len >= DISPLAY_WIDTH)
    {
        memcpy(displayBuffer, inputBuffer + len - DISPLAY_WIDTH, DISPLAY_WIDTH);
    }
    else
    {
        uint8_t spaces = DISPLAY_WIDTH - len;
        memset(displayBuffer, ' ', spaces);
        memcpy(displayBuffer + spaces, inputBuffer, len);
    }
    displayBuffer[DISPLAY_WIDTH] = '\0';

    inputIndex = 0;
}

String SerialManager::getLatestData()
{
    if (displayBuffer[0] == '\0')
    {
        return String("  ----  ");
    }
    return String(displayBuffer);
}
