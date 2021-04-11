/** @file timer.c
*   @brief Created on 2021-03-20 by Ben
*/
/**********************************************************************
* Includes
**********************************************************************/
#include "timer.h"
#include "stm32f0xx.h"

/**********************************************************************
* Module Preprocessor Constants
**********************************************************************/

/**********************************************************************
* Module Preprocessor Macros
**********************************************************************/

/**********************************************************************
* Module Typedefs
**********************************************************************/

static TIM_TypeDef* const Timers[] =
{
    TIM1,
    TIM2,
    TIM3,
};

static const uint32_t TimerClockEnableBitMasks[] =
{
    RCC_APB2ENR_TIM1EN,
    RCC_APB1ENR_TIM2EN,
    RCC_APB1ENR_TIM3EN,
};

static volatile uint32_t* const TimerRccRegisters[] =
{
    &RCC->APB2ENR,
    &RCC->APB1ENR,
    &RCC->APB1ENR,
};

/*********************************************************************
* Module Variable Definitions
**********************************************************************/

/**********************************************************************
* Function Prototypes
**********************************************************************/

/**********************************************************************
* Function Definitions
**********************************************************************/

// TODO: CHxN outputs
void timer_init()
{
    const TimerConfig *config = timer_get_config();

    while (config->channel != TIMER_CHANNEL_END)
    {
        const TimerCompareCaptureConfig* channelConfig = config->channelConfig;

        /* Enable Clock */
        *TimerRccRegisters[config->channel] |= TimerClockEnableBitMasks[config->channel];

        /* Configure Timer */
        TIM_TypeDef* TIM = Timers[config->channel];
        TIM->CR1 = config->reload << TIM_CR1_ARPE_Pos;
        TIM->PSC = config->prescaler;
        TIM->ARR = config->reloadValue;

        while (channelConfig->channel != CC_CHANNEL_END)
        {
            /* Configure Channel */
            uint32_t regOffset = channelConfig->channel / 2;
            uint32_t bitOffset = (channelConfig->channel % 2) ;

            volatile uint32_t * CCMR = (volatile uint32_t*)TIM + 6 + regOffset;
            volatile uint32_t * CCR  = (volatile uint32_t*)TIM + 13 + channelConfig->channel;
            *CCMR = (channelConfig->mode << TIM_CCMR1_OC1M_Pos | channelConfig->reload << TIM_CCMR1_OC1PE_Pos)
                    << bitOffset*8;
            *CCR = channelConfig->compareCaptureValue;
            TIM->CCER |= TIM_CCER_CC1E << channelConfig->channel*4;
            channelConfig++;
        }

        /* Set UG event to update register values */
        TIM->EGR = TIM_EGR_UG;
        timer_enable(config->channel);
        config++;
    }
}

void timer_enable(TimerChannel channel)
{
    TIM_TypeDef* TIM = Timers[channel];
    TIM->CR1 |= TIM_CR1_CEN;
}

void timer_set_cc_value(TimerChannel timer, TimerCompareCaptureChannel channel, uint32_t value)
{
    TIM_TypeDef* TIM = Timers[timer];
    volatile uint32_t * CCR  = (volatile uint32_t*)TIM + 13 + channel;
    *CCR = value;
    TIM->EGR = TIM_EGR_UG;
}
