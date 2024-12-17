#ifndef _LOG_FUNCTION_H_
#define _LOG_FUNCTION_H_

#include "system.h"

#define LOG_PRINTF
#define LOG_ABNORMAL

void LOG_Abnormal_Occurred_Event(u32 pending_flag);
void LOG_Abnormal_Release_Event(u32 pending_flag);

#endif
