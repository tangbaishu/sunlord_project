/**
 * @file i2c_Protocol_library.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-12-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _I2C_PROTOCOL_LIBRARY_H_
#define _I2C_PROTOCOL_LIBRARY_H_
/*
 *	I2C数据传输协议: 寄存器地址 + 数据
 *		固定协议长度：10
 *		头：固定为：0x55
 *		地址：目标设备地址 主机地址：0x00, 从机地址：0x01 ~ 0x80, 广播地址：0xFF
 *		命令：设备状态请求: 0x01, 设备状态应答：0x02, 设备状态同步：0x03, 设备功能执行：0x04, 设置设备参数：0x05
 *
 *		目前的长度固定为10个，不够的地方用0填充。此处主要方便后期扩展。
 *		用户应设置超时接收命令，开始收到数据后启动定时器，超过100ms,未检测到结束，则恢复初始化。
 *		用户应设置超时应答命令，发出消息后50ms未收到应答，应启动重传机制，至多重传一次，设备未应答的按之前的状态。
 *		数据：
 * 			I2C数据传输业务：
 * 				0x00: 设备持续保持工作状态
 * 				0x01：设备执行系统初始化
 * 				0x02: 设备执行重新广播PDO(SRC_CAP)
 * 				0x03：设备执行自定义功能 		
 * 				0x04: 获取设备摘要信息,信息主体：端口插拔状态、端口NTC数据
 * 				0x05：获取设备输出端口状态(按照设备映射端口编号,从小到大应答)
 * 				0x06：获取设备输出端口协议
 * 				0x07: 获取设备母线电压
 * 				0x08：获取设备Vbus电压
 * 				0x09：获取设备输出电流
 * 				0x0A: 获取设备输出功率
 * 				0x0B: 获取设备NTC温度(具体定义属性：具体温度值/过温标志/正常温度标志)
 *  * 			0x0C：设置设备输出功率
 * 				0x0D：设置设备母线调压
 * 				0xFF：设备进入睡眠状态
 * 		8位校验和：
 * 				前9位数据之和，取低8位
 * 	举例：
 * 		主机获取设备1端口状态：
 * 			主机发送数据：0x55、0x01、0x01、0x04、0x00、0x00、0x00、0x00、0x00、0x5B
 * 		从机应答主机端口状态:
 * 			从机应答数据: 0x55、0x00、0x02、0x04、0x01、0xFF、0x02、0x00、0x00、0x5D	// 解析：应答设备输出状态信息：端口1插入、端口2拔出
 * 		
 * 		
 * 
 */
#include "system.h"

#define TX_PACK_LENGTH			10		// 包长度
#define RX_PACK_LENGTH			24		// 包长度

typedef enum							// 包数据字段对应的业务信息
{								
	//Memory_Block 存储区
	KEEP_WORKING 		= 0x00,			// 当数据为0时，保持睡眠状态。当数据为1时，保持工作状态
	PORT_STATE			= 0x01,			// 摘要起始字节 1
	PORT_PROTOCOL		= 0x02,			// 摘要字节 3
	SET_OUTPUT_POWER	= 0x03,			// 摘要字节 4
	REBROADCAST_PDO		= 0x04,			// 摘要字节 5
	NTC_DATA			= 0x05,			// 摘要字节 2
	REAL_VOLTAGE		= 0x07,			// 摘要字节 6
	PROTOCOL_VOLTAGE	= 0x09,
	PROTOCOL_CURRENT	= 0x0B,
	REAL_POWER			= 0x0D,
	REAL_CURRENT		= 0x0F,
	// GET_VIN_VOLTAGE,					
	// SET_VIN_VALUE,
	CMD_BYTE			= 0x11,			// 0x01:DEVICE_INTI 0x03:GET_SUMMARY 
										// DEVICE_INTI 设备初始化，保留
										// GET_SUMMARY 获取从机摘要信息,对应区域 0x01~0x06
}I2C_SW3566_Reg_Addr_e;



typedef struct
{
	I2C_SW3566_Reg_Addr_e 	Reg_Addr;
	uint8_t 				Tx_Buffer[TX_PACK_LENGTH];
	uint8_t 				Rx_Buffer[RX_PACK_LENGTH];
}I2C_Protocol_Pack_t;


#endif
