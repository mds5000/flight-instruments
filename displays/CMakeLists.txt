
add_library(st7789 INTERFACE)
target_include_directories(st7789 INTERFACE
    "displays/st7789/"
    "lvgl/"
)
target_sources(st7789 INTERFACE st7789/st7789.c)
target_link_libraries(st7789 INTERFACE lvgl_embedded)
target_link_libraries(st7789 INTERFACE hardware_spi pico_stdlib)


add_library(ili9341 INTERFACE)
target_include_directories(ili9341 INTERFACE
    "displays/ili9341/"
    "lvgl/"
)
target_sources(ili9341 INTERFACE ili9341/ili9341.c)
target_link_libraries(ili9341 INTERFACE lvgl_embedded)
target_link_libraries(ili9341 INTERFACE hardware_spi pico_stdlib)