/** @file spi_config.h
*   @brief Created on 2021-03-10 by Ben
*/
#pragma once

/**********************************************************************
* Includes
**********************************************************************/
#include "stm32f0xx.h"
#include <stdbool.h>
/**********************************************************************
* Preprocessor Constants
**********************************************************************/

/**********************************************************************
* Typedefs
**********************************************************************/

/**
 * All clock polarities
 */
enum Polarity
{
    POL_LOW_ON_IDLE  = 0 << 1,
    POL_HIGH_ON_IDLE = 1 << 1,
};

/**
 * All clock phases
 */
enum Phase
{
    PHA_FIRST_CLK_TRANSITION  = 0 << 0,
    PHA_SECOND_CLK_TRANSITION = 1 << 0,
};

/**
 * SPI Modes (master/slave and direction)
 */
enum Mode
{
    MASTER = SPI_CR1_MSTR,
    SLAVE  = 0,
};

/**
 * The baud rate (8MHz peripheral clock)
 */
enum BaudRate
{
    BAUD_RATE_PRESCALAR_2     = 0 << 3,
    BAUD_RATE_PRESCALAR_4     = 1 << 3,
    BAUD_RATE_PRESCALAR_8     = 2 << 3,
    BAUD_RATE_PRESCALAR_16   = 3 << 3,
    BAUD_RATE_PRESCALAR_32   = 4 << 3,
    BAUD_RATE_PRESCALAR_64   = 5 << 3,
    BAUD_RATE_PRESCALAR_128  = 6 << 3,
    BAUD_RATE_PRESCALAR_256 = 7 << 3,
};

enum FirstBit{
    MSB_FIRST = 0 << 7,
    LSB_FIRST = 1 << 7,
};

enum DataSize
{
    DATA_SIZE_8Bit  = 0b0111 << 8,
    DATA_SIZE_16Bit = 0b1111 << 8,
};

enum SpiChannel
{
    SPI1_CHANNEL,
    SPI_CHANNEL_END,
};

struct SpiConfig
{
    enum SpiChannel channel;
    enum Polarity polarity;
    enum Phase phase;
    enum Mode mode;
    enum BaudRate baudRate;
    enum DataSize dataSize;
    enum FirstBit firstBit;
    bool enable_interrupts;
};

/**********************************************************************
* Function Prototypes
**********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
const struct SpiConfig* spi_get_config();
#ifdef __cplusplus
} // extern "C"
#endif