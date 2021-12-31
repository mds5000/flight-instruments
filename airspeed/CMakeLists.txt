cmake_minimum_required(VERSION 3.13)

add_executable(airspeed)

target_sources(airspeed PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/airspeed.c
        ${CMAKE_CURRENT_LIST_DIR}/main.c
        ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c
        )

# Make sure TinyUSB can find tusb_config.h
target_include_directories(airspeed PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        "../displays/ili9341"
        "../lvgl"
)

# In addition to pico_stdlib required for common PicoSDK functionality, add dependency on tinyusb_device
# for TinyUSB device support and tinyusb_board for the additional board support library used by the example
target_link_libraries(airspeed PUBLIC pico_stdlib tinyusb_device tinyusb_board)
target_link_libraries(airspeed PUBLIC ili9341)

pico_add_extra_outputs(airspeed)
