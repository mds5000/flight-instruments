#include "st7789.h"
#include "st7789_regs.h"

#include "hardware/timer.h"

#define SPI_CLK_FREQ 10000000
#define COLOR_DEPTH 2 /* 16-bit, 2-bytes per pixel */

bool st7789_initialize(st7789_t *disp)
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

static bool st7789_send_instruction(st7789_t *disp, bool is_data, uint8_t *data, uint32_t length)
{
    gpio_put(disp->dc, is_data);
    gpio_put(disp->spi_cs, 0);
    asm volatile("nop \n nop \n nop");
    spi_write_blocking(disp->spi, data, length);
    asm volatile("nop \n nop \n nop");
    gpio_put(disp->spi_cs, 1);

    return true;
}

bool st7789_send_command(st7789_t *disp, uint8_t command)
{
    return st7789_send_instruction(disp, false, &command, sizeof(command));
}

bool st7789_send_data(st7789_t *disp, uint8_t *data, uint32_t length)
{
    return st7789_send_instruction(disp, true, data, length);
}

bool st7789_hard_reset(st7789_t *disp)
{
    gpio_put(disp->rst, 0);
    busy_wait_us(100000);
    gpio_put(disp->rst, 1);
    busy_wait_us(100000);

    return true;
}

bool st7789_initialize_display(st7789_t *disp)
{
    bool ok = true;
    ok &= st7789_hard_reset(disp);
    ok &= st7789_send_command(disp, SWRESET);
    busy_wait_us(120000);

    ok &= st7789_send_command(disp, SLPOUT);
    busy_wait_us(5000);
    //ok &= st7789_send_command(disp, INVOFF);

    ok &= st7789_send_command(disp, VSCRDEF);
    uint8_t vscroll_def[6] = {0, 0, 0x14, 0, 0, 0}; // 0, 320, 0
    ok &= st7789_send_data(disp, vscroll_def, 6);

    ok &= st7789_send_command(disp, MADCTL);
    uint8_t mem_def = 0x00;
    ok &= st7789_send_data(disp, &mem_def, 1);

    ok &= st7789_send_command(disp, COLMOD);
    uint8_t color_def = 0x55;
    ok &= st7789_send_data(disp, &color_def, 1);

    ok &= st7789_send_command(disp, NORON);
    ok &= st7789_send_command(disp, DISPON);
    return ok;
}

void st7789_update_display(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
    st7789_t *disp = (st7789_t *)(drv->user_data);

    uint8_t col_data[4] = {
        (area->x1 >> 8) & 0xFF,
        area->x1 & 0xFF,
        (area->x2 >> 8) & 0xFF,
        area->x2 & 0xFF};
    st7789_send_command(disp, CASET);
    st7789_send_data(disp, col_data, sizeof(col_data));

    uint8_t row_data[4] = {
        (area->y1 >> 8) & 0xFF,
        area->y1 & 0xFF,
        (area->y2 >> 8) & 0xFF,
        area->y2 & 0xFF};
    st7789_send_command(disp, RASET);
    st7789_send_data(disp, row_data, sizeof(row_data));

    uint32_t size = lv_area_get_width(area) * lv_area_get_height(area) * COLOR_DEPTH;
    for (int i=0; i<(size/COLOR_DEPTH); i++) {
        lv_color16_t c = *(color_map + i);
        c.full = __builtin_bswap16(c.full);
        *(color_map + i) = c;
    }

    st7789_send_command(disp, RAMWR);
    st7789_send_data(disp, (uint8_t *)(color_map), size);

    lv_disp_flush_ready(drv);
}