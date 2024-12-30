/****************************************************************************
 * @copyright Copyright(C) 2020-2022 Ismartware Limited. All rights reserved.
 * @file mutil port policy.c
 * @brief  The mutil port policy
 * @author 005
 ****************************************************************************/
#include "soft_timer.h"
#include "state_machine.h"
#include "mutil_port_pd_policy.h"
#include "mutil_port_policy.h"

//state switching
static void Port_Policy_State_Switching(const sm_state_t* self);
// idle
static void Port_Policy_Idle_Entry(void);
static const sm_state_t* Port_Policy_Idle_Run(void);
// ready
static const sm_state_t* Port_Policy_Ready_Run(void);
// error recover
static void Port_Policy_Error_Recover_Entry(void);
static const sm_state_t* Port_Policy_Error_Recover_Run(void);


// global user pd policy engines
const sm_state_t gPortPolicies[PORT_POLICY_STATE_NUM] = {
    [PORT_POLICY_IDLE]                             = {Port_Policy_Idle_Entry, Port_Policy_Idle_Run},
    [PORT_POLICY_READY]                            = {0, Port_Policy_Ready_Run},    
    [PORT_POLICY_ERROR_RECOVER]                    = {Port_Policy_Error_Recover_Entry, Port_Policy_Error_Recover_Run},
};
// clang-format on

/**
 *  typec port definition
 */
typedef struct port_policy
{
    state_machine_t sm;                         //!< state machine
    pd_policy_engine_t* pdPolicy;               // pointer to pd policy
	port_policy_state_e currentState;           //!< the current procssing state, readonly	
	soft_timer_t portPolicyTimer;               //!< policy timer  
}port_policy_t;

static port_policy_t gPortPolicy[1] = {{0}};
extern pd_policy_engine_t gPdPolicyEngines[1];

//////////////////////////////////port policy layer//////////////////////////////////////////////
/**
 * @brief  state machine switching callback
 * @note   it is for print current state or tell outside which step is it
 * @param[in]  self the current state
 * @param[in]  data the data of state machine, it is set when statemachine initialized
 */
static void Port_Policy_State_Switching(const sm_state_t* self)
{
    port_policy_t* p = (port_policy_t*)(&gPortPolicy[0]);
    port_policy_state_e state = (port_policy_state_e)((self - gPortPolicies));

    p->currentState = state;
}


/**
 * @brief  entry function of PORT_POLICY_IDLE
 */
static void Port_Policy_Idle_Entry(void)
{
    port_policy_t* p = (port_policy_t*)(&gPortPolicy[0]);
   
    Soft_Timer_Stop(&p->portPolicyTimer);  
    
    Mutil_Port_Pd_Policy_Init();
}

/**
 * @brief  run function of PORT_POLICY_IDLE
 */
static const sm_state_t* Port_Policy_Idle_Run(void)
{
    port_policy_state_e state = PORT_POLICY_IDLE;
    if (Device_Get_Port_State())
    {
        state = PORT_POLICY_READY;
    }
    return &gPortPolicies[state]; 
}

/**
 * @brief  run function of PORT_POLICY_READY
 */
static const sm_state_t* Port_Policy_Ready_Run(void)
{
    port_policy_t* p = (port_policy_t*)(&gPortPolicy[0]);
    port_policy_state_e state = PORT_POLICY_READY;
    
    if (p->pdPolicy->isMisMatch)
    {
        p->pdPolicy->isMisMatch = false;
        //according to the actual power managed situacation to decide whether to Rebroadcast src cap  
    }
    
    if (p->pdPolicy->isSharedTestModeEnter)
    {
        p->pdPolicy->isSharedTestModeEnter = false;
        p->pdPolicy->isInSharedTestMode = true;  //the action after enter SharedTestMode just Rebroadcast max power src cap?whether need others action?
        //Rebroadcast max power src cap  
    }
    
    if (p->pdPolicy->isSharedTestModeExit)
    {
        p->pdPolicy->isSharedTestModeExit = false;
        if (p->pdPolicy->isInSharedTestMode)
        {
            state = PORT_POLICY_ERROR_RECOVER;
        }
    }
    
    if (!Device_Get_Port_State())
    {
        state = PORT_POLICY_IDLE;
    }
    
    return &gPortPolicies[state]; 
}

/**
 * @brief  entry function of PORT_POLICY_ERROR_RECOVER
 */
static void Port_Policy_Error_Recover_Entry(void)
{
    port_policy_t* p = (port_policy_t*)(&gPortPolicy[0]);
    
    Soft_Timer_Start(&p->portPolicyTimer, 1000);
    Port_Force_Disconnect(0);  //force disconnect typec for 1s
}

/**
 * @brief  run function of PORT_POLICY_ERROR_RECOVER
 */
static const sm_state_t* Port_Policy_Error_Recover_Run(void)
{
    port_policy_t* p = (port_policy_t*)(&gPortPolicy[0]);    
    port_policy_state_e state = PORT_POLICY_ERROR_RECOVER;
    if (Soft_Timer_Is_Expired(&p->portPolicyTimer))
    {
        Soft_Timer_Stop(&p->portPolicyTimer); 
        Port_Reconnect(0);   //recover typec
        state = PORT_POLICY_IDLE;        
    }
    return &gPortPolicies[state]; 
}
//////////////////////////////////end//////////////////////////////////////////////

//////////////////////////////////called by main.c//////////////////////////////////////////////
/**
 * @brief  initialize mutil port policy,it will be called by main.c when init
 */
void Mutil_Port_Policy_Init(void)
{
	port_policy_t* p = (port_policy_t*)(&gPortPolicy[0]);
    p->pdPolicy = (pd_policy_engine_t*)(&gPdPolicyEngines[0]);
	Soft_Timer_Init();  
	State_Machine_Init(&p->sm, &gPortPolicies[PORT_POLICY_IDLE], Port_Policy_State_Switching);
}

/**
 * @brief  mutil port policy run ,it will be called in main.c's loop
 */
void Mutil_Port_Policy_Run(void)
{
    port_policy_t* p = (port_policy_t*)(&gPortPolicy[0]);
    
    State_Machine_Run(&p->sm);
}
//////////////////////////////////end//////////////////////////////////////////////

