/****************************************************************************
 * @copyright Copyright(C) 2020-2022 Ismartware Limited. All rights reserved.
 * @file pd_pe_sink.c
 * @brief  pd policy engine sink implementation
 * @author 005
 ****************************************************************************/
#include "pd_pe_sink.h"
#include "zr_gpio.h"

/////////////////////////////////////////    Timer //////////////////////////////////
// sink wait ps_rdy Timer the range is 450-550 ms, typic.500
#define PS_TRANSITION_TIME (500)

pe_state_e Process_Ext_Message_In_Pe_Snk_Ready(const pd_mesg_summ_t* pdRxMessageSummary);
pe_state_e Process_Data_Message_In_Pe_Snk_Ready(const pd_mesg_summ_t* pdRxMessageSummary);
pe_state_e Process_Control_Message_In_Pe_Snk_Ready(const pd_mesg_summ_t* pdRxMessageSummary);

// global pd policy states
extern const sm_state_t gPdPolicies[PE_STATE_NUM];
extern pd_policy_engine_t gPdPolicyEngines[1];

/**
 * @brief  process extend message in pe_sink_ready
 * @param[in]  pdTxMessageSummary, the rx msg summary
 */
pe_state_e Process_Ext_Message_In_Pe_Snk_Ready(const pd_mesg_summ_t* pdRxMessageSummary)
{
	pd_policy_engine_t* p = &gPdPolicyEngines[PORT_NUM];
    (void)pdRxMessageSummary;
	
	pd_mesg_summ_t pdTxMessageSummary;
	pdTxMessageSummary.packet_start = PD_SOP;
	pdTxMessageSummary.mesg_class = PD_CONTROL_CLASS;
	pdTxMessageSummary.mesg_type = PD_Not_Supported;	
	return Pd_Policy_Send_Msg(&pdTxMessageSummary,
                              PE_SNK_READY, 
                              PE_SEND_SOFT_RESET,
		                      0,0);
}

/**
 * @brief  process extend message in pe_sink_ready
 * @param[in]  pdTxMessageSummary, the rx msg summary
 */
pe_state_e Process_Data_Message_In_Pe_Snk_Ready(const pd_mesg_summ_t* pdRxMessageSummary)
{
    pd_policy_engine_t* p = &gPdPolicyEngines[PORT_NUM];
	pe_state_e state = PE_SNK_READY; 
    
    if (!Pd_Is_Rev3_0() && PD_Sink_Capabilities < pdRxMessageSummary->mesg_type && PD_Vendor_Defined != pdRxMessageSummary->mesg_type)
    {
		pd_mesg_summ_t pdTxMessageSummary;
		pdTxMessageSummary.packet_start = PD_SOP;
		pdTxMessageSummary.mesg_class = PD_CONTROL_CLASS;
		pdTxMessageSummary.mesg_type = PD_Reject;	
		state = Pd_Policy_Send_Msg(&pdTxMessageSummary,
								   PE_SNK_READY, 
								   PE_SEND_SOFT_RESET,
								   0,0);		
    }
    else
    {        
        switch (pdRxMessageSummary->mesg_type)
        {
            case PD_Source_Capabilities: 
                 state = PE_SNK_EVALUATE_CAPABILITY;
                 break;                        
            case PD_Vendor_Defined:
            {				
				p->pdRxBuff = Pd_Get_Rx_Buff_Start_Address(false);			
				uint32_t vdmHeader = *(uint32_t*)(p->pdRxBuff + 2);  //must attention addr need to 4 bytes align
				if (((vdmHeader >> 15) & 0x1) && !((vdmHeader >> 6) & 0x3))  //structured && req command type
				{
					uint8_t wordNum = 0;
					uint32_t data[5] = {0};
					
					uint16_t svid = (vdmHeader >> 16) & 0xffff;
					uint8_t command = vdmHeader & 0x1f;
					if (0xff00 == svid)  //sid
					{					
						switch (command)
						{
							case 0x1:    //discover id
								wordNum = 5;
								data[0] = 0xff00a041;
								data[1] = 0x6c0017ef;
								data[2] = 0x0;
								data[3] = 0x30cb000;	
								data[4] = 0x2;								
								break;
							case 0x2:    //discover svid
								wordNum = 3;
								data[0] = 0xff00a042;
								data[1] = 0xff0117ef;
								data[2] = 0x0;							
								break;
                            default:  //encode according to actual situation
								break;								
						}						
					}
					else if (0xff01 == svid)  //dp vid
					{
						switch (command)
						{
							case 0x3:    //discover mode
								wordNum = 2;
								data[0] = 0xff01a043;
								data[1] = 0xc0045;						
								break;
							case 0x4:    //enter mode
								wordNum = 1;
								data[0] = 0xff01a144;						
								break;							
                            default:  //encode according to actual situation
								break;								
						}					
					}					
					else if (0x17ef == svid)  //vid
					{
						switch (command)
						{
							case 0x3:    //discover mode
								wordNum = 2;
								data[0] = 0x17efa043;
								data[1] = 0x1;						
								break;
							case 0x4:    //enter mode
								wordNum = 1;
								data[0] = 0x17efa144;						
								break;		
							case 0x10:    //dp status update
								wordNum = 3;
								data[0] = 0x17efa150;
								data[1] = 0x303;	
                                data[2] = 0x34c000;								
								break;								
                            default:  //encode according to actual situation
								break;								
						}						
					}
					
					if (wordNum)  //have data ,send vdm
					{
						pd_mesg_summ_t pdTxMessageSummary;
						pdTxMessageSummary.packet_start = PD_SOP;
						pdTxMessageSummary.mesg_class = PD_DATA_CLASS;
						pdTxMessageSummary.mesg_type = PD_Vendor_Defined;							
						state = Pd_Policy_Send_Msg(&pdTxMessageSummary,
												   PE_SNK_READY, 
												   PE_SEND_SOFT_RESET,
												   (uint8_t*)data,wordNum);					
					}			
				}
				//others ignore
                break;    			
			}                 
            default:
			{
				pd_mesg_summ_t pdTxMessageSummary;
				pdTxMessageSummary.packet_start = PD_SOP;
				pdTxMessageSummary.mesg_class = PD_CONTROL_CLASS;
				pdTxMessageSummary.mesg_type = PD_Not_Supported;				
                state = Pd_Policy_Send_Msg(&pdTxMessageSummary,
                                           PE_SNK_READY, 
                                           PE_SEND_SOFT_RESET,
		                                   0,0);              
                break; 			
			}                 
        }
    }            
    return state;
}

