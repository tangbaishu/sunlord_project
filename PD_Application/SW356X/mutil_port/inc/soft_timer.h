/****************************************************************************
 * @copyright Copyright(C) 2020-2022 Ismartware Limited. All rights reserved.
 * @file soft_timer.h
 * @brief  soft timer
 * @author 005
 ****************************************************************************/
#ifndef _SOFT_TIMER_H_
#define _SOFT_TIMER_H_

#include "system.h"

// clang-format off
#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
    u32 startTime;        //!< the start time of the timer
    u32 interval;         //!< the interval of delay  
} soft_timer_t;

/**
 * @brief  start a soft timer
 * @param[in]  self soft timer, allocate memory at consumer
 * @param[in]  timeInMs the interval in Ms
 */
void Soft_Timer_Start(soft_timer_t* self, u32 timeInMs);

/**
 * @brief  stop a soft timer, to avoid miss used, a timer shall be stop if it has been started
 * @param[in]  self soft timer, allocate memory at consumer
 */
void Soft_Timer_Stop(soft_timer_t* self);

/**
 * @brief  check whether soft timer is expired
 * @param[in]  self the soft timer
 * @return true when it is expired
 */
bool Soft_Timer_Is_Expired(soft_timer_t* self);

/**
 * @brief  init soft timer
 */
void Soft_Timer_Init(void);


#ifdef __cplusplus
}
#endif
// clang-format on

#endif





