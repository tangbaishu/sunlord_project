/****************************************************************************
 * @copyright Copyright(C) 2020-2022 Ismartware Limited. All rights reserved.
 * @file mutil port policy.h
 * @brief  mutil port policy
 * @author 005
 ****************************************************************************/
#ifndef _MUTIL_PORT_POLICY_H_
#define _MUTIL_PORT_POLICY_H_

#include "system.h"

// clang-format off
#ifdef __cplusplus
extern "C" {
#endif

/**
 * state machine types
 */
typedef enum
{
	PORT_POLICY_IDLE = 0,	
    PORT_POLICY_READY,    
    PORT_POLICY_ERROR_RECOVER,    
    PORT_POLICY_STATE_NUM
} port_policy_state_e;

/**
 * @brief  initialize mutil port policy,it will be called by main.c when init
 */
void Mutil_Port_Policy_Init(void);
/**
 * @brief  mutil port policy run ,it will be called in main.c's loop 
 */
void Mutil_Port_Policy_Run(void);

#ifdef __cplusplus
}
#endif
// clang-format on

#endif





