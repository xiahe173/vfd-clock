#include <Arduino.h>
#include <VFD_8MD06INKM.h>

VFD_8MD06INKM vfd(12, 15); // RST pin 12, CS pin 15

uint8_t interval = 20;

void setup()
{
    // init vfd
    vfd.hardReset();
    vfd.begin();
    vfd.setBrightness(128);
}

void loop()
{
    // display a flashing bar
    for (uint8_t i = 0; i < 8; i++)
    {
        if (i > 0)
            vfd.printChar(i - 1, ' ');
        else
            vfd.printChar(7, ' ');
        vfd.printChar(i, '|');
        delay(interval);
    }
}
