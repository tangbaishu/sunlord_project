/**
 * @file power_business.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-12-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef _POWER_BUSINESS_H_
#define _POWER_BUSINESS_H_

#include "config.h"				// 快充配置结构体
#include "port_module.h"

typedef struct
{
	Port_Module_Data_t *P_Port_Scan_Func_Data;
}Port_info_t;

typedef  struct
{
	config_data_t*				P_Init_Config;
	fast_charge_protocol_e		Port1_Current_FC_Protocol;						// 端口1当前协议
	fast_charge_protocol_e		Port2_Current_FC_Protocol;						// 端口2当前协议
	Port_Module_Data_t*			P_Port_Module_data;
}Power_Business_t;
extern Power_Business_t Power_Business_Data;

void Power_Business_Init(void);

void Port_Scan(void);

#endif
