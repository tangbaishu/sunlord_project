#ifndef _BASE_TIME_DRIVER_H_
#define _BASE_TIME_DRIVER_H_

#include "stdint.h"
extern uint32_t sys_start, sys_time, time_finish, timer_stop;
void Start_Noload_Timer(uint16_t reload_value);
void Stop_Noload_Timer(void);
void Base_Time_Driver_Check(void);

void Base_Time_Driver_Init(unsigned int reload_value);

void Base_Time_Driver_DeInit(void);

#endif
