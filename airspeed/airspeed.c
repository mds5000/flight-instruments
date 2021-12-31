#include "lvgl.h"

static lv_obj_t* gauge;
static lv_anim_t needle_slew;
static lv_meter_indicator_t* indicator;

static void set_needle(lv_meter_indicator_t* indicator, int value) {
    lv_meter_set_indicator_value(gauge, indicator, value);
}

lv_meter_indicator_t* display_airspeed(lv_obj_t *parent) {
    const lv_color_t GREEN = lv_color_make(0x00, 0x8F, 0x00);
    const lv_color_t YELLOW = lv_color_make(0x8F, 0x8F, 0x00);
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_bg_opa(&style, LV_OPA_COVER);
    lv_style_set_bg_color(&style, lv_color_black());
    lv_style_set_text_color(&style, lv_color_white());

    gauge = lv_meter_create(parent);
    lv_obj_set_size(gauge, 255, 255);
    lv_obj_add_style(gauge, &style, 0);
    lv_obj_center(gauge);

    lv_obj_t *label1 = lv_label_create(gauge);
    lv_label_set_text(label1, "AIRSPEED");
    lv_obj_set_pos(label1, 0, 70);
    lv_obj_set_width(label1, 230);
    lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(label1, lv_palette_main(LV_PALETTE_GREY), 0);

    lv_obj_t *label2 = lv_label_create(gauge);
    lv_label_set_text(label2, "KNOTS");
    lv_obj_set_pos(label2, 0, 130);
    lv_obj_set_width(label2, 230);
    lv_obj_set_style_text_align(label2, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(label2, lv_palette_main(LV_PALETTE_GREY), 0);

    lv_meter_scale_t* scale = lv_meter_add_scale(gauge);
    lv_meter_scale_t* scale2 = lv_meter_add_scale(gauge);
    lv_meter_set_scale_ticks(gauge, scale, 41, 2, 20, lv_color_white());
    lv_meter_set_scale_ticks(gauge, scale2, 21, 2, 25, lv_color_white());
    lv_meter_set_scale_major_ticks(gauge, scale, 4, 3, 25, lv_color_white(), 20);
    lv_meter_set_scale_range(gauge, scale, 40, 200, 320, 290);
    lv_meter_set_scale_range(gauge, scale2, 40, 200, 320, 290);

    indicator = lv_meter_add_needle_line(gauge, scale, 5, lv_color_white(), 0);

    lv_meter_indicator_t * low_speed_arc = lv_meter_add_arc(gauge, scale, 10, GREEN, 0);
    lv_meter_set_indicator_start_value(gauge, low_speed_arc, 60);
    lv_meter_set_indicator_end_value(gauge, low_speed_arc, 100);

    lv_meter_indicator_t * caution_arc = lv_meter_add_arc(gauge, scale, 10, YELLOW, 0);
    lv_meter_set_indicator_start_value(gauge, caution_arc, 100);
    lv_meter_set_indicator_end_value(gauge, caution_arc, 150);

    lv_meter_indicator_t * max_speed = lv_meter_add_scale_lines(gauge, scale, lv_palette_main(LV_PALETTE_RED), lv_palette_main(LV_PALETTE_RED), false, 2);
    lv_meter_set_indicator_start_value(gauge, max_speed, 150);
    lv_meter_set_indicator_end_value(gauge, max_speed, 150);

    lv_anim_init(&needle_slew);
    lv_anim_set_exec_cb(&needle_slew, set_needle);
    lv_anim_set_var(&needle_slew, indicator);
    lv_anim_set_time(&needle_slew, 100);
    lv_anim_set_values(&needle_slew, 240, 0);
    lv_anim_start(&needle_slew);
}

void set_airspeed(int airspeed) {
    static int last_speed = 40;
    lv_anim_set_values(&needle_slew, last_speed, airspeed);
    last_speed = airspeed;
    lv_anim_start(&needle_slew);
}

/*
port to pico over USB
*/