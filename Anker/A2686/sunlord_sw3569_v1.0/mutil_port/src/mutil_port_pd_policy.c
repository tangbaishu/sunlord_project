/****************************************************************************
 * @copyright Copyright(C) 2020-2022 Ismartware Limited. All rights reserved.
 * @file mutil_port_pd_policy.c
 * @brief  mutil port pd message response policy engine implementation
 * @author 005
 ****************************************************************************/
#include "mutil_port_pd_policy.h"

// mutil port pd policy src ready
static const sm_state_t* Pe_Src_Ready_Run(void);
// mutil port pd policy send message
static const sm_state_t* Pe_Send_Message_Run(void);
// mutil port pd policy soft reset sent
static const sm_state_t* Pe_Send_Soft_Reset_Run(void);


// clang-format off
/////////////////////////////////////////////////////////
// global pd policy engines
pd_policy_engine_t gPdPolicyEngines[1] = {{0}};

// clang-format off
// global pd policy engines to hold all state machine jumper
// it is not declared as static as it will be used in other source files e.g. pd_pe_vconn, pd_pe_source, etc.
const sm_state_t gPdPolicies[PE_STATE_NUM] = {
    [PE_SRC_READY]                            = {0, Pe_Src_Ready_Run}, 	
    [PE_SEND_MESSAGE]                         = {0, Pe_Send_Message_Run},     
    [PE_SEND_SOFT_RESET]                      = {0, Pe_Send_Soft_Reset_Run},   	
};
// clang-format on

//////////////////////////////////mutil port pd policy layer//////////////////////////////////////////////
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
    pd_policy_engine_t* p = &gPdPolicyEngines[0];
    
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

/**
 * @brief  state machine switching callback
 * @note   it is for print current state or tell outside which step is it
 * @param[in]  self the current state
 * @param[in]  data the data of state machine, it is set when statemachine initialized
 */
static void Pd_State_Switching(const sm_state_t* self)
{
    pd_policy_engine_t* p = (pd_policy_engine_t*)(&gPdPolicyEngines[0]);
    pe_state_e state = (pe_state_e)((self - gPdPolicies));

    p->currentState = state;
}

/**
 * @brief  run function of PE_SRC_READY
 * @return const sm_state_t*, the new state
 */
static const sm_state_t* Pe_Src_Ready_Run(void)
{
    pd_policy_engine_t* p = &gPdPolicyEngines[0];    
    pe_state_e state = PE_SRC_READY;    
	
    if (Pd_Is_Event_Happen(PD_RX_MESSAGE_SUCCESS_EVENT))
    {
        if (PD_SOP == p->pdRxMessageSummary->packet_start)
        {
            if (PD_Request == p->pdRxMessageSummary->mesg_type)
            {
                if (((*(uint32_t*)(p->pdRxBuff + 2)) >> 26) & 0x1)  //request message set Mismatch
                {
                    p->isMisMatch = true;
                }
            }
            else if (PD_BIST_Message == p->pdRxMessageSummary->mesg_type) 
            {
                uint8_t bistMode = ((*(uint32_t*)(p->pdRxBuff + 2)) >> 28) & 0xf;  //get bist message content
                if (0x9 == bistMode)
                {
                    p->isSharedTestModeEnter = true;
                }
                else if (0xa == bistMode)
                {
                    p->isSharedTestModeExit = true;                
                }
            }
            else if (PD_Get_Source_Info == p->pdRxMessageSummary->mesg_type) 
            {
                if (Pd_Is_Ready())
                {
                    Pd_Clear_Event(PD_RX_MESSAGE_SUCCESS_EVENT);  //user process PD_Get_Source_Info                
                    pd_mesg_summ_t pdTxMessageSummary;
                    pdTxMessageSummary.packet_start = PD_SOP;
                    pdTxMessageSummary.mesg_class = PD_DATA_CLASS;
                    pdTxMessageSummary.mesg_type = PD_Source_Info;
                    //example                    
                    uint32_t data[1]; 
                    data[0] = (uint32_t)(100 << 16) |  //Port Maximum PDP:100w 
                              (uint32_t)(Pd_Is_5a_Cable() ? 65 : 60) << 8 | //can use 65w,Port Present PDP   
                              (uint32_t)(Pd_Is_5a_Cable() ? 65 : 60); //Port Reported PDP
                    //set the data content according to the actual situation                
                    state = Pd_Policy_Send_Msg(&pdTxMessageSummary,
                                               PE_SRC_READY, 
                                               PE_SEND_SOFT_RESET,
                                               (uint8_t*)data,1);                
                }
            }            
        }    
    }        
	
    return &gPdPolicies[state]; 
}

/**
 * @brief  run function of PE_SEND_MESSAGE
 * @return const sm_state_t*, the new state
 */
static const sm_state_t* Pe_Send_Message_Run(void)
{
    pd_policy_engine_t* p = &gPdPolicyEngines[0];
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
    Pd_Send_Soft_Reset();
	return &gPdPolicies[PE_SRC_READY];  
}
//////////////////////////////////end//////////////////////////////////////////////

//////////////////////////////////user hook function//////////////////////////////////////////////
/**
 * @brief  user handle Get_Source_Info,get Request&&BIST Message context
           it is a hook function, called by pd main process
           it mean as long as main loop execute to pd main process,this function will be called once
 * @return when true ,user is processing
 */
static bool User_Pd_Policy_Hook(void)
{
    pd_policy_engine_t* p = (pd_policy_engine_t*)(&gPdPolicyEngines[0]);
    
    State_Machine_Run(&p->sm);
    
    if (p->currentState > PE_SRC_READY)
    {
        Low_Power_Not_Sleep();
        return true; //user is handle pd event
    }
    return false; 
}
//////////////////////////////////end//////////////////////////////////////////////

//////////////////////////////////called by mutil port policy//////////////////////////////////////////////
/**
 * @brief  policy engine initialize,it will be called by mutil_port_policy_init
 */
void Mutil_Port_Pd_Policy_Init(void)
{
    pd_policy_engine_t* p = &gPdPolicyEngines[0];
    
    p->isSharedTestModeEnter = false;
    p->isSharedTestModeExit = false;
    p->isInSharedTestMode = false;
    p->isMisMatch = false;  
	
	p->pdRxMessageSummary = Pd_Init_Rx_Message_Summary();   //get rx message summary 
    p->pdTxMessageSummary = Pd_Init_Tx_Message_Summary();   //get tx message summary
    p->pdRxBuff = Pd_Get_Rx_Buff_Start_Address(FALSE);  //in this appliction ,only care none extended message

    Pd_Init_Hook_Func(0, User_Pd_Policy_Hook);
    // init the state machine
    State_Machine_Init(&p->sm, &gPdPolicies[PE_SRC_READY], Pd_State_Switching);
}
//////////////////////////////////end//////////////////////////////////////////////





