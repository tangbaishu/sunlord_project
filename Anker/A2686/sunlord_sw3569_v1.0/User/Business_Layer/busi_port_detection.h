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
#include "func_power_alloc.h"
#include "func_hardware_api.h"
#include "busi_share_memory.h"
#include "sunlord_api.h"
#include "api.h"

typedef struct
{
	I2C_SW3566_Reg_Data_t		*Self_Reg_Data;
	uint8_t						Previous_Port_State;		
	bool						Port_Updata;				
}Port_Detection_Data_t;
extern Port_Detection_Data_t	Busi_Port_Detection_Data; 

typedef struct
{
	struct SW_API_Func_t		*P_SW_API_Func;
	uint16_t (*P_Get_Out_Voltage)	(void);
	uint16_t (*P_Get_Out_Current)	(uint8_t port);
}Busi_Port_Detection_Func_t;
extern Busi_Port_Detection_Func_t 	Busi_Port_Detection_Func;

void Busi_Port_Detection_Init(void);
void Busi_Port_Detection(void);


#endif
