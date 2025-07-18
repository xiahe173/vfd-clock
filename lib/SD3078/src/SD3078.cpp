#include "SD3078.h"

SD3078::SD3078(TwoWire &wire, uint8_t addr)
{
    _wire = &wire;
    _addr = addr;
}

void SD3078::begin(bool charge)
{
    _wire->begin();
    if (charge)
    {
        enableWrite();
        uint8_t buf[1] = {0x82};
        writeReg(0x18, buf, 1);
        disableWrite();
    }
}

void SD3078::writeReg(uint8_t reg, uint8_t *data, size_t len)
{
    _wire->beginTransmission(_addr);
    _wire->write(reg);
    _wire->write(data, len);
    _wire->endTransmission();
}

void SD3078::readRegs(uint8_t reg, uint8_t *buffer, size_t len)
{
    _wire->beginTransmission(_addr);
    _wire->write(reg);
    _wire->endTransmission(false);
    _wire->requestFrom(_addr, (uint8_t)len);
    for (size_t i = 0; i < len && _wire->available(); i++)
    {
        buffer[i] = _wire->read();
    }
}

void SD3078::enableWrite()
{
    uint8_t a[] = {0x80};
    writeReg(0x10, a, 1);
    uint8_t b[] = {0x84};
    writeReg(0x0F, b, 1);
}

void SD3078::disableWrite()
{
    uint8_t buf[2];
    readRegs(0x0F, buf, 2);
    buf[0] &= 0x7B;
    buf[1] &= 0x7F;
    writeReg(0x0F, buf, 2);
}

void SD3078::setTime(uint8_t year, uint8_t mon, uint8_t day, uint8_t week,
                     uint8_t hour, uint8_t minute, uint8_t sec,
                     bool tf12or24, uint8_t amOrpm)
{
    if (!tf12or24 && (hour == 0 || hour > 12))
    {
        Serial.println("Invalid hour for 12-hour format. Hour must be between 1 and 12.");
        return;
    }

    uint8_t buf[7] = {
        (uint8_t)(((sec / 10) << 4) | (sec % 10)),
        (uint8_t)(((minute / 10) << 4) | (minute % 10)),
        (uint8_t)((tf12or24 << 7) | (amOrpm << 5) | ((hour / 10) << 4) | (hour % 10)),
        (uint8_t)(week & 0x07),
        (uint8_t)(((day / 10) << 4) | (day % 10)),
        (uint8_t)(((mon / 10) << 4) | (mon % 10)),
        (uint8_t)(((year / 10) << 4) | (year % 10))};

    enableWrite();
    writeReg(0x00, buf, 7);
    disableWrite();
}

bool SD3078::readTime(bool &tf12or24, bool &amOrpm, uint8_t &hour,
                      uint8_t &minute, uint8_t &second, uint8_t &year,
                      uint8_t &mon, uint8_t &day, uint8_t &week)
{
    uint8_t buf[7];
    readRegs(0x00, buf, 7);

    second = ((buf[0] & 0x70) >> 4) * 10 + (buf[0] & 0x0F);
    minute = ((buf[1] & 0x70) >> 4) * 10 + (buf[1] & 0x0F);
    tf12or24 = (buf[2] & 0x80) != 0;

    if (tf12or24)
    {
        hour = ((buf[2] & 0x30) >> 4) * 10 + (buf[2] & 0x0F);
        amOrpm = false;
    }
    else
    {
        amOrpm = (buf[2] & 0x20) != 0;
        hour = ((buf[2] & 0x10) >> 4) * 10 + (buf[2] & 0x0F);
    }

    week = buf[3] & 0x07;
    day = ((buf[4] & 0x30) >> 4) * 10 + (buf[4] & 0x0F);
    mon = ((buf[5] & 0x10) >> 4) * 10 + (buf[5] & 0x0F);
    year = ((buf[6] & 0xF0) >> 4) * 10 + (buf[6] & 0x0F);

    return true;
}

float SD3078::readBatteryVoltage()
{
    uint8_t buf[2];
    readRegs(0x1A, buf, 2);
    return (((buf[0] & 0x80) << 1) + buf[1]) / 100.0f;
}

int8_t SD3078::readTemperature()
{
    uint8_t temp;
    readRegs(0x16, &temp, 1);
    return (temp & 0x80) ? (int8_t)(temp - 256) : temp;
}

void SD3078::switch12or24h(bool tf12or24)
{
    uint8_t hbuf;
    readRegs(0x02, &hbuf, 1);

    enableWrite();
    if ((hbuf & 0x80) && !tf12or24)
    { // 24h → 12h
        uint8_t hour = ((hbuf & 0x30) >> 4) * 10 + (hbuf & 0x0F);
        if (hour > 11)
        {
            hour = (hour == 12) ? 12 : hour - 12;
            uint8_t b = 0x20 | ((hour / 10) << 4) | (hour % 10);
            writeReg(0x02, &b, 1);
        }
        else
        {
            hour = (hour == 0) ? 12 : hour;
            uint8_t b = ((hour / 10) << 4) | (hour % 10);
            writeReg(0x02, &b, 1);
        }
    }
    else if (!(hbuf & 0x80) && tf12or24)
    { // 12h → 24h
        bool pm = hbuf & 0x20;
        uint8_t hour = ((hbuf & 0x10) >> 4) * 10 + (hbuf & 0x0F);
        if (pm)
        {
            hour = (hour == 12) ? 12 : hour + 12;
        }
        else
        {
            hour = (hour == 12) ? 0 : hour;
        }
        uint8_t b = 0x80 | ((hour / 10) << 4) | (hour % 10);
        writeReg(0x02, &b, 1);
    }
    disableWrite();
}
