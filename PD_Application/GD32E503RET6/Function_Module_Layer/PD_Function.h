#ifndef _PD_FUNCTION_H_
#define _PD_FUNCTION_H_

#include "PD_Protocol/sw3516p_driver.h"

typedef enum
{
	EMPTY = 0,									// 无任何插入
	PROT1_USB_A,								// 端口1 USB-A 插入
	PORT1_USB_C,								// 端口1 USB-C 插入
	PORT2_USB_A,								// 端口2 USB-A 插入
	PORT2_USB_C									// 端口2 USB-C 插入
}PD_Input_Port_e;							// 板端插入端口信息

typedef struct
{
	uint16 Port1_Output_Power;
	uint16 Port2_Output_Power;
}PD_Output_Power_t;								// PD输出功率


typedef struct
{
	SW3516P_Driver_t		*PD_Protocol_IC;	// PD协议IC驱动
	PD_Input_Port_e 		Board_Input_Port;	// 板端插入端口信息
	PD_Output_Power_t		PD_Output_Power;	// PD输出功率
}PD_Function_t;									// PD功能信息
extern PD_Function_t PD_Function;

void Board_Port_Detection(void);				// 板载端口检测
// 板载输出
// 板载异常处理
// 


#endif
