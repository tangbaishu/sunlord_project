/****************************************************************************
 * @copyright Copyright(C) 2020-2022 Ismartware Limited. All rights reserved.
 * @file pd_policy_engine.h
 * @brief  pd policy engine interface
 * @author 005
 ****************************************************************************/
#ifndef PD_POLICY_ENGINE_H
#define PD_POLICY_ENGINE_H

#include "state_machine.h"
#include "soft_timer.h"
#include "api.h"
#include "pd_message.h"

#ifdef __cplusplus
extern "C" {
#endif   

#define PORT_NUM  0
#define SINK_WAIT_PR_SWAP (1800)

/////////////////////////////////////////    Timer //////////////////////////////////
// send response timer tSenderResponse, the range PD3.0 is 27-33ms, typic.30  PD2.0 is 24-30ms, typic.27
#define SENDER_RESPONSE_TIME_REV3 (29)

/** pd policy state declaration */
typedef enum pe_state
{
    // SNK
    PE_SNK_STARTUP,                         //!< sink startup
    PE_SNK_WAIT_FOR_CAPABILITIES,           //!< sink wait for capabilities
    PE_SNK_EVALUATE_CAPABILITY,             //!< sink evaluate capabilities
    PE_SNK_SELECT_CAPABILITY,               //!< sink select capability
    PE_SNK_TRANSITION_SINK,                 //!< sink transition sink
    PE_SNK_READY,                           //!< sink ready
    PE_SNK_HARD_RESET,                      //!< sink send hard reset    
    PE_SNK_TRANSITION_TO_DEFAULT,           //!< sink transition to default        

    //commom
    PE_SEND_MESSAGE,                        //!< send message
    PE_SEND_SOFT_RESET,                     //!< send soft reset
    PE_WAIT_ACCEPT_SOFT_RESET,              //!< wait soft reset be accepted   
    PE_DISABLED,                            //!< pd disabled
    PE_SRC_READY,                           //!< src ready	

    PE_SNK_RESPONSE_PRS_PRIMARY,            //!< sink Primary response pr swap
    PE_SNK_RESPONSE_PRS_SECONDARY,          //!< sink secondary response pr swap
    PE_REQUEST_PRS,                         //!< request pr swap 	
    
    PE_STATE_NUM                            //!< number of pe_state_e
} pe_state_e;

/** PD policy engine for a specific port */
typedef struct pd_policy_engine
{
    state_machine_t sm;                             //!< state machine
    
    soft_timer_t genericTimer;                      //!< the generic timer, common use
	
	const pd_mesg_summ_t* pdRxMessageSummary;
	pd_mesg_summ_t* pdTxMessageSummary;
    
    pe_state_e currentState;                        //!< the current procssing state, readonly
    pe_state_e sendSuccessState;                    //!< the next state when send success
    pe_state_e sendErrorState;                      //!< the next state when send error 

    const uint8_t* pdRxBuff;    
    uint8_t* pdTxBuff;

    bool isCc2Connect : 1;	
}pd_policy_engine_t;

/**
 * @brief  policy engine initialize, the state machine will be in PE_SRC_STARTUP, and it is not enabled
 */
void Pd_Policy_Engine_Init(bool isCc2Connect);
/**
 * @brief pd sink policy run 
 */
bool Pd_Sink_Policy_Run(void);
/// policy send message
pe_state_e Pd_Policy_Send_Msg(pd_mesg_summ_t* pdTxMessageSummary, pe_state_e sendSuccessState, pe_state_e sendErrorState, uint8_t* pdData, uint8_t wordNum);


#ifdef __cplusplus
}
#endif

#endif        // end of PD_POLICY_ENGINE_EXT_H
