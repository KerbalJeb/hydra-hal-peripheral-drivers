/** @file gpio_config.h
*   @brief Created on 2021-03-10 by Ben
*/
#pragma once

/**********************************************************************
* Includes
**********************************************************************/

/**********************************************************************
* Preprocessor Constants
**********************************************************************/
#define PINS_PER_PORT 16
#define SS_RELAY_PIN PIN_B0
#define SPI_SCK_PIN PIN_A5
#define SPI_MISO_PIN PIN_A6
#define SPI_CS_PIN PIN_A7
#define I2C_SDA PIN_A10
#define I2C_SCK PIN_A9

/**********************************************************************
* Typedefs
**********************************************************************/

/**
 * Defines the possible states for a digital output pin.
 */
enum GpioPinState
{
    GPIO_LOW, /** Ground digital state*/
    GPIO_HIGH, /** Power digital state */
};

/**
 * Defines all of the pins on the MCU
 */
enum GpioChannel
{
    PIN_A0  =  0 + PINS_PER_PORT * 0,
    PIN_A1  =  1 + PINS_PER_PORT * 0,
    PIN_A2  =  2 + PINS_PER_PORT * 0,
    PIN_A3  =  3 + PINS_PER_PORT * 0,
    PIN_A4  =  4 + PINS_PER_PORT * 0,
    PIN_A5  =  5 + PINS_PER_PORT * 0,
    PIN_A6  =  6 + PINS_PER_PORT * 0,
    PIN_A7  =  7 + PINS_PER_PORT * 0,
    PIN_A8  =  8 + PINS_PER_PORT * 0,
    PIN_A9  =  9 + PINS_PER_PORT * 0,
    PIN_A10 = 10 + PINS_PER_PORT * 0,
    PIN_A11 = 11 + PINS_PER_PORT * 0,
    PIN_A12 = 12 + PINS_PER_PORT * 0,
    PIN_A13 = 13 + PINS_PER_PORT * 0,
    PIN_A14 = 14 + PINS_PER_PORT * 0,
    PIN_A15 = 15 + PINS_PER_PORT * 0,
    PIN_B0  =  0 + PINS_PER_PORT * 1,
    PIN_B1  =  1 + PINS_PER_PORT * 1,
    PIN_B2  =  2 + PINS_PER_PORT * 1,
    PIN_B3  =  3 + PINS_PER_PORT * 1,
    PIN_B4  =  4 + PINS_PER_PORT * 1,
    PIN_B5  =  5 + PINS_PER_PORT * 1,
    PIN_B6  =  6 + PINS_PER_PORT * 1,
    PIN_B7  =  7 + PINS_PER_PORT * 1,
    PIN_B8  =  8 + PINS_PER_PORT * 1,
    GPIO_PIN_END
};

/**
 * Defines all possible GPIO multiplexing values/modes.
 */
enum GpioMode
{
    GPIO_INPUT   = 0b00, /** Digital Input */
    GPIO_OUTPUT  = 0b01, /** Digital Output */
    GPIO_AF_MODE = 0b10, /** Alternate Function Mode */
    GPIO_ANALOG  = 0b11, /** For ADC/DAC */
};

/**
 * Defines all possible GPIO pull up/down states
 */
enum GpioPullMode
{
    GPIO_PULL_NONE = 0b00, /** Pull up/down resistor disabled */
    GPIO_PULL_UP   = 0b01, /** Pull up enabled */
    GPIO_PULL_DOWN = 0b10, /** Pull down enabled */
};

/**
 * Defines all alternate function modes
 */
enum AlternateFunctionMode
{
    ALTERNATE_FUNCTION_A0,
    ALTERNATE_FUNCTION_A1,
    ALTERNATE_FUNCTION_A2,
    ALTERNATE_FUNCTION_A3,
    ALTERNATE_FUNCTION_A4,
    ALTERNATE_FUNCTION_A5,
    ALTERNATE_FUNCTION_A6,
    ALTERNATE_FUNCTION_A7,
};

enum OutputMode
{
    PUSH_PULL_OUTPUT=0,
    OPEN_DRAIN_OUTPUT=1
};

/**
 * Defines the configuration for a GPIO pin
 */
struct GpioConfig
{
    enum GpioChannel channel;
    enum GpioMode mode;
    enum GpioPullMode pullMode;
    enum AlternateFunctionMode alternateFunctionMode;
    enum GpioPinState defaultState;
    enum OutputMode outputMode;
};

/**********************************************************************
* Function Prototypes
**********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
const struct GpioConfig * gpio_get_config(void);
#ifdef __cplusplus
} // extern "C"
#endif