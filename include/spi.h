/** @file spi.h
*   @brief Created on 2021-03-10 by Ben
*/
#pragma once

/**********************************************************************
* Includes
**********************************************************************/
#include "spi_config.h"

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

void spi_init();
void spi_transfer(enum SpiChannel channel, uint8_t* txBuffer, uint8_t* rxBuffer, uint8_t transactionLength);
void spi_read(enum SpiChannel channel, uint8_t* rxBuffer, uint8_t transactionLength);
void spi_write(enum SpiChannel channel, uint8_t* txBuffer, uint8_t transactionLength);

#ifdef __cplusplus
} // extern "C"
#endif