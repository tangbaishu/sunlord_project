
/****************************************************************************
 * @copyright Copyright(C) 2020-2022 Ismartware Limited. All rights reserved.
 * @file state_machine.h
 * @brief  Generic statemachine interface
 * @author SD1 team
 ****************************************************************************/
#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "system.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sm_state sm_state_t;

/// Pointer functions for entry of state
typedef void (*entry_func)(void);
/// Pointer functions for run of state
typedef const sm_state_t* (*run_func)(void);
/// Pointer functions for switch of state
typedef void (*state_switch_func)(const sm_state_t* self);

/**
 * @brief The generic state definition
 * entryFunc is the first call when enter a new state
 * exitFunc is the last call when exit a old state
 * To saving memory, the exit will not used, the exit feature shall be done by run_func
 * runFun is shall be scheduled to implement the logic of state
 */
struct sm_state
{
    entry_func entryFunc;        //!< entry funcation, set to 0 if not needed
    run_func runFunc;            //!< run funcation, shall not be NULL
};

/**
 * @brief statemachine structure
 */
typedef struct
{
    const sm_state_t* currentState;        //!< the current state
    state_switch_func stateSwitching;      //!< the state switching function, it is optional
	bool isInEntry;                        //!< whether current is in entry function
} state_machine_t;

/**
 * @brief  Initialize a statemachine, the memory of statemachine is allcoated at customer
 * @param[in]  self state machine
 * @param[in]  currState the initial state
 * @param[in]  switchFunc the switchFun is to know which state is it, it could set to 0 if don't needed
 */
void State_Machine_Init(state_machine_t* self, const sm_state_t* currState, state_switch_func switchFunc);

/**
 * @brief  Run a statemachine, schedule
 * @param[in]  self state machine
 */
void State_Machine_Run(state_machine_t* self);

/**
 * @brief  Force to jump to a new state
 * @param[in]  self state machine
 * @param[in]  newState the new state
 */
void State_Machine_Set_State(state_machine_t* self, const sm_state_t* newState);

#ifdef __cplusplus
}
#endif

#endif        // end of STATE_MACHINE_H
