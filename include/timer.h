/** @file timer.h
*   @brief Created on 2021-03-20 by Ben
*/
#pragma once

/**********************************************************************
* Includes
**********************************************************************/
#include "timer_config.h"

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

void timer_init();
void timer_enable(TimerChannel channel);
void timer_set_cc_value(TimerChannel timer, TimerCompareCaptureChannel channel, uint32_t value);

#ifdef __cplusplus
} // extern "C"
#endif