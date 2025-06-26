#ifndef SD3078_H
#define SD3078_H

#include <Arduino.h>
#include <Wire.h>

class SD3078
{
public:
    SD3078(TwoWire &wire = Wire, uint8_t addr = 0x32);
    void begin(bool charge = true);
    void setTime(uint8_t year, uint8_t mon, uint8_t day, uint8_t week,
                 uint8_t hour, uint8_t minute, uint8_t sec,
                 bool tf12or24, uint8_t amOrpm = 0);
    bool readTime(bool &tf12or24, bool &amOrpm, uint8_t &hour, uint8_t &minute,
                  uint8_t &second, uint8_t &year, uint8_t &mon,
                  uint8_t &day, uint8_t &week);
    float readBatteryVoltage();
    int8_t readTemperature();
    void switch12or24h(bool tf12or24);

private:
    TwoWire *_wire;
    uint8_t _addr;

    void enableWrite();
    void disableWrite();
    void writeReg(uint8_t reg, uint8_t *data, size_t len);
    void readRegs(uint8_t reg, uint8_t *buffer, size_t len);
};

#endif
