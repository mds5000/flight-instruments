#pragma once

#define NOP 0x00
#define SWRESET 0x01
#define RDDID 0x04
#define RDDST 0x09

#define RDDPM 0x0A      // Read display power mode
#define RDD_MADCTL 0x0B // Read display MADCTL
#define RDD_COLMOD 0x0C // Read display pixel format
#define RDDIM 0x0D      // Read display image mode
#define RDDSM 0x0E      // Read display signal mode
#define RDDSR 0x0F      // Read display self-diagnostic result (ST7789V)

#define SLPIN 0x10
#define SLPOUT 0x11
#define PTLON 0x12
#define NORON 0x13

#define INVOFF 0x20
#define INVON 0x21
#define GAMSET 0x26 // Gamma set
#define DISPOFF 0x28
#define DISPON 0x29
#define CASET 0x2A
#define RASET 0x2B
#define RAMWR 0x2C
#define RGBSET 0x2D // Color setting for 4096, 64K and 262K colors
#define RAMRD 0x2E

#define PTLAR 0x30
#define VSCRDEF 0x33 // Vertical scrolling definition (ST7789V)
#define TEOFF 0x34   // Tearing effect line off
#define TEON 0x35    // Tearing effect line on
#define MADCTL 0x36  // Memory data access control
#define IDMOFF 0x38  // Idle mode off
#define IDMON 0x39   // Idle mode on
#define RAMWRC 0x3C  // Memory write continue (ST7789V)
#define RAMRDC 0x3E  // Memory read continue (ST7789V)
#define COLMOD 0x3A

#define RAMCTRL 0xB0   // RAM control
#define RGBCTRL 0xB1   // RGB control
#define PORCTRL 0xB2   // Porch control
#define FRCTRL1 0xB3   // Frame rate control
#define PARCTRL 0xB5   // Partial mode control
#define GCTRL 0xB7     // Gate control
#define GTADJ 0xB8     // Gate on timing adjustment
#define DGMEN 0xBA     // Digital gamma enable
#define VCOMS 0xBB     // VCOMS setting
#define LCMCTRL 0xC0   // LCM control
#define IDSET 0xC1     // ID setting
#define VDVVRHEN 0xC2  // VDV and VRH command enable
#define VRHS 0xC3      // VRH set
#define VDVSET 0xC4    // VDV setting
#define VCMOFSET 0xC5  // VCOMS offset set
#define FRCTR2 0xC6    // FR Control 2
#define CABCCTRL 0xC7  // CABC control
#define REGSEL1 0xC8   // Register value section 1
#define REGSEL2 0xCA   // Register value section 2
#define PWMFRSEL 0xCC  // PWM frequency selection
#define PWCTRL1 0xD0   // Power control 1
#define VAPVANEN 0xD2  // Enable VAP/VAN signal output
#define CMD2EN 0xDF    // Command 2 enable
#define PVGAMCTRL 0xE0 // Positive voltage gamma control
#define NVGAMCTRL 0xE1 // Negative voltage gamma control
#define DGMLUTR 0xE2   // Digital gamma look-up table for red
#define DGMLUTB 0xE3   // Digital gamma look-up table for blue
#define GATECTRL 0xE4  // Gate control
#define SPI2EN 0xE7    // SPI2 enable
#define PWCTRL2 0xE8   // Power control 2
#define EQCTRL 0xE9    // Equalize time control
#define PROMCTRL 0xEC  // Program control
#define PROMEN 0xFA    // Program mode enable
#define NVMSET 0xFC    // NVM setting