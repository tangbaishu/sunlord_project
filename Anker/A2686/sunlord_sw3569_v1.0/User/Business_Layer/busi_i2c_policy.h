/**
 * @file busi_i2c_bus.h
 * @author tong.libo@sunlord.com.cn
 * @brief I2C总线数据传输业务
 * 
 * @version 0.1
 * @date 2024-12-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _BUSI_I2C_BUS_H_
#define _BUSI_I2C_BUS_H_

#include "hardware_config.h"
#include "busi_share_memory.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	PROCESS_READ_REG,			// 读取从机寄存器数据
	PROCESS_READ_RGE_FINSIH,	// 读取从机寄存器完成
	PROCESS_SETING_POWER,		// 设置从机输出功率
	PROCESS_REBROADCAST_PDO,	// 从机重新广播PDO
}Busi_I2C_Process_e;

typedef struct
{
	uint8_t	Slave_Output_Power;	// 从机输出功率值
	uint8_t	Rebroadcast_PDO_En;	// 使能重新关播PDO
}Busi_I2C_Policy_Data_t;
extern Busi_I2C_Policy_Data_t	Busi_I2C_Policy_Data;


Busi_I2C_Process_e Busi_I2C_Traverse_Reg(void);
Busi_I2C_Process_e Busi_I2C_Policy(Busi_I2C_Policy_Data_t *policy_data);

#ifdef __cplusplus
}
#endif

#endif // !_BUSI_I2C_BUS_H_
