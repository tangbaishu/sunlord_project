/****************************************************************************
 * @copyright Copyright(C) 2020-2022 Ismartware Limited. All rights reserved.
 * @file mutil_port_pd_policy.h
 * @brief  mutil port pd message response policy engine interface
 * @author 005
 ****************************************************************************/
#ifndef MUTIL_PORT_PD_POLICY_H
#define MUTIL_PORT_PD_POLICY_H

#include "state_machine.h"
#include "soft_timer.h"
#include "api.h"
#include "pd_message.h"

#ifdef __cplusplus
extern "C" {
#endif   

/////////////////////////////////////////    Timer //////////////////////////////////
// send response timer tSenderResponse, the range PD3.0 is 27-33ms, typic.30  PD2.0 is 24-30ms, typic.27
#define SENDER_RESPONSE_TIME_REV3 (29)

/** pd policy state declaration */
typedef enum pe_state
{
    PE_SRC_READY,                           //!< src ready	
    PE_SEND_MESSAGE,                        //!< send message
    PE_SEND_SOFT_RESET,                     //!< send soft reset  

    PE_STATE_NUM                            //!< number of pe_state_e
} pe_state_e;

/** PD policy engine for a specific port */
typedef struct pd_policy_engine
{
    state_machine_t sm;                             //!< state machine
 
	const pd_mesg_summ_t* pdRxMessageSummary;
	pd_mesg_summ_t* pdTxMessageSummary;
    
    pe_state_e currentState;                        //!< the current procssing state, readonly
    pe_state_e sendSuccessState;                    //!< the next state when send success
    pe_state_e sendErrorState;                      //!< the next state when send error 

    const uint8_t* pdRxBuff;    
    uint8_t* pdTxBuff;
    
    bool isMisMatch : 1;                          //true:request message indicate mismatch
    bool isSharedTestModeEnter : 1;               //true:rx BIST Shared Test Mode Entry
    bool isSharedTestModeExit : 1;                //true:rx BIST Shared Test Mode Exit
    bool isInSharedTestMode : 1;                  //true:now is in Shared Test Mode   
}pd_policy_engine_t;


/**
 * @brief  policy engine initialize,it will be called by mutil_port_policy_init
 */
void Mutil_Port_Pd_Policy_Init(void);

#ifdef __cplusplus
}
#endif

#endif        // end of MUTIL_PORT_PD_POLICY_H
