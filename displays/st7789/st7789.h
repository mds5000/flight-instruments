#pragma once

#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "lvgl.h"

#include <stdbool.h>
#include "st7789_regs.h"

#define VERT_LINES 320
#define HORZ_LINES 240

typedef struct
{
    uint dc;
    uint rst;
    spi_inst_t *spi;
    uint spi_mosi;
    uint spi_clk;
    uint spi_cs;
} st7789_t;

bool st7789_initialize(st7789_t *disp);
bool st7789_send_command(st7789_t *disp, uint8_t command);
bool st7789_send_data(st7789_t *disp, uint8_t *data, uint32_t length);
bool st7789_initialize_display(st7789_t *disp);
void st7789_update_display(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map);