#include "ssd1351.h"
#include "ssd1351_regs.h"

#include <stdio.h>
#include <unistd.h>

#define SPI_CLK_FREQ 1600000
#define COLOR_DEPTH 2 /* 16-bit, 2-bytes per pixel */

bool ssd1351_initialize(ssd1351_t *disp)
{
    disp->dc = gpio_new();
    disp->rst = gpio_new();
    disp->spi = spi_new();

    if (gpio_open(disp->dc, "/dev/gpiochip0", 25, GPIO_DIR_OUT) < 0)
    {
        fprintf(stderr, "gpio_open(): %s\n", gpio_errmsg(disp->dc));
        return false;
    }

    if (gpio_open(disp->rst, "/dev/gpiochip0", 24, GPIO_DIR_OUT) < 0)
    {
        fprintf(stderr, "gpio_open(): %s\n", gpio_errmsg(disp->rst));
        return false;
    }

    if (spi_open(disp->spi, "/dev/spidev0.0", 0, SPI_CLK_FREQ) < 0)
    {
        fprintf(stderr, "spi_open(): %s\n", spi_errmsg(disp->spi));
        return false;
    }

    return true;
}

static bool ssd1351_send_instruction(ssd1351_t *disp, bool is_data, uint8_t *data, uint32_t length)
{
    if (gpio_write(disp->dc, is_data) < 0)
    {
        fprintf(stderr, "gpio_write(): %s\n", gpio_errmsg(disp->dc));
        return false;
    }

    if (spi_transfer(disp->spi, data, NULL, length) < 0)
    {
        fprintf(stderr, "spi_transfer(): %s\n", spi_errmsg(disp->spi));
        return false;
    }

    return true;
}

bool ssd1351_send_command(ssd1351_t *disp, uint8_t command)
{
    return ssd1351_send_instruction(disp, false, &command, sizeof(command));
}

bool ssd1351_send_data(ssd1351_t *disp, uint8_t *data, uint32_t length)
{
    return ssd1351_send_instruction(disp, true, data, length);
}

bool ssd1351_hard_reset(ssd1351_t *disp)
{
    if (gpio_write(disp->rst, 0) < 0)
    {
        fprintf(stderr, "gpio_write(): %s\n", gpio_errmsg(disp->rst));
        return false;
    }
    usleep(100000);
    if (gpio_write(disp->rst, 1) < 0)
    {
        fprintf(stderr, "gpio_write(): %s\n", gpio_errmsg(disp->rst));
        return false;
    }
    usleep(100000);

    return true;
}

bool ssd1351_initialize_display(ssd1351_t *disp)
{
    uint8_t command[3] = {0, 0, 0};
    bool ok = true;
    ok &= ssd1351_hard_reset(disp);
    usleep(120000);

    ssd1351_send_command(disp, COMMAND_LOCK);
    command[0] = 0x12;
    ssd1351_send_data(disp, command, 1);

    ssd1351_send_command(disp, COMMAND_LOCK);
    command[0] = 0xB1;
    ssd1351_send_data(disp, command, 1);

    ssd1351_send_command(disp, DISPLAY_OFF);

    ssd1351_send_command(disp, CLOCK_DIV);
    command[0] = 0xF1;
    ssd1351_send_data(disp, command, 1);

    ssd1351_send_command(disp, MUX_RATIO);
    command[0] = 0x7F;
    ssd1351_send_data(disp, command, 1);

    usleep(100000);

    ssd1351_send_command(disp, SET_REMAP);
    command[0] = 0x20;
    ssd1351_send_data(disp, command, 1);

    ssd1351_send_command(disp, SET_COL_ADDR);
    command[0] = 0x00;
    command[1] = 0x7F;
    ssd1351_send_data(disp, command, 2);

    ssd1351_send_command(disp, SET_ROW_ADDR);
    command[0] = 0x00;
    command[1] = 0x7F;
    ssd1351_send_data(disp, command, 2);

    ssd1351_send_command(disp, DISP_START_LN);
    command[0] = 0x00;
    ssd1351_send_data(disp, command, 1);

    ssd1351_send_command(disp, SET_GPIO);
    command[0] = 0x00;
    ssd1351_send_data(disp, command, 1);

    ssd1351_send_command(disp, FUNC_SELECT);
    command[0] = 0x01;
    ssd1351_send_data(disp, command, 1);

    ssd1351_send_command(disp, PRECHARGE);
    command[0] = 0x32;
    ssd1351_send_data(disp, command, 1);
    ssd1351_send_command(disp, PRECHARGE_2);
    command[0] = 0x01;
    ssd1351_send_data(disp, command, 1);

    ssd1351_send_command(disp, SET_V_COMH);
    command[0] = 0x05;
    ssd1351_send_data(disp, command, 1);

    ssd1351_send_command(disp, DISP_NORMAL);

    ssd1351_send_command(disp, CONTRAST_ABC);
    command[0] = 0xC8;
    command[1] = 0x8F;
    command[2] = 0xC8;
    ssd1351_send_data(disp, command, 3);

    ssd1351_send_command(disp, CONTRAST_MASTER);
    command[0] = 0x0F;
    ssd1351_send_data(disp, command, 1);

    ssd1351_send_command(disp, SET_VSL);
    command[0] = 0xA0;
    command[1] = 0xB5;
    command[2] = 0x55;
    ssd1351_send_data(disp, command, 3);

    ssd1351_send_command(disp, DISPLAY_ON);

    return ok;
}

void ssd1351_update_display(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
    ssd1351_t *disp = (ssd1351_t *)(drv->user_data);

    uint8_t col_data[2] = { area->x1 & 0xFF, area->x2 & 0xFF};
    ssd1351_send_command(disp, SET_COL_ADDR);
    ssd1351_send_data(disp, col_data, sizeof(col_data));

    uint8_t row_data[2] = { area->y1 & 0xFF, area->y2 & 0xFF};
    ssd1351_send_command(disp, SET_ROW_ADDR);
    ssd1351_send_data(disp, row_data, sizeof(row_data));

    uint32_t size = lv_area_get_width(area) * lv_area_get_height(area) * COLOR_DEPTH;

    ssd1351_send_command(disp, WRITE_RAM);
    ssd1351_send_data(disp, (uint8_t *)(color_map), size);

    lv_disp_flush_ready(drv);
}