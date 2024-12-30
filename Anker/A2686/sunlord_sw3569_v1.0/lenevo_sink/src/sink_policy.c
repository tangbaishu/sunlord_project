/****************************************************************************
 * @copyright Copyright(C) 2020-2022 Ismartware Limited. All rights reserved.
 * @file sink policy.c
 * @brief  The sink policy
 * @author SD1 team
 ****************************************************************************/
#include "soft_timer.h"
#include "state_machine.h"
#include "sink_policy.h"
#include "pd_policy_engine.h"
#include "zr_rcc.h"
#include "zr_gpio.h"
#include "zr_adc.h"

// clang-format off
//when adc compara iterrupt,call this func
static void Adc_Comparator_Callback(adc_channel_e channel);
// clang-format on

// clang-format off
//sink state switching
static void Sink_State_Switching(const sm_state_t* self);
// idle
static void Snk_Idle_State_Entry(void);
static const sm_state_t* Snk_Idle_State_Run(void);
// unattach sink
static const sm_state_t* Unattached_Snk_State_Run(void);
// attachwait sink .wait src in
static const sm_state_t* Attachwait_Snk_State_Run(void);
// attach sink, src in
static void Attached_Snk_State_Entry(void);
static const sm_state_t* Attached_Snk_State_Run(void);
// unattach src
static void Unattached_Src_State_Entry(void);
static const sm_state_t* Unattached_Src_State_Run(void);
// attach src, sink in
static void Attached_Src_State_Entry(void);
static const sm_state_t* Attached_Src_State_Run(void);

// global user pd policy engines
const sm_state_t gSinkPolicies[SINK_STATE_NUM] = {
    [SNK_IDLE_STATE]                               = {Snk_Idle_State_Entry, Snk_Idle_State_Run},
    
    [UNATTACHED_SNK_STATE]                         = {0, Unattached_Snk_State_Run},
    [ATTACHWAIT_SNK_STATE]                         = {0, Attachwait_Snk_State_Run},
    [ATTACHED_SNK_STATE]                           = {Attached_Snk_State_Entry, Attached_Snk_State_Run}, 
	
    [UNATTACHED_SRC_STATE]                         = {Unattached_Src_State_Entry, Unattached_Src_State_Run},
    [ATTACHED_SRC_STATE]                           = {Attached_Src_State_Entry, Attached_Src_State_Run}, 
};
// clang-format on

/**
 *  typec port definition
 */
typedef struct sink_policy
{
    state_machine_t sm;                         //!< sink state machine
//    state_machine_t pdSinkSm;                 //!< pd sink state machine
	sink_state_e currentState;                  //!< the current procssing state, readonly
	
	soft_timer_t sinkPolicyTimer;               //!< sink policy timer
		
	uint8_t ccCheckCnt[2];	
	bool isSinkConnect : 1;
	bool isCc2Connect : 1;
    bool isHadTrySink :1;
    bool isInPrswap :1;    
}sink_policy_t;

static sink_policy_t gSink[1] = {{0}};
extern pd_policy_engine_t gPdPolicyEngines[1];

/**
 * @brief  initialize sink policy
 */
void Sink_Policy_Init(void)
{
	sink_policy_t* p = (sink_policy_t*)(&gSink[PORT_NUM]);
	
	p->ccCheckCnt[0] = 0;
	p->ccCheckCnt[1] = 0;
	p->isSinkConnect = false;
	p->isCc2Connect = false;
    p->isHadTrySink = false;
    p->isInPrswap = false;    
	
	Adc_Register_Comparator_Callback(Adc_Comparator_Callback);
	State_Machine_Init(&p->sm, &gSinkPolicies[SNK_IDLE_STATE], Sink_State_Switching);
	
	Soft_Timer_Init();
}

/**
 * @brief  sink policy run 
 */
void Sink_Policy_Run(void)
{
	sink_policy_t* p = (sink_policy_t*)(&gSink[PORT_NUM]);
	State_Machine_Run(&p->sm);	
	if (p->sm.isInEntry || p->currentState < UNATTACHED_SRC_STATE)
	{
		Low_Power_Not_Sleep();  //no sleep in this main loop
	}	
}

/**
 * @brief  adc compara irq callback
 * @param[in]  channel, irq adc channel
 */
static void Adc_Comparator_Callback(adc_channel_e channel)
{
    if (channel < ADC_CHL_2)
    {
        return;
    }
	sink_policy_t* p = (sink_policy_t*)(&gSink[PORT_NUM]);
	uint8_t channelArray[2] = {ADC_CHL_2, ADC_CHL_3};
	uint8_t channelIndex = channel - ADC_CHL_2;
		
	Adc_Stop_Comparator(channelArray[channelIndex]);
	p->ccCheckCnt[channelIndex]++;  //20ms check debounce
	if (p->ccCheckCnt[channelIndex] > 15)  //cc vol met for 15 times
	{
		p->ccCheckCnt[channelIndex] = 0;
		p->isSinkConnect = !p->isSinkConnect;  //sink connect or disconnect
		p->isCc2Connect = (bool)channelIndex;  //set cc for pd comunicate
	}
	adc_cmp_param_t param;
	param.mode = ADC_CMP_TRIGGER_MODE_INTERRUPT;
	param.threshold = 300/0.8;		
	param.op = p->isSinkConnect ? ADC_CMP_OP_LESS : ADC_CMP_OP_GREATER_OR_EQU;  //sink connect,check vol less,
	Adc_Start_Comparator(channelArray[channelIndex], &param);  //restart compara check			
}

