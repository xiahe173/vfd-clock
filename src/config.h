#pragma once

// #define VFD_HW_VER_0_3
#define VFD_HW_VER_0_2

#ifdef VFD_HW_VER_0_2

#define BUZZER_PIN 16

#define VFD_RST_PIN 12
#define VFD_CS_PIN 15

#define HV_EN_PIN P1
#define F_EN_PIN P0

#define PCF_ADDRESS 0x20

#define BTN_MODE 0
#define BTN_NTP_SYNC 1
#define BTN_BRIGHTNESS_UP 2
#define BTN_BRIGHTNESS_DOWN 3

#endif

#ifdef VFD_HW_VER_0_3

#define BUZZER_PIN 15

#define VFD_RST_PIN 12
#define VFD_CS_PIN 16

#define HV_EN_PIN P3
#define F_EN_PIN P2

#define PCF_ADDRESS 0x20

#define BTN_MODE 3
#define BTN_NTP_SYNC 2
#define BTN_BRIGHTNESS_UP 0
#define BTN_BRIGHTNESS_DOWN 1

#endif