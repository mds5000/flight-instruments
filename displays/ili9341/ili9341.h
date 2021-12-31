#pragma once

#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "lvgl.h"

#include <stdbool.h>

/* 
// Vertical Orientation
#define VERT_LINES 320
#define HORZ_LINES 240
*/
// Horizontal Orientation
#define VERT_LINES 240
#define HORZ_LINES 320

typedef struct
{
    uint dc;
    uint rst;
    spi_inst_t *spi;
    uint spi_mosi;
    uint spi_clk;
    uint spi_cs;
} ili9341_t;

bool ili9341_initialize(ili9341_t *disp);
bool ili9341_send_command(ili9341_t *disp, uint8_t command);
bool ili9341_send_data(ili9341_t *disp, uint8_t *data, uint32_t length);
bool ili9341_initialize_display(ili9341_t *disp);
void ili9341_update_display(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map);
