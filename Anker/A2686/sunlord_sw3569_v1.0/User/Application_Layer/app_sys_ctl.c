#include "app_sys_ctl.h"
#include "busi_share_memory.h"
#include "busi_i2c_policy.h"
#include "device_driver.h"
#include "app_fast_charge.h"
#include "api.h"

typedef struct
{
	uint8_t Current_Seting_Power;
}Slave_Running_Data_t;
Slave_Running_Data_t	Slave_Running_Data;

static APP_SYS_Ctl_t APP_SYS_Ctl;

void Slave_State_Machine(void)
{
	uint8_t emark_check;
	if ((0 != I2C_SW3566_Reg_Data->Port_Seting_Power) && \
		(Slave_Running_Data.Current_Seting_Power != I2C_SW3566_Reg_Data->Port_Seting_Power))
	{
		MY_PRINTF("seting power = %d\r\n", I2C_SW3566_Reg_Data->Port_Seting_Power);
		App_Fast_Charge.SW_API_Func->SW_Set_Output_Power(1, ENABLE, I2C_SW3566_Reg_Data->Port_Seting_Power);
		Slave_Running_Data.Current_Seting_Power = I2C_SW3566_Reg_Data->Port_Seting_Power;
	}
	if(0 != I2C_SW3566_Reg_Data->Port_Rebroadcast_PDO)
	{
		if(Slave_Running_Data.Current_Seting_Power >= 65)
		{
			emark_check = 1;
		}
		MY_PRINTF("Rebroadcast PDO\r\n");
		App_Fast_Charge.SW_API_Func->SW_Rebroadcast_PDO(1, emark_check, 0);
	}
	if(I2C_SW3566_Reg_Data->Keep_Working)
	{
		MY_PRINTF("not sleep\r\n");
		Low_Power_Not_Sleep();
	}
}