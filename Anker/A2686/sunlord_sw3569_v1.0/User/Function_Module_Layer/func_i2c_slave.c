#include "func_i2c_slave.h"
#include "i2c_protocol_library.h"
#include "i2c_slave_driver.h"
#include "zr_i2c.h"
#include "api.h"

Func_I2C_Slave_Data_t	Func_I2C_Slave_Data;

/**
 * @brief 响应主机数据读取请求
 * 
 */
static void Func_I2CS_Response_Read(void);

/**
 * @brief 处理主机发送命令
 * 
 */
static void Func_I2CS_Rx_CMD_Dispose(void);

void Func_I2CS_Dispose(I2CS_Int_Event event)
{
	if(MASTER_READ == event)
	{
		*Func_I2C_Slave_Data.I2C_Running_State = BUSY_STATE;
	}
	else if(MASTER_WRITE == event)
	{
		*Func_I2C_Slave_Data.I2C_Running_State = BUSY_STATE;
	}
}

/**
 * @brief 响应主机数据读取请求
 * 
 */
static void Func_I2CS_Response_Read(void)
{
	uint8_t visit_reg = 0; // 主机请求响应寄存器
	visit_reg = I2c_Slave_Get_Register_Address();
	if(PORT2_OUT_PROTOCOL == visit_reg)
	{
		I2c_Slave_Write_TX_Data((u8*)Func_I2C_Slave_Data.Port2_Out_Protocol_Data, sizeof(Func_I2C_Slave_Data.Port2_Out_Protocol_Data));
	}
	else if(PORT2_REALTIME_POWER == visit_reg)
	{
		I2c_Slave_Write_TX_Data((u8*)Func_I2C_Slave_Data.Port2_RealTime_Out_Power, sizeof(Func_I2C_Slave_Data.Port2_RealTime_Out_Power));
	}
	else if(PORT2_NTC_TEMP_DATA == visit_reg)
	{
		I2c_Slave_Write_TX_Data((u8*)Func_I2C_Slave_Data.NTC_Temp_Data, sizeof(Func_I2C_Slave_Data.NTC_Temp_Data));
	}
}

/**
 * @brief 处理主机发送命令
 * 
 */
static void Func_I2CS_Rx_CMD_Dispose(void)
{
	uint8_t visit_reg = 0, rx_length; // 主机请求响应寄存器
	visit_reg = I2c_Slave_Get_Register_Address();
	if(SLAVE_ROUSE == visit_reg)	// 唤醒
	{
		*Func_I2C_Slave_Data.I2C_Running_State = BUSY_STATE;
		// Low_Power_Not_Sleep();
	}
	else if(SLAVE_SLEEP == visit_reg)	// 睡眠
	{
		*Func_I2C_Slave_Data.I2C_Running_State = LEISURE_STATE;
	}
	else if(PORT2_CTL_OUT_POWER == visit_reg)
	{
		rx_length = I2c_Slave_Get_RX_Byte_Length();
		I2c_Slave_Get_RX_Data(Func_I2C_Slave_Data.Port2_Seting_Output_Power, rx_length);
	}

}
