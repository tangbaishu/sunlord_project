#include "func_hardware_api.h"
#include "role_identification.h"
#include "base_timer_driver.h"
#include "ntc_temp_sample.h"
#include "zr_systick.h"

#if defined(DEBUG_PRINTF_OPEN)
    #include "stdio.h"
    #define LOG "func_h_i: "
#endif

Hardware_Driver_Data_t Hardware_Driver_Data =
{
    .NTC_Data.NTC_Voltage_Value = &NTC_Temp_Data.NTC_Voltage_Value,
    .NTC_Data.NTC_Pullup_Current = &NTC_Temp_Data.NTC_Pullup_Current,
    .NTC_Data.NTC_Over_Temp_Flag = &NTC_Temp_Data.NTC_Over_Temp_Flag,
    .NTC_Data.OverTemp_Dispose_Flag = &NTC_Temp_Data.OverTemp_Dispose_Flag,
};

Hardware_Driver_API_t Hardware_Driver_API=
{
    .Func_Base_Timer_Driver_Init = Base_Timer_Driver_Init,
    .Func_NTC_Driver_Init = NTC_Driver_Init,
    .Func_Role_Identify = Role_Identification,
};

void Func_Hardware_Drvier_Init(void)
{
    Hardware_Driver_API.Func_Role_Identify(&Hardware_Driver_Data.Device_Type);
    Hardware_Driver_API.Func_Base_Timer_Driver_Init(1); // 1ms 基础定时器
    Hardware_Driver_API.Func_NTC_Driver_Init();
   
    if(Hardware_Driver_Data.Device_Type == HOST_ROLE)
    {
        MY_PRINTF(LOG"I2C Master Init\r\n");
        // Func_I2C_API_Example(I2C_MASTER_ASYNC);
        Hardware_Driver_API.I2C_Driver_API.I2C_Driver_Data.Driver_Type = I2C_MASTER_ASYNC;
        Func_I2C_Driver_Init(Hardware_Driver_API.I2C_Driver_API.I2C_Driver_Data.Driver_Type,\
                     50, & Hardware_Driver_API.I2C_Driver_API);
        Systick_Delay_Ms(400);
    }
    else
    {
        MY_PRINTF(LOG"I2C Slave Init\r\n");
        Hardware_Driver_API.I2C_Driver_API.I2C_Driver_Data.Driver_Type = I2C_SLAVE;
        Func_I2C_Driver_Init(Hardware_Driver_API.I2C_Driver_API.I2C_Driver_Data.Driver_Type,\
                     50, & Hardware_Driver_API.I2C_Driver_API);
    }
}
    