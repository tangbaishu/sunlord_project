
/****************************************************************************
 * @copyright Copyright(C) 2020-2022 Ismartware Limited. All rights reserved.
 * @file state_machine.c
 * @brief  The generic statemachine implementation
 * @author SD1 team
 ****************************************************************************/
#include "state_machine.h"

/**
 * @brief  Initialize a statemachine, the memory of statemachine is allcoated at customer
 * @param[in]  self state machine
 * @param[in]  currState the initial state
 * @param[in]  switchFunc the switchFun is to know which state is it, it could set to 0 if don't needed
 */
void State_Machine_Init(state_machine_t* self, const sm_state_t* currState, state_switch_func switchFunc)
{
    self->currentState = 0;
    self->stateSwitching = switchFunc;
	self->isInEntry = true;

    State_Machine_Set_State(self, currState);
}

/**
 * @brief  run a statemachine
 * @param[in]  self state machine
 */
void State_Machine_Run(state_machine_t* self)
{
	self->isInEntry = false;
    if (self->currentState->runFunc)
    {
        const sm_state_t* newState = self->currentState->runFunc();
        if (newState && (newState != self->currentState))
        {
			self->isInEntry = true;
            State_Machine_Set_State(self, newState);
        }
    }
}

/**
 * @brief  set the new state
 * @param[in]  self state machine
 * @param[in]  newState the new state
 */
void State_Machine_Set_State(state_machine_t* self, const sm_state_t* newState)
{
    if (!newState || (newState == self->currentState))
    {
        return;
    }

    self->currentState = newState;

    // log
    if (self->stateSwitching)
    {
        self->stateSwitching(self->currentState);
    }

    // run the entry function
    if (self->currentState->entryFunc)
    {
        self->currentState->entryFunc();
    }
}
