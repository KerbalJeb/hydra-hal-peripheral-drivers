/** @file i2c_config.h
*   @brief Created on 2021-03-11 by Ben
*/
#pragma once

/**********************************************************************
* Includes
**********************************************************************/
#include "stm32f0xx.h"
/**********************************************************************
* Preprocessor Constants
**********************************************************************/

/**********************************************************************
* Typedefs
**********************************************************************/
enum AddressingMode{
    ADDRESS_7BIT = 0,
    ADDRESS_10BIT = I2C_CR2_ADD10,
};

enum Speed{
    I2C_SPEED_100kHz,
    I2C_SPEED_400kHz,
    I2C_SPEED_1MHz,
};

enum I2C_Channel{
    I2C1_CHANNEL,
    I2C_CHANNEL_END,
};

struct I2C_Config{
    enum I2C_Channel channel;
    enum Speed speed;
    enum AddressingMode addressingMode;
};

/**********************************************************************
* Function Prototypes
**********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
const struct I2C_Config* i2c_get_config();

#ifdef __cplusplus
} // extern "C"
#endif