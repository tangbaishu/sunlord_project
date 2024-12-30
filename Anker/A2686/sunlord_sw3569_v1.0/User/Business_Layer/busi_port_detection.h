/**
 * @file busi_port_detection.h	端口检测业务
 * @author 	tong.libo@sunlord.com.cn
 * @brief 
 * @version 0.1
 * @date 2024-12-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _BUSI_PORT_DETECTION_H_
#define _BUSI_PORT_DETECTION_H_

#include "system.h"
#include "port_data_library.h"
#include "busi_fast_charge_config.h"
#include "func_hardware_init.h"
#include "api.h"

typedef struct
{
	bool						Port_State_Updata;			// 1: 状态更新 0：状态未更新
	Port_BSP_Data_t				*P_Port_BSP_Data;				// 所有端口板端数据
	Port_Out_Protocol_Data_t	Port1_Out_Protocol_Data;	
	Port_Out_Protocol_Data_t	*Port2_Out_Protocol_Data;
	Port_RealTime_Out_Power_t	Port1_RealTime_Out_Power;
	Port_RealTime_Out_Power_t	*Port2_RealTime_Out_Power;
}Port_Detection_Data_t;
extern Port_Detection_Data_t	Busi_Port_Detection_Data; 


typedef Port_Link_State_e 			(*P_Port_Link_Detection_Func) (void);	
typedef Port_Out_Protocol_Data_t	(*P_Port_Out_Protocol_Func)	(void);		
typedef Port_RealTime_Out_Power_t 	(*P_Port1_RealTime_Out_Power_Func) (void);

typedef struct
{
	P_Port_Link_Detection_Func 		Port1_Link_Detection_Func;	// api.h 接口自带相关函数
	P_Port_Link_Detection_Func 		Port2_Link_Detection_Func;
	P_Port_Out_Protocol_Func   		Port1_Out_Protocol_Func;	// api.h 接口自带相关函数
	P_Port_Out_Protocol_Func		Port2_Out_Protocol_Func;
	P_Port1_RealTime_Out_Power_Func	Port1_RealTime_Out_Power_Func;	// api.h 接口自带相关函数
	P_Port1_RealTime_Out_Power_Func	Port2_RealTime_Out_Power_Func;
}Busi_Port_Detection_Func_t;
extern Busi_Port_Detection_Func_t Busi_Port_Detection_Func;

void Busi_Port_Detection_Init(void);
void Get_All_Port_Link_State(void);

#endif
