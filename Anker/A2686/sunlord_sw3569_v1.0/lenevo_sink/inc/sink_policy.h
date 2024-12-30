/****************************************************************************
 * @copyright Copyright(C) 2020-2022 Ismartware Limited. All rights reserved.
 * @file sink policy.h
 * @brief  sink policy
 * @author 005
 ****************************************************************************/
#ifndef _SINK_POLICY_H_
#define _SINK_POLICY_H_

#include "system.h"

// clang-format off
#ifdef __cplusplus
extern "C" {
#endif

/**
 * sink state machine types
 */
typedef enum
{
	SNK_IDLE_STATE = 0,
	
    UNATTACHED_SNK_STATE,
    ATTACHWAIT_SNK_STATE,
    ATTACHED_SNK_STATE,	

    UNATTACHED_SRC_STATE,
    ATTACHED_SRC_STATE,

    SINK_STATE_NUM
} sink_state_e;

/**
 * @brief  initialize sink policy
 */
void Sink_Policy_Init(void);
/**
 * @brief  sink policy run 
 */
void Sink_Policy_Run(void);


#ifdef __cplusplus
}
#endif
// clang-format on

#endif





