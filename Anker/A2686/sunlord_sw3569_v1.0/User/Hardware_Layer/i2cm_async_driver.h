#ifndef _I2CM_ASYNC_DRIVER_H_
#define _I2CM_ASYNC_DRIVER_H_

#include "system.h"
#include "i2cm_async.h"
#include "hardware_config.h"

typedef struct
{
	uint32_t			Transfer_Time_Slice;	// 数据开始传输时间片
	uint16_t			OverTime_Threshold;		// 超时阈值	单位：@SystemTimeCounter 对应的时间单位
}I2CM_Async_RAM_t;								// 异步i2c 外设运行必要变量数据

typedef struct
{
	uint8_t				Buffer_Area[IIC_BUFFER_AREA_LEN];
	i2cs_device_t		I2CS_Device;		// i2cs_device_t 异步i2c 主机驱动数据
	I2CM_Async_RAM_t	I2CM_Async_RAM;			// 异步i2c 外设运行必要变量数据
	on_begin_event		P_On_Begin_Event_Fun;
	on_end_event		P_On_End_Event_Fun;
}Func_I2CM_Async_Data_t;

/**
 * @brief init i2cm async
 * @param[in] i2c_clock the frequency of I2c master, unit : 1KHz  e.g. 100 -> 100KHz
 */
void I2CM_Async_Hardware_Init(uint8_t i2c_clock);

void Func_I2CM_Async_Example(void);

void I2CM_Async_Driver_Example(void);

/**
 * @brief 异步I2C变量数据初始化
 * 在硬件初始化之后，再进一步对异步I2C变量进行初始化
 * @param ram_data 		
 * @param device_addr 			设备寄存器地址
 * @param buffer_area 			写入/读取 数据缓存区 映射地址
 * @param buffer_len 			写入/读取 数据缓存区 长度
 * @param overtime_threshold 	超时阈值
 */
void I2CM_Async_Driver_Attach_Slave(Func_I2CM_Async_Data_t *ram_data, uint8_t device_addr, uint8_t *buffer_area, uint8_t buffer_len, uint32_t overtime_threshold);

// /**
//  * @brief 异步I2C数据传输超时检测
//  * 
//  * @param ram_data 
//  * @param over_threshold 超时阈值
//  * @return true 	超时
//  * @return false 	未超时
//  */
// bool I2CM_Async_Driver_Overtime(I2CM_Async_RAM_t ram_data, uint32_t over_threshold);

// 异步I2C 写数据
bool I2CM_Async_Driver_Write(Func_I2CM_Async_Data_t *ram_data, uint8_t reg, uint8_t *p_wdata, uint8_t wdata_len);

// 异步I2C 读数据
bool Func_I2CM_Async_Driver_Read(Func_I2CM_Async_Data_t *ram_data, uint8_t reg, uint8_t rdata_len);

bool I2CM_Async_Driver_Overtime(I2CM_Async_RAM_t ram_data, uint32_t over_threshold);
#endif
