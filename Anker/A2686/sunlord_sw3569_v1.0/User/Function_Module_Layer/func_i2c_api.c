#include "func_i2c_api.h"
#include "i2cm_async_driver.h"
#include "i2c_master_driver.h"
#include "i2c_slave_driver.h"
#include "device_driver.h"
#include "zr_systick.h"
#include "api.h"
#include "system.h"

static void Func_I2C_Loop(I2C_API_Data_t *api_data);
static bool Func_I2C_Write_Data(I2C_API_Data_t *api_data, uint8_t reg, uint8_t *p_wdata, uint8_t write_data_len);
static bool Func_I2C_Read_Data(I2C_API_Data_t *api_data, uint8_t reg, uint8_t read_data_len);
static void Func_I2C_Attach_Slave(I2C_API_Data_t *api_data, uint8_t device_addr, uint8_t overtime_seting);
extern void Func_I2C_Slave_Int_Dispose(void);

void Func_I2C_API_Example(I2C_Driver_Type_e driver_type)
{
	static I2C_Driver_API_t I2C_Driver_API;
	static bool read_en=1;
	static uint8_t write_data = 70;
	static uint32_t i2c_bus_time=0;
	// Func_I2C_Driver_Init(I2C_MASTER_ASYNC, 50, &I2C_Driver_API);
	I2C_Driver_API.I2C_Driver_Data.Driver_Type = I2C_MASTER_ASYNC;
    Func_I2C_Driver_Init(I2C_Driver_API.I2C_Driver_Data.Driver_Type, 50, &I2C_Driver_API);
	Systick_Delay_Ms(400);
	i2c_bus_time = My_GetSystemTimeMark();
	while(1)
	{
		I2C_Driver_API.P_Func_Async_Thread(&I2C_Driver_API.I2C_Driver_Data);
		// Func_I2C_Loop(&I2C_Driver_API.I2C_Driver_Data);
		if(My_GetSystemTimePass(i2c_bus_time) > 250)
		{
			i2c_bus_time = My_GetSystemTimeMark(); 
			if(I2C_Driver_API.I2C_Driver_Data.Driver_State.I2C_Running.Running == false)
			{
				if(read_en)
				{
					read_en = 0;
					// Func_I2C_Read_Data(&I2C_Driver_API.I2C_Driver_Data, 0x01, 1);
					I2C_Driver_API.P_Func_Read_Data(&I2C_Driver_API.I2C_Driver_Data, 0x01, 1);
					MY_PRINTF("i2c_read\r\n");
				}
				else
				{
					if(I2C_Driver_API.I2C_Driver_Data.Driver_State.Reg_Read_Finish == true)
					{
						MY_PRINTF("i2cm async read data:\r\n");
						// for(uint8_t num=0; num < sizeof(write_data); num++)
						// {
							MY_PRINTF("read_data[0] = %#x\r\n", I2C_Driver_API.I2C_Driver_Data.I2C_Read_Buffer.Read_Buffer[0]);

						// }
						I2C_Driver_API.I2C_Driver_Data.Driver_State.Allow_Read_Write = true;
						I2C_Driver_API.I2C_Driver_Data.Driver_State.Reg_Read_Finish = false;
					}
					read_en = 1;
					// Func_I2C_Write_Data(&I2C_Driver_API.I2C_Driver_Data, SET_OUTPUT_POWER, &write_data, 1);
					I2C_Driver_API.P_Func_Write_Data(&I2C_Driver_API.I2C_Driver_Data, SET_OUTPUT_POWER, &write_data, 1);
					MY_PRINTF("I2C_Write\r\n");
				}

			}
			else
			{
				MY_PRINTF("i2c_busy\r\n");
			}
		}
	}
	
}


void Func_I2C_Driver_Init (I2C_Driver_Type_e driver_type, uint8_t freq_khz, I2C_Driver_API_t *I2C_Driver_API)
{
	if( I2C_SLAVE == driver_type )
	{
		I2C_Driver_API->P_Func_I2c_Slave_Int_Dispose = Func_I2C_Slave_Int_Dispose;
		I2c_Slave_Driver_Init(I2C_Driver_API->P_Func_I2c_Slave_Int_Dispose, SLAVE_1_DEV_ADDR);
		return;
	}
	else if( I2C_MASTER_ASYNC == driver_type )
	{
		I2CM_Async_Hardware_Init(freq_khz);
		
	}
	I2C_Driver_API->P_Func_Write_Data = Func_I2C_Write_Data;
	I2C_Driver_API->P_Func_Read_Data = Func_I2C_Read_Data;
	I2C_Driver_API->P_Func_Attach_Slave = Func_I2C_Attach_Slave;
	I2C_Driver_API->P_Func_Async_Thread	= Func_I2C_Loop;
	Func_I2C_Attach_Slave(&I2C_Driver_API->I2C_Driver_Data, SLAVE_1_DEV_ADDR,50);
	// I2C_Driver_API->P_Func_Attach_Slave(&I2C_Driver_API->I2C_Driver_Data, SLAVE_1_DEV_ADDR,50);		
	I2C_Driver_API->I2C_Driver_Data.I2C_Read_Buffer.Read_Buffer = \
			((Func_I2CM_Async_Data_t *)(I2C_Driver_API->I2C_Driver_Data.Driver_Support))->I2CS_Device.buffer;
	I2C_Driver_API->I2C_Driver_Data.I2C_Read_Buffer.Buffer_Length = \
			((Func_I2CM_Async_Data_t *)(I2C_Driver_API->I2C_Driver_Data.Driver_Support))->I2CS_Device.bufferLen;
}

