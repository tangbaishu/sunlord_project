/****************************************************************************
 * @copyright Copyright(C) 2020-2022 Ismartware Limited. All rights reserved.
 * @file pd_policy_engine.c
 * @brief  pd source policy engine implementation
 * @author 005
 ****************************************************************************/
#include "pd_policy_engine.h"
#include "pd_pe_sink.h"
#include "pd_pe_power_swap.h"

// pd policy send message
static const sm_state_t* Pe_Send_Message_Run(void);
// soft reset sent
static const sm_state_t* Pe_Send_Soft_Reset_Run(void);
// soft reset recover
static void Pe_Wait_Accept_Soft_Reset_Entry(void);
static const sm_state_t* Pe_Wait_Accept_Soft_Reset_Run(void);
// on pd response pd disable but pd phy alive
static const sm_state_t* Pe_Disable_Run(void);
// pr swap to src
static const sm_state_t* Pe_Src_Ready_Run(void);

// clang-format off
/////////////////////////////////////////////////////////
// global pd policy engines
pd_policy_engine_t gPdPolicyEngines[1] = {{0}};

// clang-format off
// global pd policy engines to hold all state machine jumper
// it is not declared as static as it will be used in other source files e.g. pd_pe_vconn, pd_pe_source, etc.
const sm_state_t gPdPolicies[PE_STATE_NUM] = {
    // SNK
    [PE_SNK_STARTUP]                          = {Pe_Snk_Startup_Entry, Pe_Snk_Startup_Run},
    [PE_SNK_WAIT_FOR_CAPABILITIES]            = {0, Pe_Snk_Wait_For_Capabilities_Run},
    [PE_SNK_EVALUATE_CAPABILITY]              = {0, Pe_Snk_Evaluate_Capability_Run},
    [PE_SNK_SELECT_CAPABILITY]                = {Pe_Snk_Select_Capability_Entry, Pe_Snk_Select_Capability_Run},
    [PE_SNK_TRANSITION_SINK]                  = {Pe_Snk_Transition_Sink_Entry, Pe_Snk_Transition_Sink_Run},
    [PE_SNK_READY]                            = {Pe_Snk_Ready_Entry, Pe_Snk_Ready_Run},
    [PE_SNK_HARD_RESET]                       = {0, Pe_Snk_Hard_Reset_Run},
    [PE_SNK_TRANSITION_TO_DEFAULT]            = {Pe_Snk_Transition_To_Default_Entry, Pe_Snk_Transition_To_Default_Run},          
    
    //commom
    [PE_SEND_MESSAGE]                         = {0, Pe_Send_Message_Run},     
    [PE_SEND_SOFT_RESET]                      = {0, Pe_Send_Soft_Reset_Run},                
    [PE_WAIT_ACCEPT_SOFT_RESET]               = {Pe_Wait_Accept_Soft_Reset_Entry, Pe_Wait_Accept_Soft_Reset_Run},  
    [PE_DISABLED]                             = {0, Pe_Disable_Run},
	[PE_SRC_READY]                            = {0, Pe_Src_Ready_Run},

	[PE_SNK_RESPONSE_PRS_PRIMARY]             = {Pe_Snk_Response_Prs_Primary_Entry,  Pe_Snk_Response_Prs_Primary_Run},     
    [PE_SNK_RESPONSE_PRS_SECONDARY]           = {0,  Pe_Snk_Response_Prs_Secondary_Run},  
    [PE_REQUEST_PRS]                          = {Pe_Request_Prs_Entry,  Pe_Request_Prs_Run},   	
};
// clang-format on

/**
 * @brief  state machine switching callback
 * @note   it is for print current state or tell outside which step is it
 * @param[in]  self the current state
 * @param[in]  data the data of state machine, it is set when statemachine initialized
 */
static void Pd_State_Switching(const sm_state_t* self)
{
    pd_policy_engine_t* p = (pd_policy_engine_t*)(&gPdPolicyEngines[PORT_NUM]);
    pe_state_e state = (pe_state_e)((self - gPdPolicies));

    p->currentState = state;
}


//////////////////////////////////called by sink policy layer//////////////////////////////////////////////
/**
 * @brief  policy engine initialize, the state machine will be in PE_SRC_STARTUP, and it is not enabled
 * @param[in]  isCc2Connect,when true, pd communicate on cc2 
 */
