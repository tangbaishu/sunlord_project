/****************************************************************************
 * @copyright Copyright(C) 2020-2022 Ismartware Limited. All rights reserved.
 * @file pd_pe_sink.h
 * @brief  pd policy engine sink interface
 * @author 005
 ****************************************************************************/
#ifndef PD_PE_SINK_H
#define PD_PE_SINK_H

#include "state_machine.h"
#include "pd_policy_engine.h"

#ifdef __cplusplus
extern "C" {
#endif

// start up
void Pe_Snk_Startup_Entry(void);
const sm_state_t* Pe_Snk_Startup_Run(void);
// wait for capabilities
const sm_state_t* Pe_Snk_Wait_For_Capabilities_Run(void);
// PE_SNK_EVALUATE_CAPABILITY
const sm_state_t* Pe_Snk_Evaluate_Capability_Run(void);
// PE_SNK_SELECT_CAPABILITY
void Pe_Snk_Select_Capability_Entry(void);
const sm_state_t* Pe_Snk_Select_Capability_Run(void);
// PE_SNK_TRANSITION_SINK
void Pe_Snk_Transition_Sink_Entry(void);
const sm_state_t* Pe_Snk_Transition_Sink_Run(void);
// PE_SNK_READY
void Pe_Snk_Ready_Entry(void);
const sm_state_t* Pe_Snk_Ready_Run(void);
// PE_SNK_HARD_RESET
const sm_state_t* Pe_Snk_Hard_Reset_Run(void);
// PE_SNK_TRANSITION_TO_DEFAULT
void Pe_Snk_Transition_To_Default_Entry(void);
const sm_state_t* Pe_Snk_Transition_To_Default_Run(void);
#ifdef __cplusplus
}
#endif

#endif        // end of PD_PE_SINK_H