/**
 * @brief  state machine switching callback
 * @note   it is for print current state or tell outside which step is it
 * @param[in]  self the current state
 */
static void Sink_State_Switching(const sm_state_t* self)
{
    sink_policy_t* p = (sink_policy_t*)(&gSink[PORT_NUM]);
	
    sink_state_e state = (sink_state_e)((self - gSinkPolicies));
    p->currentState = state;
}

/**
 * @brief  entry function of SNK_IDLE_STATE
 */
static void Snk_Idle_State_Entry(void)
{
#define VIN_POWER_ON_WAIT  4 
	sink_policy_t* p = (sink_policy_t*)(&gSink[PORT_NUM]);
	Soft_Timer_Start(&p->sinkPolicyTimer, VIN_POWER_ON_WAIT);
}

/**
 * @brief  run function of SNK_IDLE_STATE
 */
static const sm_state_t* Snk_Idle_State_Run(void)
{
	sink_policy_t* p = (sink_policy_t*)(&gSink[PORT_NUM]);
	sink_state_e state = SNK_IDLE_STATE;
	
	if (Soft_Timer_Is_Expired(&p->sinkPolicyTimer))  //when vin power-on first time,wait until basic project init hardware complete
	{
		Soft_Timer_Stop(&p->sinkPolicyTimer);

		Rcc_Module_Clk_Enable(GPIO_ID);
		Gpio_Function_Select(GPIO8, GPIO8_FUNC_SEL_GPIO);
		Gpio_Direction_Cfg(GPIO8, GPIO_DIRECTION_OUTPUT);
		Gpio_Set_Output_Value(GPIO8, false);  //close rd,dp1

		Gpio_Function_Select(GPIO3, GPIO3_FUNC_SEL_ADC);
		Gpio_Function_Select(GPIO6, GPIO6_FUNC_SEL_ADC);
		Gpio_Pulldown_Pullup_Cfg(GPIO6, PULLDOWN_DIS, PULLUP_DIS);	//gpio6/7 default pullup,close pullup	
		Rcc_Module_Clk_Enable(ADC_ID);
		adc_config_t adcCfg;
		Adc_Get_Config(&adcCfg);
		adcCfg.adc2Source = ADC2_SIG_SRC_GPIO3;  //check cc1
		adcCfg.adc3Source = ADC3_SIG_SRC_GPIO6;  //check cc2
		Adc_Set_Config(&adcCfg);
//		Gpio_Function_Select(GPIO2, GPIO2_FUNC_SEL_ADC);		
//		Gpio_Function_Select(GPIO3, GPIO3_FUNC_SEL_ADC);		
//		Rcc_Module_Clk_Enable(ADC_ID);
//		adc_config_t adcCfg;
//		Adc_Get_Config(&adcCfg);
//		adcCfg.adc2Source = ADC2_SIG_SRC_GPIO2;  //check cc1
//		adcCfg.adc3Source = ADC3_SIG_SRC_GPIO3;  //check cc2
//		Adc_Set_Config(&adcCfg);
		
		Adc_Stop_Comparator(ADC_CHL_2);
		Adc_Stop_Comparator(ADC_CHL_3);		
		Adc_Start_Adc_Channel(ADC_CHL_2);
		Adc_Start_Adc_Channel(ADC_CHL_3);
		Adc_Enabled();
	
        state = UNATTACHED_SRC_STATE;			//default as src
	}	
	return &gSinkPolicies[state];
}

/**
 * @brief  run function of UNATTACHED_SNK_STATE
 */	
static const sm_state_t* Unattached_Snk_State_Run(void)
{
	sink_policy_t* p = (sink_policy_t*)(&gSink[PORT_NUM]);
 	
    Port_Force_Disconnect(PORT_NUM);  //remove rp	
	Gpio_Set_Output_Value(GPIO8, true);  //open rd
		
	p->ccCheckCnt[0] = 0;
	p->ccCheckCnt[1] = 0;
	p->isSinkConnect = false;  
	
	//start cc vol check
	adc_cmp_param_t param;
	param.mode = ADC_CMP_TRIGGER_MODE_INTERRUPT;
	param.op = ADC_CMP_OP_GREATER_OR_EQU;
	param.threshold = 300/0.8;  //0.3v threshold
	Adc_Start_Comparator(ADC_CHL_2, &param);  //start compara and enable irq
	Adc_Start_Comparator(ADC_CHL_3, &param);	
    
#define TRY_SINK_TIME  1000 
	Soft_Timer_Start(&p->sinkPolicyTimer, TRY_SINK_TIME);       
	
	return &gSinkPolicies[ATTACHWAIT_SNK_STATE];
}

/**
 * @brief  run function of ATTACHWAIT_SNK_STATE
 */	
