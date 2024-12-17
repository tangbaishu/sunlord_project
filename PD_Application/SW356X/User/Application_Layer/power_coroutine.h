/**
 * @file power_coroutine.h
 * @author tong.libo@sunlord.com.cn
 * @brief 电源模块例程
 * 例程清单：
 * 电源供应改变例程		void Power_Supply_Charge_Routine(void)
 * 
 * @version 0.1
 * @date 2024-12-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _POWER_COROUTINE_H_
#define _POWER_COROUTINE_H_

/**
 * @brief 电源供应改变例程
 * 基于端口插入逻辑&事件，设定/切换输出功率值 并实时获取输出功率值
 */
void Power_Supply_Charge_Routine(void);



#endif
