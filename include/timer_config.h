/** @file timer_config.h
*   @brief Created on 2021-03-20 by Ben
*/
#pragma once

/**********************************************************************
* Includes
**********************************************************************/
#include <stdint.h>
#include <stdbool.h>

/**********************************************************************
* Preprocessor Constants
**********************************************************************/

/**********************************************************************
* Typedefs
**********************************************************************/
typedef enum {
    TIMER1_CHANNEL,
    TIMER2_CHANNEL,
    TIMER3_CHANNEL,
    TIMER_CHANNEL_END,
} TimerChannel;

typedef enum
{
    CC_CHANNEL_1,
    CC_CHANNEL_2,
    CC_CHANNEL_3,
    CC_CHANNEL_4,
    CC_CHANNEL_END,
} TimerCompareCaptureChannel;

typedef enum {
    TIM_COUNTER_UP,
    TIM_COUNTER_DOWN,
} CounterDir;

typedef enum {
    CC_MODE_PWM               = 0b110,
    CC_MODE_FROZEN            = 0b000,
    CC_MODE_ACTIVE_ON_MATCH   = 0b001,
    CC_MODE_INACTIVE_ON_MATCH = 0b010,
    CC_MODE_TOGGLE            = 0b011,
    CC_MODE_FORCE_ACTIVE      = 0b101,
    CC_MODE_FORCE_INACTIVE    = 0b100,
} CompareCaptureMode;

typedef struct
{
    TimerCompareCaptureChannel channel;
    uint32_t compareCaptureValue;
    CompareCaptureMode mode;
    bool reload;
}TimerCompareCaptureConfig;

typedef struct {
    TimerChannel channel;
    uint32_t prescaler;
    uint32_t reloadValue;
    CounterDir direction;
    bool reload;
    const TimerCompareCaptureConfig* channelConfig;
} TimerConfig;

/**********************************************************************
* Function Prototypes
**********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
const TimerConfig * timer_get_config(void);

#ifdef __cplusplus
} // extern "C"
#endif