/**
 * @brief  process extend message in pe_sink_ready
 * @param[in]  pdTxMessageSummary, the rx msg summary
 */
pe_state_e Process_Control_Message_In_Pe_Snk_Ready(const pd_mesg_summ_t* pdRxMessageSummary)
{
    pd_policy_engine_t* p = &gPdPolicyEngines[PORT_NUM];
	pe_state_e state = PE_SNK_READY; 
    
    if (!Pd_Is_Rev3_0() && PD_Soft_Reset < pdRxMessageSummary->mesg_type)
    {
		pd_mesg_summ_t pdTxMessageSummary;
		pdTxMessageSummary.packet_start = PD_SOP;
		pdTxMessageSummary.mesg_class = PD_CONTROL_CLASS;
		pdTxMessageSummary.mesg_type = PD_Reject;	
		state = Pd_Policy_Send_Msg(&pdTxMessageSummary,
								   PE_SNK_READY, 
								   PE_SEND_SOFT_RESET,
								   0,0);
    }
    else
    {
        switch (pdRxMessageSummary->mesg_type)
        {              
            case PD_Get_Sink_Cap:
			{
				pd_mesg_summ_t pdTxMessageSummary;
				pdTxMessageSummary.packet_start = PD_SOP;
				pdTxMessageSummary.mesg_class = PD_DATA_CLASS;
				pdTxMessageSummary.mesg_type = PD_Sink_Capabilities;
				uint32_t buff = 0x1912c;  //5v3a sink cap
				state = Pd_Policy_Send_Msg(&pdTxMessageSummary,
										   PE_SNK_READY, 
										   PE_SEND_SOFT_RESET,
										   (uint8_t*)&buff,1);  					
                break;     			
			}      
            case PD_Get_Sink_Cap_Extended:	             
                break;    			
            default:
			{
				pd_mesg_summ_t pdTxMessageSummary;
				pdTxMessageSummary.packet_start = PD_SOP;
				pdTxMessageSummary.mesg_class = PD_CONTROL_CLASS;
				pdTxMessageSummary.mesg_type = PD_Not_Supported;				
                state = Pd_Policy_Send_Msg(&pdTxMessageSummary,
                                           PE_SNK_READY, 
                                           PE_SEND_SOFT_RESET,
		                                   0,0);              
                break; 			
			}                    
        }    
    }
    return state;
}

/**
 * @brief  entry function of PE_SNK_STARTUP
 */
void Pe_Snk_Startup_Entry(void)
{
    pd_policy_engine_t* p = &gPdPolicyEngines[PORT_NUM];
    
    Pd_Init_Sink(PORT_NUM, p->isCc2Connect);//init pd hardware	
}

/**
 * @brief  run function of PE_SNK_STARTUP
 * @return const sm_state_t*, the new state
 */
