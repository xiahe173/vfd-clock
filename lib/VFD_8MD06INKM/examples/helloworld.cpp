#include <Arduino.h>
#include <VFD_8MD06INKM.h>

VFD_8MD06INKM vfd(12, 15); // RST pin 12, CS pin 15

uint8_t count = 0;

void setup()
{
    // init vfd
    vfd.hardReset();
    vfd.begin();
    vfd.setBrightness(128); // Set brightness to 128 (0-255)

    // print some characters
    vfd.printString("Hello");
    vfd.printChar(5, '!');

    // create a custom character
    uint8_t box[6] = {
        0b01111111,
        0b01000001,
        0b01000001,
        0b01000001,
        0b01111111};

    vfd.printBits(2, box);
}

void loop()
{
    // print an incrementing number
    vfd.printCode(6, count / 10 + '0');
    vfd.printCode(7, count % 10 + '0');

    count++;
    if (count >= 100)
        count = 0;

    delay(1000);
}
