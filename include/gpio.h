/** @file gpio.h
*   @brief Created on 2021-03-10 by Ben
*/
#pragma once

/**********************************************************************
* Includes
**********************************************************************/
#include <stdint.h>
#include "gpio_config.h"
/**********************************************************************
* Preprocessor Constants
**********************************************************************/

/**********************************************************************
* Typedefs
**********************************************************************/

/**********************************************************************
* Function Prototypes
**********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
void gpio_init();
enum GpioPinState gpio_read(enum GpioChannel channel);
void gpio_write(enum GpioChannel channel, enum GpioPinState state);
void gpio_toggle(enum GpioChannel channel);
void gpio_register_write(uint32_t address, uint32_t value);
uint32_t gpio_register_read(uint32_t address);
#ifdef __cplusplus
} // extern "C"
#endif