/**
 * @file port_module.h
 * @author tong.libo@sunlord.com.cn
 * @brief 端口扫描功能
 * 基于 CONFIG_PORT_MODE 选项，扫描端口1(PORT_1)、端口2(PORT_2)是否存在插入/拔出事件、基于不同事件切换匹配的输出功率
 * @version 0.1
 * @date 2024-12-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _PORT_MODULE_H_
#define _PORT_MODULE_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief 声明快充端口组合
 * 仅支持下拉组合
 * 声明值须于 port_mode_e 保持一致
*/
#define SET_SINGLE_A_MODE		0   //!< single A mode
#define SET_SINGLE_C_MODE		1   //!< single C mode
#define SET_DUO_AA_MODE			2  	//!< A + A mode
#define SET_DUO_CA_MODE			3  	//!< C + A mode
#define SET_DUO_CC_MODE			4   //!< C + C mode
#define CONFIG_PORT_MODE		SET_DUO_CC_MODE

typedef enum
{
	PORT_DETACH = 0,
	PORT_ATTACH
}Port_Insert_e;

typedef enum
{
	POWER_SUPPLY_MODE_A = 0,
	POWER_SUPPLY_MODE_B
}Power_Supply_Mode_e;

typedef struct
{
	bool 			State_Change 	: 1;	// TRUE or FALSE
	Port_Insert_e	Current_State 		: 1;
}Port_Insert_State_t;

typedef struct
{
	Port_Insert_State_t	Port1_Insert_State;										// 
	Port_Insert_State_t	Port2_Insert_State;										//
	void (*Power_Supply_Select) (Power_Supply_Mode_e mode, bool right_away);	// 函数功能：在识别到不同类型端口或同时插入多个端口时，切换不同功率
}Port_Module_Data_t;
extern	Port_Module_Data_t	Port_Module_Data;

void Port_Scan_Func_Init(void);

#if CONFIG_PORT_MODE != SET_DUO_CC_MODE
	void Port_Scan_Func_Logic(void);
#endif

#endif
