#ifndef _APP_SYS_CTL_H_
#define _APP_SYS_CTL_H_

#include "system.h"

typedef enum
{
	DRIVER_POLICY = 0,
	FAST_CHARGE,
}APP_List_e;		// APP应用列表

typedef struct
{
	APP_List_e		APP_Process;	// 应用进程
}APP_SYS_Ctl_t;					// 系统控制APP应用

void Slave_State_Machine(void);

#endif