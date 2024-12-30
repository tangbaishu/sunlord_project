/****************************************************************************
 * @copyright Copyright(C) 2020-2022 Ismartware Limited. All rights reserved.
 * @file pd_pe_power_swap.c
 * @brief  pd policy engine power role swap implementation
 * @author 005
 ****************************************************************************/
#include "pd_pe_power_swap.h"
#include "zr_gpio.h"
#include "zr_adc.h"

/////////////////////////////////////////    Timer //////////////////////////////////
// tPSSourceOff the spr range is 750-920 ms, typic.835
#define SINK_WAIT_SPR_SOURCE_OFF_TIME (835)

// global pd policy states
extern const sm_state_t gPdPolicies[PE_STATE_NUM];
extern pd_policy_engine_t gPdPolicyEngines[1];

/**
 * @brief  entry function of PE_REQUEST_PRS
 */
void Pe_Request_Prs_Entry(void)
{
    pd_policy_engine_t* p = &gPdPolicyEngines[PORT_NUM];  
    
    Soft_Timer_Start(&p->genericTimer, SENDER_RESPONSE_TIME_REV3);  //start timer
}

/**
 * @brief  run function of PE_REQUEST_PRS
 * @return const sm_state_t*, the new state
 */
const sm_state_t* Pe_Request_Prs_Run(void)
{
    pd_policy_engine_t* p = &gPdPolicyEngines[PORT_NUM];
	pe_state_e state = PE_REQUEST_PRS; 

    if (Pd_Is_Event_Happen(PD_RX_MESSAGE_SUCCESS_EVENT))
    {
        Pd_Clear_Event(PD_RX_MESSAGE_SUCCESS_EVENT); 
        Soft_Timer_Stop(&p->genericTimer);

        if (PD_Accept == p->pdRxMessageSummary->mesg_type)
        {        			
            state = PE_SNK_RESPONSE_PRS_PRIMARY;              
        }
        else
        {
            state = PE_SNK_READY;  
        }   
    }
    if (Soft_Timer_Is_Expired(&p->genericTimer))
    {
        Soft_Timer_Stop(&p->genericTimer);
        state = PE_SNK_READY;    
    }    
    return &gPdPolicies[state];   
}

/**
 * @brief  entry function of PE_SNK_RESPONSE_PRS_PRIMARY
 */
void Pe_Snk_Response_Prs_Primary_Entry(void)
{
    pd_policy_engine_t* p = &gPdPolicyEngines[PORT_NUM];

    Soft_Timer_Start(&p->genericTimer, SINK_WAIT_SPR_SOURCE_OFF_TIME);  //equivalent PSSourceOffTimer	
}

/**
 * @brief  run function of PE_SNK_RESPONSE_PRS_PRIMARY
 * @return const sm_state_t*, the new state
 */
const sm_state_t* Pe_Snk_Response_Prs_Primary_Run(void)
{
    pd_policy_engine_t* p = &gPdPolicyEngines[PORT_NUM];
	pe_state_e state = PE_SNK_RESPONSE_PRS_PRIMARY; 

    if (Pd_Is_Event_Happen(PD_RX_MESSAGE_SUCCESS_EVENT))
    {
        Pd_Clear_Event(PD_RX_MESSAGE_SUCCESS_EVENT);        

        if (PD_PS_RDY == p->pdRxMessageSummary->mesg_type)
        {		
            Soft_Timer_Stop(&p->genericTimer);			
			Gpio_Set_Output_Value(GPIO8, false);  //close rd
			Adc_Stop_Comparator(ADC_CHL_2);
			Adc_Stop_Comparator(ADC_CHL_3);	
			Port_Reconnect(PORT_NUM);	//apply rp
			Pd_Set_Power_Role(true);//set to src			
            state = PE_SNK_RESPONSE_PRS_SECONDARY; 
        }  
    }
    if (Soft_Timer_Is_Expired(&p->genericTimer))
    {
		state = PE_DISABLED;      		        
    }    
    return &gPdPolicies[state];    
}

/**
 * @brief  run function of PE_SNK_RESPONSE_PRS_SECONDARY
 * @return const sm_state_t*, the new state
 */
const sm_state_t* Pe_Snk_Response_Prs_Secondary_Run(void)
{
    pd_policy_engine_t* p = &gPdPolicyEngines[PORT_NUM];
	pe_state_e state = PE_SNK_RESPONSE_PRS_SECONDARY; 
	
    if (Pd_Is_Prs_And_Apply_5V_Complete()) //pr swap to src complete,must caal this func to check whether complete
    {	
		pd_mesg_summ_t pdTxMessageSummary;
		pdTxMessageSummary.packet_start = PD_SOP;
		pdTxMessageSummary.mesg_class = PD_CONTROL_CLASS;
		pdTxMessageSummary.mesg_type = PD_PS_RDY;	
		state = Pd_Policy_Send_Msg(&pdTxMessageSummary,
								   PE_SRC_READY, 
								   PE_DISABLED,
								   0,0);		      
    }    
    return &gPdPolicies[state];
}

