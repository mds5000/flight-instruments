cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(flight-instruments VERSION 0.1.0)
pico_sdk_init()

add_subdirectory(lvgl)
file(GLOB_RECURSE SOURCES lvgl/src/*.c)
add_library(lvgl_embedded STATIC ${SOURCES})
target_compile_definitions(lvgl_embedded PUBLIC LV_CONF_INCLUDE_SIMPLE)
target_include_directories(lvgl_embedded SYSTEM PUBLIC lvgl lvgl/displays/st7789)

add_subdirectory(displays)
add_subdirectory(airspeed)
