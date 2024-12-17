#ifndef _PORT_SCAN_FUNCTION_H_
#define _PORT_SCAN_FUNCTION_H_

#include <stdint.h>

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
#define CONFIG_PORT_MODE		SET_DUO_CA_MODE

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
	Port_Insert_e	Port1_Insert_State;
	Port_Insert_e	Port2_Insert_State;

	void (*Power_Supply_Select) (Power_Supply_Mode_e mode);		// 函数功能：在识别到不同类型端口或同时插入多个端口时，切换不同功率
}Port_Scan_Func_Data_t;
extern	Port_Scan_Func_Data_t	Port_Scan_Func_Data;

void Port_Scan_Func_Init(void);

#endif