void Pd_Policy_Engine_Init(bool isCc2Connect)
{
    pd_policy_engine_t* p = &gPdPolicyEngines[PORT_NUM];
	
	p->pdRxMessageSummary = Pd_Init_Rx_Message_Summary();   //get rx message summary 
    p->pdTxMessageSummary = Pd_Init_Tx_Message_Summary();   //get tx message summary

   p->isCc2Connect = isCc2Connect;

    // init the state machine
    State_Machine_Init(&p->sm, &gPdPolicies[PE_SNK_STARTUP], Pd_State_Switching);
}

/**
 * @brief pd sink policy run 
 */
bool Pd_Sink_Policy_Run(void)
{
	pd_policy_engine_t* p = &gPdPolicyEngines[PORT_NUM];

    if (Pd_Is_Event_Happen(PD_RX_SOFT_RESET_EVENT))  //rx soft reset
    {
        Pd_Clear_Event(PD_RX_SOFT_RESET_EVENT); //clear rx soft reset 
        pd_mesg_summ_t pdTxMessageSummary;
        pdTxMessageSummary.packet_start = PD_SOP;
        pdTxMessageSummary.mesg_class = PD_CONTROL_CLASS;
		pdTxMessageSummary.mesg_type = PD_Accept;
        State_Machine_Set_State(&p->sm, 
                                &gPdPolicies[Pd_Policy_Send_Msg(&pdTxMessageSummary,
                                                                PE_SNK_WAIT_FOR_CAPABILITIES, 
                                                                PE_SNK_HARD_RESET,
		                                                        0,0)]);
    }
    if (Pd_Is_Event_Happen(PD_RX_HARD_RESET_EVENT)) //rx hard reset
    {
        Pd_Clear_Event(PD_RX_HARD_RESET_EVENT);//clear rx hard reset 
        State_Machine_Set_State(&p->sm, &gPdPolicies[PE_SNK_TRANSITION_TO_DEFAULT]);
    }	

    State_Machine_Run(&p->sm);
	Low_Power_Not_Sleep();  //pd use 10mhz clk,in this sink apply scan,will enter deep sleep,close high freq clk,so when run pd sink,not sleep!!!
    return (PE_SRC_READY == p->currentState);	
}
//////////////////////////////////end//////////////////////////////////////////////

/////////////////////called by pd policy engine layer///////////////////////////
/**
 * @brief  policy trigger send message
 * @param[in]  self the policy engine instance
 * @param[in]  pdTxMessageSummary, the tx msg summary
 * @param[in]  sendSuccessState the next state when this msg send ok
 * @param[in]  sendErrorState the next state when this msg send fail
 * @param[in]  pdData pd tx data,except header
 * @param[in]  wordNum pd tx data num,word/step
 * @return  the policy engine instance
 */
pe_state_e Pd_Policy_Send_Msg(pd_mesg_summ_t* pdTxMessageSummary, pe_state_e sendSuccessState, pe_state_e sendErrorState, uint8_t* pdData, uint8_t wordNum)
{
    pd_policy_engine_t* p = &gPdPolicyEngines[PORT_NUM];
    
    p->sendSuccessState = sendSuccessState;
    p->sendErrorState = sendErrorState;
    
	p->pdTxMessageSummary->packet_start = pdTxMessageSummary->packet_start;
	p->pdTxMessageSummary->mesg_class = pdTxMessageSummary->mesg_class;
	p->pdTxMessageSummary->mesg_type = (PD_Not_Supported == pdTxMessageSummary->mesg_type && (!Pd_Is_Rev3_0())) ? PD_Reject : pdTxMessageSummary->mesg_type;
	p->pdTxBuff = Pd_Get_Tx_Buff_Start_Address(PD_EXTENDED_CLASS == pdTxMessageSummary->mesg_class ? true : false);
	uint8_t byteNum = (uint8_t)(wordNum << 2);
	for (uint8_t i = 0; i < byteNum; i++)
	{
		*(p->pdTxBuff + 2 + i) = *(pdData + i);  //+2 is to except header,start from data block to set pdTxBuff
	}         
	Pd_Tx_Message(wordNum, p->pdTxBuff);  //start tx,also set header in pdTxBuff(top 2 bytes)
    return PE_SEND_MESSAGE;
}
//////////////////////////////////end//////////////////////////////////////////////

