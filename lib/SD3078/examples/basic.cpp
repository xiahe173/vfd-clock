#include <Arduino.h>
#include <SD3078.h>

SD3078 rtc;

bool tf24, ampm;
uint8_t h, m, s, y, mon, d, w;

void setup()
{
    Serial.begin(115200);
    Serial.println("SD3078 test");

    rtc.begin();
    rtc.setTime(25, 6, 24, 1, 10, 51, 0, false, 1); // Set time to 24.06.2025 22:51:00 PM
    rtc.switch12or24h(true);                        // Switch to 24 hour format

    Serial.printf("Battery voltage: %.2fV, Temperature: %d°C\n",
                  rtc.readBatteryVoltage(),
                  rtc.readTemperature());
}

void loop()
{
    rtc.readTime(tf24, ampm, h, m, s, y, mon, d, w);
    Serial.printf("Time: %02d:%02d:%02d, Date: %02d.%02d.%02d, Week: %d, Format: %s, AM/PM: %s\n",
                  h, m, s, d, mon, y, w, tf24 ? "24h" : "12h", ampm ? "PM" : "AM");
    delay(1000);
}
