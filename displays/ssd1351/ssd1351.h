#pragma once

#include "gpio.h"
#include "spi.h"
#include "lvgl.h"

#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    gpio_t *dc;
    gpio_t *rst;
    spi_t *spi;
} ssd1351_t;

bool ssd1351_initialize(ssd1351_t *disp);
bool ssd1351_send_command(ssd1351_t *disp, uint8_t command);
bool ssd1351_send_data(ssd1351_t *disp, uint8_t *data, uint32_t length);
bool ssd1351_initialize_display(ssd1351_t *disp);
void ssd1351_update_display(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map);