///////////////////////source and sink common policy engine////////////////////////
/**
 * @brief  run function of PE_SEND_MESSAGE
 * @return const sm_state_t*, the new state
 */
static const sm_state_t* Pe_Send_Message_Run(void)
{
    pd_policy_engine_t* p = &gPdPolicyEngines[PORT_NUM];
	pe_state_e state = PE_SEND_MESSAGE;
    
    if (Pd_Is_Event_Happen(PD_TX_MESSAGE_SUCCESS_EVENT))//tx success
    {
        Pd_Clear_Event(PD_TX_MESSAGE_SUCCESS_EVENT);//clear tx success	
        state = p->sendSuccessState;
    }
    if (Pd_Is_Event_Happen(PD_TX_MESSAGE_FAIL_EVENT))//tx fail
    {
        Pd_Clear_Event(PD_TX_MESSAGE_FAIL_EVENT);//clear tx fail	
        state = p->sendErrorState;
    }    	
    return &gPdPolicies[state]; 
}

/**
 * @brief  run function of PE_SEND_SOFT_RESET
 * @return const sm_state_t*, the new state
 */
static const sm_state_t* Pe_Send_Soft_Reset_Run(void)
{
    pd_policy_engine_t* p = &gPdPolicyEngines[PORT_NUM];

	pd_mesg_summ_t pdTxMessageSummary;
	pdTxMessageSummary.packet_start = PD_SOP;
	pdTxMessageSummary.mesg_class = PD_CONTROL_CLASS;
	pdTxMessageSummary.mesg_type = PD_Soft_Reset;    
	return &gPdPolicies[Pd_Policy_Send_Msg(&pdTxMessageSummary,
						                   PE_WAIT_ACCEPT_SOFT_RESET, 
							               PE_SNK_HARD_RESET,
							               0,0)];  
}

/**
 * @brief  entry function of PE_WAIT_ACCEPT_SOFT_RESET
 * @param[in]  data the policy engine instance
 */
static void Pe_Wait_Accept_Soft_Reset_Entry(void)
{
    pd_policy_engine_t* p = &gPdPolicyEngines[PORT_NUM];

    Soft_Timer_Start(&p->genericTimer, SENDER_RESPONSE_TIME_REV3);  //restart timer
}

/**
 * @brief  run function of PE_WAIT_ACCEPT_SOFT_RESET
 * @return const sm_state_t*, the new state
 */
static const sm_state_t* Pe_Wait_Accept_Soft_Reset_Run(void)
{
    pd_policy_engine_t* p = &gPdPolicyEngines[PORT_NUM];
    pe_state_e state = PE_WAIT_ACCEPT_SOFT_RESET;

    if (Pd_Is_Event_Happen(PD_RX_MESSAGE_SUCCESS_EVENT))
    {
        Pd_Clear_Event(PD_RX_MESSAGE_SUCCESS_EVENT);
        Soft_Timer_Stop(&p->genericTimer);		
		state = PD_Accept == p->pdRxMessageSummary->mesg_type ? PE_SNK_WAIT_FOR_CAPABILITIES : PE_SNK_HARD_RESET;               
    }
    if (Soft_Timer_Is_Expired(&p->genericTimer))
    {
		Soft_Timer_Stop(&p->genericTimer);
		state = PE_SNK_HARD_RESET;   
    }
    return &gPdPolicies[state];    
}

/**
 * @brief  run function of PE_DISABLED
 * @return const sm_state_t*, the new state
 */
static const sm_state_t* Pe_Disable_Run(void)
{
    pd_policy_engine_t* p = &gPdPolicyEngines[PORT_NUM];
	
    Soft_Timer_Stop(&p->genericTimer);  
	
    return &gPdPolicies[PE_DISABLED]; //wait,until sink policy to apply rp   
}

/**
 * @brief  run function of PE_SRC_READY
 * @return const sm_state_t*, the new state
 */
static const sm_state_t* Pe_Src_Ready_Run(void)
{
    pd_policy_engine_t* p = &gPdPolicyEngines[PORT_NUM];
	
    Soft_Timer_Stop(&p->genericTimer);  
	
    return &gPdPolicies[PE_SRC_READY]; 
}

