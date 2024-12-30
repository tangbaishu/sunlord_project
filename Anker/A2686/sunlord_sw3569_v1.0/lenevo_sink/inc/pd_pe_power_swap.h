/****************************************************************************
 * @copyright Copyright(C) 2020-2022 Ismartware Limited. All rights reserved.
 * @file pd_pe_power_swap.h
 * @brief  pd policy engine power role swap interface
 * @author SD1 team
 ****************************************************************************/
#ifndef PD_PE_POWER_SWAP_H
#define PD_PE_POWER_SWAP_H

#include "state_machine.h"
#include "pd_policy_engine.h"

#ifdef __cplusplus
extern "C" {
#endif
  
// request pr_swap
void Pe_Request_Prs_Entry(void);
const sm_state_t* Pe_Request_Prs_Run(void);  
// snk primary response pr_swap
void Pe_Snk_Response_Prs_Primary_Entry(void);
const sm_state_t* Pe_Snk_Response_Prs_Primary_Run(void);    
// snk secondary response pr_swap
const sm_state_t* Pe_Snk_Response_Prs_Secondary_Run(void); 

#ifdef __cplusplus
}
#endif

#endif        // end of PD_PE_POWER_SWAP_H
