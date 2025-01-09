#ifndef _BUSI_SHARE_MEMORY_H_
#define _BUSI_SHARE_MEMORY_H_

#include "i2c_protocol_library.h"
#include "hardware_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)
	typedef union
	{
		uint8_t Memory_Block[18];
		struct 
		{
			uint8_t		Keep_Working;				// addr0		0：当前循环后，进入睡眠状态、1：保持工作状态
			uint8_t 	Port_State;					// addr1		0：端口未插入，1：端口插入
			uint8_t		Port_Protocol;				// addr2		端口输出协议，@
			uint8_t		Port_Seting_Power;			// addr3		端口设定输出功率，单位：W
			uint8_t		Port_Rebroadcast_PDO;		// addr4		端口重新广播PDO
			uint16_t	Port_NTC_Res;				// addr5~6		端口NTC电阻值，单位：KΩ
			uint16_t	Port_Real_Voltage;			// addr7~8		单位：mV
			uint16_t	Port_Protocol_Voltage;		// addr9~10		单位：mV
			uint16_t	Port_Protocol_Current;		// addr11~12		单位：mA,保留
			uint16_t	Port_Real_Power;			// addr13~14	单位：mW
			uint16_t	Port_Real_Current;			// addr15~16	单位：mA
			uint8_t		CMD_Byte;					// addr17
													// 0x01:DEVICE_INTI 0x03:GET_SUMMARY 
													// DEVICE_INTI 设备初始化，保留
													// REBROADCAST_PDO 从机重新广播PDO
		};
	}I2C_SW3566_Reg_Data_t;		// I2C寄存器数据存储地址，注：排列顺序需严格与 I2C_SW3566_Reg_Addr_e 寄存器地址表保持一致
#pragma pack()

extern I2C_SW3566_Reg_Data_t I2C_SW3566_Reg_Data[SLAVE_SUM+1];  // 从机设备数量 + 主机

#ifdef __cplusplus
}
#endif

#endif

