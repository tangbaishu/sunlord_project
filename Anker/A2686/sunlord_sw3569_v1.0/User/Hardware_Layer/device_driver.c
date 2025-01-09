#include "device_driver.h"
#include "base_timer_driver.h"
#define TIMEMARKMASK			0x7FFFFFFF				// 32bit无符号整型最大值

#define SystemTimeCounter		Base_Timer_1ms_Count	// 时基累加值

static void time_slice_example(void)
{
	static unsigned int time_slice=0;
	Base_Timer_Driver_Init(1);	// 初始化时基定时器
	if(time_slice == 0)
	{
		time_slice = My_GetSystemTimeMark();	// 获取前一时刻
	}
	if(My_GetSystemTimePass(time_slice) >= 500)
	{
		// 超时
	}
}

/**
 * @brief 获取系统走过的时间
 * 计算公式：系统耗时 = 当前时刻 - 前一时刻 （内部存在 计时器溢出逻辑）
 * @param tm 通过 My_GetSystemTimeMark()获取的前一时刻
 * @return unsigned int 
 */
unsigned int My_GetSystemTimePass(unsigned int tm)
{
	unsigned int otm = (TIMEMARKMASK & SystemTimeCounter);
	if(otm > tm)
		return (otm-tm);
	else	// 若计时器溢出
		return (TIMEMARKMASK&((TIMEMARKMASK & ~tm) + 1 + otm));
}

unsigned int My_GetSystemTimeMark(void)
{
	return (TIMEMARKMASK & SystemTimeCounter);
}


