#include "AIDA64Handler.h"

void AIDA64Handler::begin()
{
    displayText = "        "; // 8 spaces
}

void AIDA64Handler::update()
{
    while (Serial.available())
    {
        int ch = Serial.read();

        if (ch == 0x1B)
        { // ESC
            while (Serial.available() < 1)
                ;
            ch = Serial.read();
            if (ch != 0x40)
                return;

            // Clear screen
            char dummy[6];
            while (Serial.available() < 6)
                ;
            Serial.readBytes(dummy, 6);
            displayText = "        ";
            return;
        }

        if (ch == 0x1F)
        {
            while (Serial.available() < 1)
                ;
            ch = Serial.read();
            if (ch == 0x58)
            {
                while (Serial.available() < 1)
                    ;
                ch = Serial.read();
                // Brightness control
                return;
            }
            return;
        }

        if (ch == 0x0C)
        {
            // Form Feed, ignore
            return;
        }

        // Text data
        static char buffer[40] = {0};
        buffer[0] = ch;
        while (Serial.available() < 39)
            ;
        Serial.readBytes(buffer + 1, 39);

        char lineBuffer[9] = {0};
        for (int i = 0; i < 8; i++)
        {
            lineBuffer[i] = (i < 16) ? buffer[i] : ' ';
        }

        displayText = String(lineBuffer);
    }
}

String AIDA64Handler::getDisplayText()
{
    return displayText;
}
