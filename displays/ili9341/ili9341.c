#include "ili9341.h"
#include "ili9341_regs.h"

#include "hardware/timer.h"

#define SPI_CLK_FREQ 10000000
#define COLOR_DEPTH 2 /* 16-bit, 2-bytes per pixel */

bool ili9341_initialize(ili9341_t *disp)
{
    gpio_init(disp->spi_cs);
    gpio_set_dir(disp->spi_cs, GPIO_OUT);
    gpio_put(disp->spi_cs, 1);

    gpio_init(disp->dc);
    gpio_set_dir(disp->dc, GPIO_OUT);
    gpio_put(disp->dc, 0);

    gpio_init(disp->rst);
    gpio_set_dir(disp->rst, GPIO_OUT);
    gpio_put(disp->rst, 1);

    spi_init(disp->spi, SPI_CLK_FREQ);
    gpio_set_function(disp->spi_mosi, GPIO_FUNC_SPI);
    gpio_set_function(disp->spi_clk, GPIO_FUNC_SPI);

    return true;
}

static bool ili9341_send_instruction(ili9341_t *disp, bool is_data, uint8_t *data, uint32_t length)
{
    gpio_put(disp->dc, is_data);
    gpio_put(disp->spi_cs, 0);
    asm volatile("nop \n nop \n nop");
    spi_write_blocking(disp->spi, data, length);
    asm volatile("nop \n nop \n nop");
    gpio_put(disp->spi_cs, 1);

    return true;
}

bool ili9341_send_command(ili9341_t *disp, uint8_t command)
{
    return ili9341_send_instruction(disp, false, &command, sizeof(command));
}

bool ili9341_send_data(ili9341_t *disp, uint8_t *data, uint32_t length)
{
    return ili9341_send_instruction(disp, true, data, length);
}

bool ili9341_hard_reset(ili9341_t *disp)
{
    gpio_put(disp->rst, 0);
    busy_wait_us(1000);
    gpio_put(disp->rst, 1);
    busy_wait_us(5000);

    return true;
}

bool ili9341_initialize_display(ili9341_t *disp)
{
    bool ok = true;
    ok &= ili9341_hard_reset(disp);
    ok &= ili9341_send_command(disp, SWRESET);
    busy_wait_us(120000);
    ok &= ili9341_send_command(disp, SLP_OUT);
    busy_wait_us(5000);

    ok &= ili9341_send_command(disp, PIX_FMT);
    uint8_t pixel_format = 0x55;
    ok &= ili9341_send_data(disp, &pixel_format, 1);


    ok &= ili9341_send_command(disp, VSCRDEF);
    uint8_t vscroll_def[6] = {0, 0, 0x14, 0, 0, 0}; // 0, 320, 0
    ok &= ili9341_send_data(disp, vscroll_def, 6);

    ok &= ili9341_send_command(disp, MADCTL);
    // uint8_t mem_def[1] = {0x88}; // Vertical, BRG Color
    uint8_t mem_def[1] = {0x28}; // Horizontal, BRG Color
    ok &= ili9341_send_data(disp, &mem_def, 1);

    ok &= ili9341_send_command(disp, NORON);
    ok &= ili9341_send_command(disp, DISPON);
    return ok;
}

void ili9341_update_display(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
    ili9341_t *disp = (ili9341_t *)(drv->user_data);

    uint8_t col_data[4] = {
        (area->x1 >> 8) & 0xFF,
        area->x1 & 0xFF,
        (area->x2 >> 8) & 0xFF,
        area->x2 & 0xFF};
    ili9341_send_command(disp, CASET);
    ili9341_send_data(disp, col_data, sizeof(col_data));

    uint8_t row_data[4] = {
        (area->y1 >> 8) & 0xFF,
        area->y1 & 0xFF,
        (area->y2 >> 8) & 0xFF,
        area->y2 & 0xFF};
    ili9341_send_command(disp, PASET);
    ili9341_send_data(disp, row_data, sizeof(row_data));

    uint32_t size = lv_area_get_width(area) * lv_area_get_height(area) * COLOR_DEPTH;
    for (int i=0; i<(size/COLOR_DEPTH); i++) {
        lv_color16_t c = *(color_map + i);
        c.full = __builtin_bswap16(c.full);
        *(color_map + i) = c;
    }

    ili9341_send_command(disp, RAMWR);
    ili9341_send_data(disp, (uint8_t *)(color_map), size);

    lv_disp_flush_ready(drv);
}