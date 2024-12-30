#include "func_hardware_init.h"
#include "base_timer_driver.h"
#include "ntc_temp_sample.h"
#include "zr_systick.h"
#include "func_i2c_slave.h"
#if defined(DEBUG_PRINTF_OPEN)
    #include "stdio.h"
    #define LOG "func_h_i: "
#endif

Port_BSP_Data_t Port_BSP_Data = 
{
    .Port1_NTC_Temp_State = &NTC_Temp_Data.NTC_Over_Temp_Flag,
//    .Port2_NTC_Temp_State = ,
    .Port1_OverTemp_Dispose_Flag = &NTC_Temp_Data.OverTemp_Dispose_Flag,
//    .Port2_OverTemp_Dispose_Flag = ,
};

Func_Fast_Charge_Driver_t Func_Fast_Charge_Driver=
{
    .Func_Base_Timer_Driver_Init = Base_Timer_Driver_Init,
    .Func_NTC_Driver_Init = NTC_Driver_Init,
    #if (DEVICE_TYPE == HOST_DEVICE)
        .IIC_Master_Driver.P_I2C_Master_Init = I2C_Master_Driver_Init,
        .IIC_Master_Driver.P_I2C_Master_Write_NByte = I2C_Master_Write_NByte,
        .IIC_Master_Driver.P_I2C_Master_Read_NByte = I2C_Master_Read_NByte
    #else
        .Func_I2C_Slave_Driver_Init = I2c_Slave_Driver_Init,
    #endif
};

void Func_Fast_Charge_Drvier_Init(void)
{
    #if defined(DEBUG_PRINTF_OPEN)
	    printf(LOG"Func_Fast_Charge_Drvier_Init()\r\n");
    #endif
    Func_Fast_Charge_Driver.Func_NTC_Driver_Init();
    Func_Fast_Charge_Driver.Func_Base_Timer_Driver_Init(1);
    #if (DEVICE_TYPE == HOST_DEVICE)
        Func_Fast_Charge_Driver.IIC_Master_Driver.P_I2C_Master_Init(0x3c);
    #else
        Func_Fast_Charge_Driver.Func_I2C_Slave_Driver_Init();
        Pointer_I2C_Slave_Busi = Func_I2CS_Dispose;
    #endif
}