static void Func_I2C_Loop(I2C_API_Data_t *api_data)
{
	uint8_t return_value;
	if(true == api_data->Driver_State.I2C_Running.Running)	// 若处于运行状态
	{
		return_value = 	I2cm_Async_Get_And_Clear_Finished(&((Func_I2CM_Async_Data_t *)(api_data->Driver_Support))->I2CS_Device);
		if( return_value == I2CM_RESULT_SUCCESS || return_value == I2CM_RESULT_FAILURE )
		{
			if(true == api_data->Driver_State.I2C_Running.Write_Running)
			{
				api_data->Driver_State.I2C_Running.Running = false;
				api_data->Driver_State.I2C_Running.Write_Running = false;
				api_data->Driver_State.Allow_Read_Write = true;
				return;
			}

			if(true == api_data->Driver_State.I2C_Running.Read_Running)
			{
				api_data->Driver_State.I2C_Running.Running = false;
				api_data->Driver_State.I2C_Running.Read_Running = false;
				api_data->Driver_State.Reg_Read_Finish = true;
				return;
			}
		}
		I2cm_Async_Run();

		if( I2CM_Async_Driver_Overtime( ((Func_I2CM_Async_Data_t *)(api_data->Driver_Support))->I2CM_Async_RAM,\
								 ((Func_I2CM_Async_Data_t *)(api_data->Driver_Support))->I2CM_Async_RAM.OverTime_Threshold ) )
		{
			#if defined(DEBUG_PRINTF_OPEN)
				// 超时
				#if 1
					MY_PRINTF("i2cm_async transfer overtime\r\n");
				#endif
			#endif
		}
	}
}

static bool Func_I2C_Write_Data(I2C_API_Data_t *api_data, uint8_t reg, uint8_t *p_wdata, uint8_t write_data_len)
{
	if(GET_I2C_BUS_LEISUER(api_data))
	// if( (api_data->Driver_State.I2C_Running.Running == false) &&\
	// 		(api_data->Driver_State.Allow_Read_Write == true) )
	{
		api_data->Driver_State.I2C_Running.Running = true;
		api_data->Driver_State.I2C_Running.Write_Running = true;
		if( I2C_MASTER_ASYNC == api_data->Driver_Type)
		{
			I2CM_Async_Driver_Write((Func_I2CM_Async_Data_t *)(api_data->Driver_Support), reg, p_wdata, write_data_len);
		}
		api_data->Driver_State.Allow_Read_Write = false;
		return SUCCESS;
	}
	return ERROR;
}

static bool Func_I2C_Read_Data(I2C_API_Data_t *api_data, uint8_t reg, uint8_t read_data_len)
{
	if(GET_I2C_BUS_LEISUER(api_data))
	// if (	(api_data->Driver_State.I2C_Running.Read_Running == false) &&\
	// 		(api_data->Driver_State.Allow_Read_Write == true) )
	{
		api_data->Driver_State.I2C_Running.Running = true;
		api_data->Driver_State.I2C_Running.Read_Running = true;
		if( I2C_MASTER_ASYNC == api_data->Driver_Type)
		{
			Func_I2CM_Async_Driver_Read((Func_I2CM_Async_Data_t *)(api_data->Driver_Support), reg, read_data_len);
		}
		api_data->Driver_State.Allow_Read_Write = false;
		return SUCCESS;
	}
	return ERROR;
}

static void Async_Attach_Slave(I2C_API_Data_t *api_data, uint8_t device_addr, uint8_t overtime_seting)
{
	static Func_I2CM_Async_Data_t i2cm_async_data;
	i2cm_async_data.P_On_Begin_Event_Fun = NULL;
	i2cm_async_data.P_On_End_Event_Fun = NULL;
	I2CM_Async_Driver_Attach_Slave((Func_I2CM_Async_Data_t*)&i2cm_async_data, device_addr, (uint8_t*)i2cm_async_data.Buffer_Area, IIC_BUFFER_AREA_LEN, overtime_seting);
	api_data->Driver_Support = (uint32_t*) &i2cm_async_data;
	// api_data->Read_Buffer.Read_Data = i2cm_async_data.Buffer_Area;
} 

static void Func_I2C_Attach_Slave(I2C_API_Data_t *api_data, uint8_t device_addr, uint8_t overtime_seting)
{
	if(I2C_MASTER_ASYNC == api_data->Driver_Type)
	{
		Async_Attach_Slave(api_data, device_addr, overtime_seting);
		api_data->Driver_State.Allow_Read_Write = true;
	}
}

// void Func_I2C_Slave_Resp_Read(uint8_t register)
// {
	
// }

