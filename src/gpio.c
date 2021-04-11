/** @file gpio.c
*   @brief Created on 2021-03-10 by Ben
*/
/**********************************************************************
* Includes
**********************************************************************/
#include "gpio.h"
#include "gpio_config.h"
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
static GPIO_TypeDef* GpioPorts[] =
{
    GPIOA,
    GPIOB,
};

static uint32_t GpioClockEnableBitMasks[] =
{
    RCC_AHBENR_GPIOAEN,
    RCC_AHBENR_GPIOBEN,
};

static StaticSemaphore_t MutexBuffers[2];
static SemaphoreHandle_t GPIOMutexHandles[2];
/**********************************************************************
* Function Prototypes
**********************************************************************/

/**********************************************************************
* Function Definitions
**********************************************************************/
void gpio_init()
{
    const struct GpioConfig* currentConfig = gpio_get_config();

    while (currentConfig->channel != GPIO_PIN_END)
    {
        uint32_t port_id = currentConfig->channel / 16;
        uint32_t pin_id  = currentConfig->channel % 16;
        GPIO_TypeDef* port = GpioPorts[port_id];
        GPIOMutexHandles[port_id] = xSemaphoreCreateMutexStatic(&MutexBuffers[port_id]);


        RCC->AHBENR |= GpioClockEnableBitMasks[port_id];
        port->MODER |= currentConfig->mode << 2 * pin_id;
        port->PUPDR |= currentConfig->pullMode << 2 * pin_id;
        port->ODR |= currentConfig->defaultState << pin_id;
        port->OSPEEDR |= 0x3 << 2*pin_id;

        if (currentConfig->mode == GPIO_AF_MODE)
        {
            uint32_t af_register_idx = pin_id / 8;
            uint32_t af_offset = pin_id % 8;
            port->AFR[af_register_idx] |= currentConfig->alternateFunctionMode << af_offset * 4;
        }

        currentConfig++;
    }
}

enum GpioPinState gpio_read(enum GpioChannel channel)
{
    uint32_t port_id = channel / 16;
    uint32_t pin_id  = channel % 16;
    xSemaphoreTake(GPIOMutexHandles[port_id], portMAX_DELAY);

    GPIO_TypeDef* port = GpioPorts[port_id];
    enum GpioPinState state = (port->IDR & 1 << pin_id) >> pin_id;
    xSemaphoreGive(GPIOMutexHandles[port_id]);
    return state;
}

void gpio_write(enum GpioChannel channel, enum GpioPinState state)
{
    uint32_t port_id = channel / 16;
    uint32_t pin_id  = channel % 16;
    xSemaphoreTake(GPIOMutexHandles[port_id], portMAX_DELAY);

    GPIO_TypeDef* port = GpioPorts[port_id];

    if (state)
    {
        port->BSRR = 1 << pin_id;
    }
    else
    {
        port->BRR = 1 << pin_id;
    }
    xSemaphoreGive(GPIOMutexHandles[port_id]);
}

void gpio_toggle(enum GpioChannel channel)
{
    uint32_t port_id = channel / 16;
    uint32_t pin_id  = channel % 16;
    xSemaphoreTake(GPIOMutexHandles[port_id], portMAX_DELAY);
    GPIO_TypeDef* port = GpioPorts[port_id];

    port->ODR ^= 1 << pin_id;
    xSemaphoreGive(GPIOMutexHandles[port_id]);
}

void gpio_register_write(uint32_t address, uint32_t value)
{
    volatile uint32_t * reg = (volatile uint32_t*)address;
    *reg = value;
}

uint32_t gpio_register_read(uint32_t address)
{
    volatile uint32_t * reg = (volatile uint32_t*)address;
    return *reg;
}