#include "busi_i2c_bus.h"
#include "func_hardware_init.h"
#if (DEVICE_TYPE == SLAVE_DEVICE)
	#include "func_i2c_slave.h"
#endif

#if defined(DEBUG_PRINTF_OPEN)
    #include "stdio.h"
    #define LOG "busi_i_b: "
#endif

I2C_Transfer_Data_t Busi_I2C_Bus_Data;

void Busi_I2C_Init(void)
{
	#if (DEVICE_TYPE == SLAVE_DEVICE)
		Func_I2C_Slave_Data.Port2_Out_Protocol_Data = &Busi_I2C_Bus_Data.Port2_Out_Protocol_Data;
		Func_I2C_Slave_Data.Port2_RealTime_Out_Power = &Busi_I2C_Bus_Data.Port2_RealTime_Out_Power;
		Func_I2C_Slave_Data.Port2_Seting_Output_Power = &Busi_I2C_Bus_Data.Port2_Seting_Output_Power;
		Func_I2C_Slave_Data.NTC_Temp_Data = &Busi_I2C_Bus_Data.NTC_Temp_Data;
		Func_I2C_Slave_Data.I2C_Running_State = (uint8_t *)&Busi_I2C_Bus_Data.I2C_Running_State;
	#endif
}

void Busi_I2C_Bus(void)
{
	if(*Busi_I2C_Bus_Data.I2C_Running_State == BUSY_STATE)
	{
		Low_Power_Not_Sleep();
	}
}

#if (DEVICE_TYPE == HOST_DEVICE)
	void Busi_I2CM_Read_Slave_Register(I2C_Register_List_e register_adr)
	{
		I2CM_Transfer_Info_t transfer_info;
		transfer_info.Wdata_Len = 0;
		if(PORT2_OUT_PROTOCOL == register_adr)
		{
			transfer_info.Reg_Addr = PORT2_OUT_PROTOCOL;
			transfer_info.Rdata_Len = sizeof(Busi_I2C_Bus_Data.Port2_Out_Protocol_Data);
			transfer_info.P_Rdata = (uint8_t *)&Busi_I2C_Bus_Data.Port2_Out_Protocol_Data;
			#if defined(DEBUG_PRINTF_OPEN)
				printf("PORT2_OUT_PROTOCOL = %d, %d, %d,%d, %d\r\n", (uint8_t)Busi_I2C_Bus_Data.Port2_Out_Protocol_Data.Port_Link_State,\
									(uint8_t)Busi_I2C_Bus_Data.Port2_Out_Protocol_Data.Protocol_Type,\
									Busi_I2C_Bus_Data.Port2_Out_Protocol_Data.Protocol_Voltage,\
									Busi_I2C_Bus_Data.Port2_Out_Protocol_Data.Protocol_Current,\
									(uint8_t)Busi_I2C_Bus_Data.Port2_Out_Protocol_Data.Current_Power_Grade);
			#endif
		}
		else if(PORT2_REALTIME_POWER == register_adr)
		{
			transfer_info.Reg_Addr = PORT2_REALTIME_POWER;
			transfer_info.Rdata_Len = sizeof(Busi_I2C_Bus_Data.Port2_RealTime_Out_Power);
			transfer_info.P_Rdata = (uint8_t *)&Busi_I2C_Bus_Data.Port2_RealTime_Out_Power;
		}
		else if(PORT2_NTC_TEMP_DATA == register_adr)
		{
			transfer_info.Reg_Addr = PORT2_NTC_TEMP_DATA;
			transfer_info.Rdata_Len = sizeof(Busi_I2C_Bus_Data.NTC_Temp_Data);
			transfer_info.P_Rdata = (uint8_t *)&Busi_I2C_Bus_Data.NTC_Temp_Data;
		}
		Func_Fast_Charge_Driver.IIC_Master_Driver.P_I2C_Master_Read_NByte(&transfer_info);
	}

	void Busi_I2CM_Write_Slave_Register(void)
	{
		I2CM_Transfer_Info_t transfer_info;
		transfer_info.Reg_Addr = PORT2_CTL_OUT_POWER;
		transfer_info.Wdata_Len = 1;
		transfer_info.P_Wdata[0] = Busi_I2C_Bus_Data.Port2_Seting_Output_Power;
		Func_Fast_Charge_Driver.IIC_Master_Driver.P_I2C_Master_Write_NByte(&transfer_info);
	}
#endif