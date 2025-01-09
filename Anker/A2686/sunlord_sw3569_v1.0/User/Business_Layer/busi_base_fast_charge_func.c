#include "busi_base_fast_charge_func.h"
#include "func_hardware_api.h"
#include "busi_port_detection.h"

#if defined(DEBUG_PRINTF_OPEN)
    #include "stdio.h"
    #define LOG "busi_b_f_c_f: "
#endif


// 基于客户需求，对快充模块完成初始化配置
void Busi_Fast_Charge_Driver_Init (void)
{
	Busi_Port_Detection_Init();
}

void Busi_LowPower_OnBefore(void)			// 进入低功耗之前的处理函数
{
	#if defined(DEBUG_PRINTF_OPEN)
	printf(LOG"Busi_LowPower_OnBefore()\r\n");
	#endif
}

void Busi_LowPower_OnAfter(void)			// 从低功耗唤醒后的处理函数
{
	#if defined(DEBUG_PRINTF_OPEN)
	printf(LOG"Busi_LowPower_OnAfter()\r\n");
	#endif
}
