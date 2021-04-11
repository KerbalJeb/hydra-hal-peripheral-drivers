/** @file i2c.c
*   @brief Created on 2021-03-11 by Ben
*/
/**********************************************************************
* Includes
**********************************************************************/
#include "i2c.h"
#include "i2c_config.h"
#include "stm32f0xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/**********************************************************************
* Module Preprocessor Constants
**********************************************************************/

/**********************************************************************
* Module Preprocessor Macros
**********************************************************************/

/**********************************************************************
* Module Typedefs
**********************************************************************/

/*********************************************************************
* Module Variable Definitions
**********************************************************************/

static I2C_TypeDef * const I2C_Channels[] =
{
    I2C1,
};

static const uint32_t I2C_ClockEnableMasks[] =
{
    RCC_APB1ENR_I2C1EN
};

static const IRQn_Type IRQn_Vectors[] =
{
    I2C1_IRQn,
};

static StaticSemaphore_t MutexBuffers[1];
static SemaphoreHandle_t  MutexHandles[1];

/* Todo support multiple i2c peripherals */
static TaskHandle_t BlockedI2C_Task = NULL;
static uint8_t * dataBuffer;
static bool transmitting;

/**********************************************************************
* Function Prototypes
**********************************************************************/

/**********************************************************************
* Function Definitions
**********************************************************************/

void i2c_init()
{
    const struct I2C_Config* config = i2c_get_config();

    while (config->channel != I2C_CHANNEL_END)
    {
        MutexHandles[config->channel] = xSemaphoreCreateMutexStatic(&MutexBuffers[config->channel]);
        RCC->APB1ENR |= I2C_ClockEnableMasks[config->channel];

        I2C_TypeDef * i2c = I2C_Channels[config->channel];
        /* Configure clock */
        switch (config->speed)
        {
            case I2C_SPEED_100kHz:
                i2c->TIMINGR = 0x20000A0D;
                break;
            case I2C_SPEED_400kHz:
                i2c->TIMINGR = 0x0010020B;
                break;
            default:
                break;
        }

//        RCC->CFGR3 |= RCC_CFGR3_I2C1SW_SYSCLK;

        /* Configure I2C */
        i2c->CR2 |= config->addressingMode;
        NVIC_EnableIRQ(IRQn_Vectors[config->channel]);
        config++;
    }
}

/* Todo: Error handling */
void i2c_write(enum I2C_Channel channel, uint16_t slaveAddress, const uint8_t* txData, uint16_t dataLen, bool sendStop)
{
    xSemaphoreTake(MutexHandles[channel], portMAX_DELAY);
    BlockedI2C_Task = xTaskGetCurrentTaskHandle();
    dataBuffer = (uint8_t *)txData;
    transmitting = true;

    I2C_TypeDef * i2c = I2C_Channels[channel];

    /* Set slave address */
    uint32_t cr2Mask = I2C_CR2_SADD_Msk | I2C_CR2_AUTOEND_Msk | I2C_CR2_RD_WRN_Msk | I2C_CR2_START_Msk | I2C_CR2_NBYTES_Msk;
    uint32_t cr2Value = (slaveAddress << 1) | (sendStop << I2C_CR2_AUTOEND_Pos) | I2C_CR2_START;

    if (dataLen <= 255)
    {
        cr2Value |= dataLen << I2C_CR2_NBYTES_Pos;
    }
    else
    {
        while (1){}
    }

    uint32_t cr2NewValue = i2c->CR2;

    /* Set the 1 bits */
    cr2NewValue |= cr2Value & cr2Mask;
    /* Clear the 0 bits */
    cr2NewValue &= cr2Value | ~cr2Mask;
    if (sendStop)
    {
        i2c->CR1 = I2C_CR1_STOPIE | I2C_CR1_PE | I2C_CR1_TXIE;

    } else
    {
        i2c->CR1 = I2C_CR1_TCIE | I2C_CR1_PE | I2C_CR1_TXIE;
    }
    i2c->CR2 = cr2NewValue;

    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    xSemaphoreGive(MutexHandles[channel]);
}

void i2c_send_stop(enum I2C_Channel channel)
{
    xSemaphoreTake(MutexHandles[channel], portMAX_DELAY);
    I2C_TypeDef * I2C = I2C_Channels[channel];
    I2C->CR2 |= I2C_CR2_STOP;
    xSemaphoreGive(MutexHandles[channel]);
}

void i2c_read(enum I2C_Channel channel, uint16_t slaveAddress, uint8_t *rxData, uint16_t dataLen, bool sendStop)
{
    xSemaphoreTake(MutexHandles[channel], portMAX_DELAY);

    BlockedI2C_Task = xTaskGetCurrentTaskHandle();
    dataBuffer = rxData;
    transmitting = false;

    I2C_TypeDef * i2c = I2C_Channels[channel];

    /* Set slave address */
    uint32_t cr2Mask = I2C_CR2_SADD_Msk | I2C_CR2_AUTOEND_Msk | I2C_CR2_RD_WRN_Msk | I2C_CR2_START_Msk | I2C_CR2_NBYTES_Msk;
    uint32_t cr2Value = (slaveAddress << 1) | (sendStop << I2C_CR2_AUTOEND_Pos) | I2C_CR2_START | I2C_CR2_RD_WRN;

    if (dataLen <= 255)
    {
        cr2Value |= dataLen << I2C_CR2_NBYTES_Pos;
    }
    else
    {
        while (1){}
    }

    uint32_t cr2NewValue = i2c->CR2;

    /* Set the 1 bits */
    cr2NewValue |= cr2Value & cr2Mask;
    /* Clear the 0 bits */
    cr2NewValue &= cr2Value | ~cr2Mask;
    if (sendStop)
    {
        i2c->CR1 = I2C_CR1_STOPIE | I2C_CR1_PE | I2C_CR1_RXIE;

    } else
    {
        i2c->CR1 = I2C_CR1_TCIE | I2C_CR1_PE | I2C_CR1_RXIE;
    }
    i2c->CR2 = cr2NewValue;

    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    xSemaphoreGive(MutexHandles[channel]);
}

void I2C1_IRQHandler()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (I2C1->ISR & (I2C_ISR_STOPF | I2C_ISR_TC))
    {
        I2C1->ICR =   I2C_ICR_STOPCF;
        I2C1->CR1 = 0;
        vTaskNotifyGiveFromISR(BlockedI2C_Task, &xHigherPriorityTaskWoken);
    }
    else
    {
        if (I2C1->ISR & I2C_ISR_TXE && transmitting)
        {
            I2C1->TXDR = *dataBuffer;
            dataBuffer++;
        }

        if (I2C1->ISR & I2C_ISR_RXNE && !transmitting)
        {
            *dataBuffer = I2C1->RXDR;
            dataBuffer++;
        }
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}