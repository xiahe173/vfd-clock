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

#endif

#ifdef VFD_HW_VER_0_3

#define BUZZER_PIN 15

#define VFD_RST_PIN 12
#define VFD_CS_PIN 16

#define HV_EN_PIN P3
#define F_EN_PIN P2

#define PCF_ADDRESS 0x20

#endif