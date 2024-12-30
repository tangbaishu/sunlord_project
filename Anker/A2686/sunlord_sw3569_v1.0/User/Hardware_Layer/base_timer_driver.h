#ifndef _BASE_TIME_DRIVER_H_
#define _BASE_TIME_DRIVER_H_

#include "stdint.h"

extern uint32_t Base_Timer_1ms_Count;

void Start_Noload_Timer(uint16_t reload_value);
void Stop_Noload_Timer(void);
void Base_Timer_Driver_Check(void);

void Base_Timer_Driver_Init(unsigned int timer_ms);

void Base_Timer_Driver_DeInit(void);

#endif
