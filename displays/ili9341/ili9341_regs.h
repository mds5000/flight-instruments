#pragma once

#define NOP 		0x00
#define SWRESET 	0x01
#define RD_ID 		0x04 // Read display ID
#define RD_STATUS	0x09 // Read display status

#define RDD_PM 		0x0A // Read display power mode
#define RDD_MADCTL 	0x0B // Read display MADCTL
#define RDD_PFMT 	0x0C // Read display pixel format
#define RDD_IM 		0x0D // Read display image mode
#define RDD_SM		0x0E // Read display signal mode
#define RDD_SR	 	0x0F // Read display self-diagnostic result

#define SLP_IN 		0x10 // Enter Sleep Mode
#define SLP_OUT 	0x11 // Exit Sleep Mode
#define PTLON 		0x12 // Partial Mode On
#define NORON 		0x13 // Normal Mode On
#define INVOFF 		0x20 // Display Inversion Off
#define INVON 		0x21 // Display Inversion On
#define GAMMA_SET 	0x26 // Gamma set
#define DISPOFF 	0x28 // Display Off
#define DISPON 		0x29 // Display Off
#define CASET 		0x2A // Column Address Set
#define PASET 		0x2B // Page Address Set
#define RAMWR 		0x2C // Memory Write
#define COLOR_SET	0x2D // Color Set
#define RAMRD 		0x2E

#define PARTIAL 	0x30 // Partial Area
#define VSCRDEF 	0x33 // Vertical scrolling definition
#define TEOFF 		0x34 // Tearing effect line off
#define TEON 		0x35 // Tearing effect line on
#define MADCTL 		0x36 // Memory access control
#define IDMOFF 		0x38 // Idle mode off
#define IDMON 		0x39 // Idle mode on
#define PIX_FMT 	0x3A // Pixel Format Set
#define RAMWRC 		0x3C // Memory write continue
#define RAMRDC 		0x3E // Memory read continue
#define STSCNL		0x44 // Set Tear Scanline
#define GTSCNL		0x45 // Get Scanline	
#define BRIGHTNESS	0x51 // Write Display Brightness