static const sm_state_t* Attachwait_Snk_State_Run(void)
{
	sink_policy_t* p = (sink_policy_t*)(&gSink[PORT_NUM]);
	sink_state_e state = ATTACHWAIT_SNK_STATE;
    
	if (Soft_Timer_Is_Expired(&p->sinkPolicyTimer)) 
	{        
        p->isHadTrySink = true;
		state = UNATTACHED_SRC_STATE;  //try sink fail,as src
	}    
	
	if (p->isSinkConnect)  //if need,can multi check vbus adc vol
	{
		state = ATTACHED_SNK_STATE;	  //src in
	}
	
    if (ATTACHWAIT_SNK_STATE != state)
    {
        Soft_Timer_Stop(&p->sinkPolicyTimer);
    }
    
	return &gSinkPolicies[state];
}

/**
 * @brief  entry function of ATTACHED_SNK_STATE
 */
static void Attached_Snk_State_Entry(void)
{
#define SINK_TO_SRC_DELAY_TIME  (SINK_WAIT_PR_SWAP << 1) 
	sink_policy_t* p = (sink_policy_t*)(&gSink[PORT_NUM]);

	Soft_Timer_Start(&p->sinkPolicyTimer, SINK_TO_SRC_DELAY_TIME);
	Pd_Policy_Engine_Init(p->isCc2Connect);  //init pd policy  
    
    p->isInPrswap = false;
}

/**
 * @brief  run function of ATTACHED_SNK_STATE
 */
static const sm_state_t* Attached_Snk_State_Run(void)
{
	sink_policy_t* p = (sink_policy_t*)(&gSink[PORT_NUM]);
	sink_state_e state = ATTACHED_SNK_STATE;
    	
	if (Pd_Sink_Policy_Run())//pd sink policy
	{
        Adc_Stop_Comparator(ADC_CHL_2);
        Adc_Stop_Comparator(ADC_CHL_3);	        
		state = ATTACHED_SRC_STATE;  //pr swap to src complete
	}
    
    if (!p->isInPrswap)
    {
        if (PE_SNK_RESPONSE_PRS_PRIMARY == gPdPolicyEngines[0].currentState)
        {
            p->isInPrswap = true;
        }
    }
	if (!p->isSinkConnect && !p->isInPrswap)  //have prswap process, ignore src remove
	{        
		state = UNATTACHED_SRC_STATE;	  //src remove		
	}      
	
	if (Soft_Timer_Is_Expired(&p->sinkPolicyTimer)) 
	{
        p->isHadTrySink = true;
		state = UNATTACHED_SRC_STATE;  //time out,still in this state,force to apply rp
	}
	
    if (ATTACHED_SNK_STATE != state)
    {
        Soft_Timer_Stop(&p->sinkPolicyTimer);
    }
    
	return &gSinkPolicies[state];
}

/**
 * @brief  entry function of UNATTACHED_SRC_STATE
 */
static void Unattached_Src_State_Entry(void)
{
	sink_policy_t* p = (sink_policy_t*)(&gSink[PORT_NUM]);
	
	Gpio_Set_Output_Value(GPIO8, false);  //close rd
	Adc_Stop_Comparator(ADC_CHL_2);
	Adc_Stop_Comparator(ADC_CHL_3);	
    Port_Reconnect(PORT_NUM);	//apply rp
    
#define WAIT_SINK_TIME  400
    if (p->isHadTrySink) 
    {
        Soft_Timer_Start(&p->sinkPolicyTimer, WAIT_SINK_TIME);        
    }          
}

/**
 * @brief  run function of UNATTACHED_SRC_STATE
 */
static const sm_state_t* Unattached_Src_State_Run(void)
{
	sink_policy_t* p = (sink_policy_t*)(&gSink[PORT_NUM]);
	sink_state_e state = UNATTACHED_SRC_STATE;
    
	if (Soft_Timer_Is_Expired(&p->sinkPolicyTimer)) 
	{
        p->isHadTrySink = false;
	}    
	
	if (Device_Get_Port_State()) //sink in
	{
		state = p->isHadTrySink ? ATTACHED_SRC_STATE : UNATTACHED_SNK_STATE;   //had try sink fail,to as src,other to try sink
	}
    
    if (UNATTACHED_SRC_STATE != state)
    {
        Soft_Timer_Stop(&p->sinkPolicyTimer);
    }
	
	return &gSinkPolicies[state];
}

/**
 * @brief  entry function of ATTACHED_SRC_STATE
 */
static void Attached_Src_State_Entry(void)
{
	sink_policy_t* p = (sink_policy_t*)(&gSink[PORT_NUM]);
    p->isHadTrySink = false;
}

/**
 * @brief  run function of ATTACHED_SRC_STATE
 */
static const sm_state_t* Attached_Src_State_Run(void)
{
	sink_policy_t* p = (sink_policy_t*)(&gSink[PORT_NUM]);
	sink_state_e state = ATTACHED_SRC_STATE;
	
	if (!Device_Get_Port_State())
	{
		state = UNATTACHED_SRC_STATE;  //sink remove	
	}
	
	return &gSinkPolicies[state];
}

