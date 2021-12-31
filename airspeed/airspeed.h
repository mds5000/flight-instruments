#pragma once

#include "lvgl.h"

lv_meter_indicator_t* display_airspeed(lv_obj_t *parent);
void set_airspeed(int airspeed);