const sm_state_t* Pe_Snk_Startup_Run(void)
{   
	return &gPdPolicies[PE_SNK_WAIT_FOR_CAPABILITIES];
}

/**
 * @brief  run function of PE_SNK_WAIT_FOR_CAPABILITIES
 * @return const sm_state_t*, the new state
 */
const sm_state_t* Pe_Snk_Wait_For_Capabilities_Run(void)
{
    pd_policy_engine_t* p = &gPdPolicyEngines[PORT_NUM];
	pe_state_e state = PE_SNK_WAIT_FOR_CAPABILITIES; 
    
    if (Pd_Is_Event_Happen(PD_RX_MESSAGE_SUCCESS_EVENT))
    {
        Pd_Clear_Event(PD_RX_MESSAGE_SUCCESS_EVENT);     
        state = PE_SNK_EVALUATE_CAPABILITY;        
    }
    return &gPdPolicies[state];
}

/**
 * @brief  run function of PE_SNK_EVALUATE_CAPABILITY
 * @return const sm_state_t*, the new state
 */
const sm_state_t* Pe_Snk_Evaluate_Capability_Run(void)
{
    pd_policy_engine_t* p = &gPdPolicyEngines[PORT_NUM];
	pe_state_e state = PE_SNK_EVALUATE_CAPABILITY; 
    
    if (PD_Source_Capabilities == p->pdRxMessageSummary->mesg_type)
    {   
        p->pdRxBuff = Pd_Get_Rx_Buff_Start_Address(false);
		if(0x1 == (((*(uint16_t*)p->pdRxBuff) >> 6) & 0x3))	 //pd2.0	
		{	
			Pd_Set_Pd_Rev(true);//set pd2.0		
		}
        uint16_t current = (*(uint32_t*)(p->pdRxBuff + 2)) & 0x3ff;	
		
		pd_mesg_summ_t pdTxMessageSummary;
		pdTxMessageSummary.packet_start = PD_SOP;
		pdTxMessageSummary.mesg_class = PD_DATA_CLASS;
		pdTxMessageSummary.mesg_type = PD_Request;
		uint8_t buff[4] = {0};
 		*(uint32_t*)buff = (1 << 28) | (current << 10) | current;  //request message
		state = Pd_Policy_Send_Msg(&pdTxMessageSummary,
								   PE_SNK_SELECT_CAPABILITY, 
								   PE_SEND_SOFT_RESET,
								   (uint8_t*)buff,1);  		       
    }
    else if (PD_EPR_Source_Capabilities == p->pdRxMessageSummary->mesg_type)
    {
		state = PE_SNK_HARD_RESET;      		
    }
    else
    {
        state = PE_SEND_SOFT_RESET;  
    } 
    return &gPdPolicies[state];
}

/**
 * @brief  entry function of PE_SNK_SELECT_CAPABILITY
 */
void Pe_Snk_Select_Capability_Entry(void)
{
    pd_policy_engine_t* p = &gPdPolicyEngines[PORT_NUM];

    Soft_Timer_Start(&p->genericTimer, SENDER_RESPONSE_TIME_REV3);  //restart timer
}

/**
 * @brief  run function of PE_SNK_SELECT_CAPABILITY
 * @return const sm_state_t*, the new state
 */
const sm_state_t* Pe_Snk_Select_Capability_Run(void)
{
    pd_policy_engine_t* p = &gPdPolicyEngines[PORT_NUM];
	pe_state_e state = PE_SNK_SELECT_CAPABILITY; 

    if (Pd_Is_Event_Happen(PD_RX_MESSAGE_SUCCESS_EVENT))
    {
        Pd_Clear_Event(PD_RX_MESSAGE_SUCCESS_EVENT); 
        Soft_Timer_Stop(&p->genericTimer);
        if (PD_Accept == p->pdRxMessageSummary->mesg_type)
        {
            state = PE_SNK_TRANSITION_SINK;   
        }
		else
		{
			state = PE_SNK_WAIT_FOR_CAPABILITIES;  
		}       
    }
    if (Soft_Timer_Is_Expired(&p->genericTimer))
    {
        Soft_Timer_Stop(&p->genericTimer);
        state = PE_SNK_HARD_RESET;   
    }
    return &gPdPolicies[state];
}

/**
 * @brief  entry function of PE_SNK_TRANSITION_SINK
 */
void Pe_Snk_Transition_Sink_Entry(void)
{
    pd_policy_engine_t* p = &gPdPolicyEngines[PORT_NUM];

    Soft_Timer_Start(&p->genericTimer, PS_TRANSITION_TIME);  //restart timer
}

/**
 * @brief  run function of PE_SNK_TRANSITION_SINK
 * @return const sm_state_t*, the new state
 */
