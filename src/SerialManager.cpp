#include "SerialManager.h"
#include <string.h>

void SerialManager::begin(unsigned long baud)
{
    Serial.begin(baud);
    bufferIndex = 0;
    buffer[0] = '\0';
    latestData[0] = '\0';
}

void SerialManager::update()
{
    while (Serial.available())
    {
        char c = Serial.read();
        if (c == '\n')
        {
            buffer[bufferIndex] = '\0';
            strcpy(latestData, buffer);
            bufferIndex = 0;
            buffer[0] = '\0';
        }
        else if (bufferIndex < 8)
        {
            // ensure buffer do not overflow when no \n is received
            buffer[bufferIndex] = c;
            bufferIndex++;
        }
    }
}

String SerialManager::getLatestData()
{
    return String(latestData);
}
