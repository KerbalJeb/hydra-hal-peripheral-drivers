/** @file spi.c
*   @brief Created on 2021-03-10 by Ben
*/
/**********************************************************************
* Includes
**********************************************************************/
#include "spi.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"

/**********************************************************************
* Module Preprocessor Constants
**********************************************************************/
#define SPI_QUEUE_LENGTH 4
#define SPI_QUEUE_ITEM_SIZE 1


/**********************************************************************
* Module Preprocessor Macros
**********************************************************************/

/**********************************************************************
* Module Typedefs
**********************************************************************/

/*********************************************************************
* Module Variable Definitions
**********************************************************************/

SPI_TypeDef* SpiChannels[] =
{
    SPI1
};

IRQn_Type SpiIRQs[] =
{
    SPI1_IRQn,
};

uint32_t SpiClockEnableMasks[] =
{
    RCC_APB2ENR_SPI1EN,
};

StaticSemaphore_t xSpiSemaphoreBuffer;
SemaphoreHandle_t xSpiSemaphore;


QueueHandle_t SpiDataQueueHandle;
StaticQueue_t SpiDataQueue;
uint8_t SpiQueueStorage[SPI_QUEUE_LENGTH*SPI_QUEUE_ITEM_SIZE];


/**********************************************************************
* Function Prototypes
**********************************************************************/

/**********************************************************************
* Function Definitions
**********************************************************************/

void spi_init()
{
    const struct SpiConfig* config = spi_get_config();
    xSpiSemaphore = xSemaphoreCreateMutexStatic(&xSpiSemaphoreBuffer);
    SpiDataQueueHandle = xQueueCreateStatic(SPI_QUEUE_LENGTH, SPI_QUEUE_ITEM_SIZE, SpiQueueStorage, &SpiDataQueue);

    while (config->channel != SPI_CHANNEL_END)
    {
        RCC->APB2ENR |= SpiClockEnableMasks[config->channel];

        SPI_TypeDef * spi = SpiChannels[config->channel];
        spi->CR1 =(uint16_t)config->firstBit |
                            config->mode     |
                            config->phase    |
                            config->polarity |
                            config->baudRate |
                            SPI_CR1_SSM | SPI_CR1_SSI;

        spi->CR2 = config->dataSize;

        if (config->enable_interrupts)
        {
            if (config->dataSize == DATA_SIZE_8Bit)
            {
                spi->CR2 |= SPI_CR2_FRXTH;
            }
            spi->CR2 |= SPI_CR2_RXNEIE;
            __NVIC_EnableIRQ(SpiIRQs[config->channel]);
        }
        config++;
    }
}

void spi_read(enum SpiChannel channel, uint8_t* rxBuffer, uint8_t transactionLength)
{
    /* Make sure only one task is using spi at once */
    xSemaphoreTake(xSpiSemaphore, portMAX_DELAY);

    /* Get rid of any incoming data */
    xQueueReset(SpiDataQueueHandle);
    SPI_TypeDef * spi = SpiChannels[channel];

    for (int i = 0; i < transactionLength; ++i)
    {
        /* Send next packet */
        spi->CR1 |= SPI_CR1_RXONLY;
        spi->CR1 |= SPI_CR1_SPE;

        /* Wait for transmission to finish */
        if (xQueueReceive(SpiDataQueueHandle, &rxBuffer[i], pdMS_TO_TICKS(100)) == pdFALSE)
        {
            while (true){}
        }
    }
    xSemaphoreGive(xSpiSemaphore);
}

void SPI1_IRQHandler()
{
    SPI1->CR1 &= ~SPI_CR1_SPE;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if (SPI1->SR & SPI_SR_RXNE)
    {
        uint16_t value = SPI1->DR & 0xff;
        SPI1->CR1 &= ~SPI_CR1_RXONLY;
        xQueueSendFromISR(SpiDataQueueHandle, &value, &xHigherPriorityTaskWoken);
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}