const sm_state_t* Pe_Snk_Transition_Sink_Run(void)
{
    pd_policy_engine_t* p = &gPdPolicyEngines[PORT_NUM];
	pe_state_e state = PE_SNK_TRANSITION_SINK; 

    if (Pd_Is_Event_Happen(PD_RX_MESSAGE_SUCCESS_EVENT))
    {
        Pd_Clear_Event(PD_RX_MESSAGE_SUCCESS_EVENT); 
        Soft_Timer_Stop(&p->genericTimer);
        if (PD_PS_RDY == p->pdRxMessageSummary->mesg_type)
        {
            state = PE_SNK_READY;   
        }
        else
        {
            state = PE_SNK_HARD_RESET;           
        }        
    }
    if (Soft_Timer_Is_Expired(&p->genericTimer))
    {
        Soft_Timer_Stop(&p->genericTimer);
        state = PE_SNK_HARD_RESET;     
    }
    return &gPdPolicies[state];
}

/**
 * @brief  entry function of PE_SNK_READY
 */
void Pe_Snk_Ready_Entry(void)
{
    pd_policy_engine_t* p = &gPdPolicyEngines[PORT_NUM];

    Soft_Timer_Start(&p->genericTimer, SINK_WAIT_PR_SWAP);  //restart timer      
}

/**
 * @brief  run function of PE_SNK_READY
 * @return const sm_state_t*, the new state
 */
const sm_state_t* Pe_Snk_Ready_Run(void)
{
    pd_policy_engine_t* p = &gPdPolicyEngines[PORT_NUM];
	pe_state_e state = PE_SNK_READY; 

    if (Pd_Is_Event_Happen(PD_RX_MESSAGE_SUCCESS_EVENT))
    {
        Pd_Clear_Event(PD_RX_MESSAGE_SUCCESS_EVENT); 
        
        if (PD_EXTENDED_CLASS == p->pdRxMessageSummary->mesg_class)
        {
            // external message
            state = Process_Ext_Message_In_Pe_Snk_Ready(p->pdRxMessageSummary);
        }
        else if (PD_DATA_CLASS == p->pdRxMessageSummary->mesg_class)
        {
            // data message
            state = Process_Data_Message_In_Pe_Snk_Ready(p->pdRxMessageSummary);
        }
        else
        {
            // control message
            state = Process_Control_Message_In_Pe_Snk_Ready(p->pdRxMessageSummary);
        }
    }
    else
    {
		if (Soft_Timer_Is_Expired(&p->genericTimer))   //start pr swap
		{
			pd_mesg_summ_t pdTxMessageSummary;
			pdTxMessageSummary.packet_start = PD_SOP;
			pdTxMessageSummary.mesg_class = PD_CONTROL_CLASS;
			pdTxMessageSummary.mesg_type = PD_PR_Swap;	
			state = Pd_Policy_Send_Msg(&pdTxMessageSummary,
									   PE_REQUEST_PRS, 
									   PE_SEND_SOFT_RESET,
									   0,0);			      
		}
    }
    if (PE_SNK_READY != state)
    {        
        Soft_Timer_Stop(&p->genericTimer);   //stop timer 
    }
    return &gPdPolicies[state];
}

/**
 * @brief  run function of PE_SNK_HARD_RESET
 * @return const sm_state_t*, the new state
 */
const sm_state_t* Pe_Snk_Hard_Reset_Run(void)
{
    pd_policy_engine_t* p = &gPdPolicyEngines[PORT_NUM];
	
    Pd_Snk_Send_Hard_Reset();//tx hard reset	
    return &gPdPolicies[PE_SNK_TRANSITION_TO_DEFAULT];
}

/**
 * @brief  entry function of PE_SNK_TRANSITION_TO_DEFAULT
 */
void Pe_Snk_Transition_To_Default_Entry(void)
{
    pd_policy_engine_t* p = &gPdPolicyEngines[PORT_NUM];

    Soft_Timer_Start(&p->genericTimer, SENDER_RESPONSE_TIME_REV3);  //restart timer
}

/**
 * @brief  run function of PE_SNK_TRANSITION_TO_DEFAULT
 * @return const sm_state_t*, the new state
 */
const sm_state_t* Pe_Snk_Transition_To_Default_Run(void)
{
    pd_policy_engine_t* p = &gPdPolicyEngines[PORT_NUM];
	pe_state_e state = PE_SNK_TRANSITION_TO_DEFAULT; 

    if (Soft_Timer_Is_Expired(&p->genericTimer))
    {
        Soft_Timer_Stop(&p->genericTimer);
        state = PE_SNK_STARTUP;     
    }
    return &gPdPolicies[state];
}


