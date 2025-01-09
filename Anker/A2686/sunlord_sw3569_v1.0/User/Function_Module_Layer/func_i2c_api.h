#ifndef _FUNC_I2C_API_H_
#define _FUNC_I2C_API_H_

#include "system.h"
#include "i2c_protocol_library.h"
#include "i2c_slave_driver.h"
#include "zr_i2c.h"

typedef enum
{
	BUS_LEISURE,
	BUS_BUSY,
}I2C_State_e;

typedef enum
{
	I2C_MASTER_ASYNC,
	I2C_MASTER,
	I2C_SLAVE,
}I2C_Driver_Type_e;

typedef struct
{
	bool		Running;							// I2C处于运行状态
	bool		Write_Running;						// I2C写 true:传输中、false:传输完成/未开始传输
	bool		Read_Running;						// I2C读 true:传输中、false:传输完成/未开始传输
}I2C_Running_State_t;


typedef struct
{
	bool				Allow_Read_Write;			// 允许读写进程
	bool				Reg_Read_Finish;			// I2C总线寄存器读取已完成,
													// 该位置1后，需软件获取完Read_Data数据后；
													// 同时对 Allow_Read_Write 和 Reg_Read_Finish 清零
	I2C_Running_State_t	I2C_Running;
}I2C_Driver_State_t;

typedef struct
{
	uint8_t		*Read_Buffer;
	uint8_t		Buffer_Length;
}I2C_Read_Buffer_t;


typedef struct
{
	I2C_Driver_Type_e	Driver_Type;
	uint32_t 			*Driver_Support;			// 驱动支持变量，指向对应驱动必要的变量
	I2C_Read_Buffer_t  	I2C_Read_Buffer;
	I2C_Driver_State_t	Driver_State;
	uint8_t				Overtime_Seting;
}I2C_API_Data_t;

// 获取总线忙碌状态
#define GET_I2C_BUS_LEISUER(api_data)					( (api_data->Driver_State.I2C_Running.Running == false) &&\
														(api_data->Driver_State.Allow_Read_Write == true) == 1 )

typedef struct
{
	I2C_API_Data_t					I2C_Driver_Data;	//	无需自己赋值，由驱动初始化生成
	bool (*P_Func_Write_Data)		(I2C_API_Data_t *api_data, uint8_t reg, uint8_t *p_wdata, uint8_t write_data_len);		// 1: success , 0: i2c busy
	bool (*P_Func_Read_Data)		(I2C_API_Data_t *api_data, uint8_t reg, uint8_t read_data_len);
	void (*P_Func_Attach_Slave)		(I2C_API_Data_t *api_data, uint8_t device_addr, uint8_t overtime_seting);
	void (*P_Func_Async_Thread)		(I2C_API_Data_t *api_data);		//	用于维持 异步I2C功能正常运行的线程，需要放置在主循环中
	P_I2c_Slave_Int_Dispose			P_Func_I2c_Slave_Int_Dispose;
}I2C_Driver_API_t;

void Func_I2C_Driver_Init (I2C_Driver_Type_e driver_type, uint8_t freq_khz, I2C_Driver_API_t *I2C_Driver_API);

void Func_I2C_API_Example(I2C_Driver_Type_e driver_type);

#endif
