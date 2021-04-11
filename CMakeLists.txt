add_library(peripheral_drivers
    ${CMAKE_CURRENT_SOURCE_DIR}/src/flash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gpio_config.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/i2c.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/i2c_config.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/spi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/spi_config.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/flash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/timer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/timer_config.c
)

target_include_directories(peripheral_drivers
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc)

target_link_libraries(peripheral_drivers PRIVATE DeviceCMSIS FreeRTOS)

