/** @file i2c.h
*   @brief Created on 2021-03-11 by Ben
*/
#pragma once

/**********************************************************************
* Includes
**********************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "i2c_config.h"
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

void i2c_init();
void i2c_read(enum I2C_Channel channel, uint16_t slaveAddress, uint8_t *rxData, uint16_t dataLen, bool sendStop);
void i2c_write(enum I2C_Channel channel, uint16_t slaveAddress, const uint8_t* txData, uint16_t dataLen, bool sendStop);
void i2c_send_stop(enum I2C_Channel channel);


#ifdef __cplusplus
} // extern "C"
#endif