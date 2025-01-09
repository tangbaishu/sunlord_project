/****************************************************************************
 * @copyright Copyright(C) 2020-2023 Ismartware Limited. All rights reserved.
 * @file main.c
 * @brief Main program body
 * @author SD1 & SW team
 ****************************************************************************/
#include "base_timer_driver.h"
#include "app_fast_charge.h"
// #include "busi_i2c_bus.h"

#include "api.h"
#include "config.h"
#include "ufcs_caps.h"
#include "pdo.h"
#include "serial.h"
#include "zr_rcc.h"
#include "pd_message.h"
#include "sink_policy.h"
#include "zr_btm.h"
#include "zr_nvic.h"
#include "zr_adc.h"
#include "zr_gpio.h"
#include "zr_systick.h"

#include "user_pd_policy.h"

#include <stdio.h>
// #define POWER_CALLBACK
// #define TRACE_PROTOCOL
// #define TRACE_PORT_STATE
// #define A_C_NOLOAD_TRIGGER
// #define A_A_NOLOAD_TRIGGER
// #define TRACE_BUCK_CFG
// #define SING_C_CTRL_TWO_PORTS
// #define I2C_SLAVE_TEST
// #define GATE_FORCE_CTRL
// #define LOG_GPIO

#ifdef POWER_CALLBACK
static bool PowerAdjust(uint16_t* vol, uint16_t* port1Curr, uint16_t* port2Curr);
#endif

#if defined(A_C_NOLOAD_TRIGGER) || defined(A_A_NOLOAD_TRIGGER)

static void A_C_Noload_Trigger_Callback(bool* isDisableNoLoad);

#ifdef A_C_NOLOAD_TRIGGER
static void Port_A_NoLoad_X_Second_Customize_Callback(uint8_t* xSecond);
#endif

static void A_NoLoad_Customize_Callback(bool* isPort1Offline, bool* isPort2Offline);

static void Start_Noload_Timer(void);
static void Stop_Noload_Timer(void);
static uint32_t gNoLoadCount = 0;
static bool gNoLoadChecking = false;
static bool gNoLoadSpecHandle = false;
static bool gSimPort2Offline = false;

typedef enum 
{
    NOLOAD_STEP_UNKNOWN,
    NOLOAD_STEP_A_A,
    NOLOAD_STEP_A_X,
    NOLOAD_STEP_A_X_PLUG_OFF,
} noload_step_e;
static noload_step_e gNoLoadStep = NOLOAD_STEP_UNKNOWN;
static bool gNoSleep = false;

#endif

#ifdef TRACE_BUCK_CFG
static void Print_Buck_Cfg(void)
{
    uint32_t BuckConfig0 = *((volatile uint32_t *)0x4000D000);
    uint32_t BuckConfig1 = *((volatile uint32_t *)0x4000D004);
    uint32_t BuckConfig2 = *((volatile uint32_t *)0x4000D008);
    uint32_t BuckConfig3 = *((volatile uint32_t *)0x4000D00C);

    printf("buck config: \n");
    printf("config 0 old %#x \n", BuckConfig0);
    printf("config 1 old %#x \n", BuckConfig1);
    printf("config 2 old %#x \n", BuckConfig2);
    printf("config 3 old %#x \n", BuckConfig3);
}

static void Buck_Config_Test(buck_cfg_type_e iType)
{
    Device_Buck_Config(iType);
    Print_Buck_Cfg();
}
#endif

#ifdef SING_C_CTRL_TWO_PORTS

typedef union 
{
    uint8_t value;
    struct 
    {
        uint8_t port1Online: 1;
        uint8_t port2Online: 1;
        uint8_t port3NewOnline: 1;
        uint8_t port3CurrOnline: 1;
        uint8_t waitPowerAjustFinished: 1;
    }bitFields;
} mul_port_state_e;



static mul_port_state_e gMulPortState = {0};

static bool Buck_Off_Callback(void);
static bool Mos_On_Callback(void);
static bool Current_Limitation_Callback(uint16_t* port1Current, uint16_t* port2Current);
#endif

#ifdef GATE_FORCE_CTRL
static volatile bool gIsForceMode = false;
static volatile uint8_t gIsQuitForceMode = 0;
static volatile bool gIsAdcPrint = false;
static volatile uint16_t gIsAbnCount = false;
static volatile uint8_t gGateCtrlState = 3;
static uint8_t Gate_State_Callback(void);
static void Gate_Online_Customize_Callback(bool* isPort1Offline, bool* isPort2Offline);
#endif

#ifdef LOG_GPIO
#define LOG_PIN (GPIO2)

#define Log_Pin_Init() do {                                \
    Gpio_Function_Select(LOG_PIN, FUNCTION_SEL0);          \
    Gpio_Direction_Cfg(LOG_PIN, GPIO_DIRECTION_OUTPUT);    \
    Gpio_Interrupt_Disable(LOG_PIN);                       \
    Gpio_Set_Output_Value(LOG_PIN, false);                 \
} while (0);

#define Log_Pin_On() do{ZR_GPIO->DATAOUT |= LOG_PIN;} while (0);
#define Log_Pin_Off() do{ZR_GPIO->DATAOUT &= (uint32_t)(~LOG_PIN);} while (0);
#define Log_Pin_Toggle() do{ZR_GPIO->DATAOUT ^= LOG_PIN;} while (0);
#else
#define Log_Pin_Init()
#define Log_Pin_On()
#define Log_Pin_Off()
#define Log_Pin_Toggle()
#endif

#include "busi_port_detection.h"
#include "func_power_alloc.h"
#include "app_fast_charge.h"
#include "device_driver.h"
#include "func_hardware_api.h"
#include "busi_i2c_policy.h"
#include "app_sys_ctl.h"

int main(void)
{
    static uint32_t xms_slice=0;
    /// device init shall be called firstly    
    Device_Init();
    Log_Pin_Init();
// User_Pd_Policy_Init();
//    init sink policy 
//    Sink_Policy_Init();   
    
// #if defined(DEBUG_PRINTF_OPEN)
    Serail_Init(460800);
    printf("Serail_Init\r\n");
    printf("\nzr2067_app: %s %s\n", __DATE__, __TIME__);
// #endif
//	Func_I2CM_Async_Example();
    // I2C_Master_Driver_Test(0X3C);
    // I2CM_Async_Driver_Test();
    // Busi_I2C_Init();
    APP_Fast_Charge_Init();

    // Func_I2C_Driver_Init(I2C_SLAVE, 50, NULL); 
    // Func_I2C_Driver_Init(I2C_MASTER, 50, &I2C_Driver_Module);
    Func_Hardware_Drvier_Init();

    // Base_Timer_Driver_Init(1);
    // xms_slice = Base_Timer_1ms_Count;
    // Func_I2C_API_Example(I2C_MASTER_ASYNC);
    while (1)
    {
        // the following sequence shall NOT change
        Device_Run();
        Busi_Port_Detection();  // 获取自身硬件驱动相关状态
        if(HOST_ROLE == Hardware_Driver_Data.Device_Type)
        {
            if( PROCESS_READ_RGE_FINSIH == Busi_I2C_Policy(&Busi_I2C_Policy_Data) )  // 主机遍历从机运行数据
            {
                
            }
        }
        else
        {
            Slave_State_Machine();  // 从机运行逻辑，基于状态机确定
        }
        
        if((Base_Timer_1ms_Count - xms_slice) > 2000)
        {
            xms_slice = Base_Timer_1ms_Count;
            // Busi_I2CM_Write_Slave_Register();
            // Busi_I2CM_Read_Slave_Register(PORT2_OUT_PROTOCOL);
            // // Get_All_Port_Link_State();
            // if(Busi_Port_Detection_Data.Port1_Out_Protocol_Data.Port_Link_State == Port_1_Insert)
            // {
            //     Busi_Port_Detection_Func.Port1_RealTime_Out_Power_Func();
            // }
            
            // if(Busi_Port_Detection_Data.Port2_Out_Protocol_Data.Port_Link_State == Port_2_Insert)
            // {
            //     Busi_Port_Detection_Func.Port2_RealTime_Out_Power_Func();
            // }
            
        }
        // Low_Power_Sleep();
    }
}

#ifdef POWER_CALLBACK
static bool PowerAdjust(uint16_t* vol, uint16_t* port1Curr, uint16_t* port2Curr)
{
    #ifdef SING_C_CTRL_TWO_PORTS
    if (gMulPortState.bitFields.port2Online && gMulPortState.bitFields.port3CurrOnline)
    {
        *vol = 5000/10;
        if (port1Curr)
        {
            *port1Curr = 3000/25;
        }

        if (port2Curr)
        {
            *port2Curr = 3000/25;
        }
        return true;
    }
    else
    {
        return false;
    }
    #else
    // printf("%d, %d, %d\n", (*vol) * 10, (*port1Curr) * 25, (*port2Curr) * 25);
    printf("%d, %d\n", (*vol) * 10, port1Curr ? (*port1Curr) * 25 : (*port2Curr) * 25);
    if (*vol < 520)
    // if (Device_Get_Protocol(1) == FC_PD_PPS)
    {
        // *port1Curr = 160;
        *vol = 525;
        return true;
    }
    else
    {
        return false;
    }
    #endif
}
#endif


#if defined(A_C_NOLOAD_TRIGGER) || defined(A_A_NOLOAD_TRIGGER)
static void A_C_Noload_Trigger_Callback(bool* isDisableNoLoad)
{
    #ifdef A_C_NOLOAD_TRIGGER
    *isDisableNoLoad = true;
    printf("no load: disable auto checking, manual checking start, port state = %d\n", Device_Get_Port_State());
    gNoLoadChecking = true;
    gNoLoadCount = 0;
    Start_Noload_Timer();
    #else
    if (3 == Device_Get_Port_State())
    {
        *isDisableNoLoad = true;
        printf("no load: disable auto checking, manual checking start, port state = %d\n", Device_Get_Port_State());
        gNoLoadChecking = true;
        gNoLoadCount = 0;
        Start_Noload_Timer();
        gNoLoadStep = NOLOAD_STEP_A_A;
    }
    #endif    
}

#ifdef A_C_NOLOAD_TRIGGER
static void Port_A_NoLoad_X_Second_Customize_Callback(uint8_t* xSecond)
{
    uint8_t temp = *xSecond;
    *xSecond = 0;
    printf("x Second old : %d set to 0, port state = %d\n", temp, Device_Get_Port_State());
}
#endif


#define NOLOAD_CHECKING_INTERVAL (200)
#define NOLOAD_MAX_COUNT (3*1000/NOLOAD_CHECKING_INTERVAL)
#define IOUT_NOLOAD_TH (100)


static void A_NoLoad_Customize_Callback(bool* isPort1Offline, bool* isPort2Offline)
{
    #ifdef A_C_NOLOAD_TRIGGER
    if (gNoLoadChecking)
    {
        if (gNoLoadCount >= NOLOAD_MAX_COUNT)
        {
            Stop_Noload_Timer();
            // set port2 to offline
            *isPort2Offline = true;
            gNoLoadChecking = false;
            gNoLoadSpecHandle = true;
            gNoLoadCount = 0;
            printf("set port2 is offline = 1\n");
        }
    }
    #else

    
    if (gNoLoadChecking)
    {
        if (gNoLoadStep == NOLOAD_STEP_A_A)
        {
            if (gNoLoadCount >= NOLOAD_MAX_COUNT)
            {
                Stop_Noload_Timer();
                gNoLoadChecking = false;
                gNoLoadSpecHandle = true;
                gNoLoadCount = 0;
                gNoLoadStep = NOLOAD_STEP_A_X;
                if (gSimPort2Offline)
                {
                    *isPort2Offline = true;
                    printf("set port2 is offline = 1\n");
                }
                else
                {
                    *isPort1Offline = true;
                    printf("set port1 is offline = 1\n");
                }
                // not sleep when the port2 device still plugin
                gNoSleep = true;
            }
        }
    }
    else
    {
        if (gSimPort2Offline)
        {
            if (1 == Device_Get_Port_State() && gNoLoadStep == NOLOAD_STEP_A_X)
            {
                *isPort2Offline = true;
            }
        }
        else
        {
            if (2 == Device_Get_Port_State() && gNoLoadStep == NOLOAD_STEP_A_X)
            {
                *isPort1Offline = true;
            }

        }


        if (gNoLoadStep == NOLOAD_STEP_A_X_PLUG_OFF)
        {
            gNoSleep = false;
        }
    }
    #endif
}

void BTM4_Handler(void)
{
    Low_Power_Not_Sleep();

	if(Btm4_Get_Int_Pending())
	{
        Btm4_Clear_Int_Pending();
        uint16_t temp = Adc_Iout_Get(1);
        if (temp < IOUT_NOLOAD_TH)
        {
            gNoLoadCount++;
            printf("iout = %d, count:%d\n", temp, gNoLoadCount);
        }
        else
        {
            gNoLoadCount = 0;
        }
	}
}

static void Start_Noload_Timer(void)
{
    // start a btm every NOLOAD_CHECKING_INTERVAL ms
    Btm4_Set_Reload_Value(NOLOAD_CHECKING_INTERVAL*1000/8 - 1);
	Nvic_Irq_Enable(BTM4_IRQn,PRIORITY_2);
    Btm4_Int_Enable();
    Btm4_Enable();   
}

static void Stop_Noload_Timer(void)
{
	Nvic_Irq_Disable(BTM4_IRQn);
    Btm4_Int_Disable();
    Btm4_Disable();   
}

#endif


#ifdef GATE_FORCE_CTRL
static uint8_t Gate_State_Callback(void)
{
    return gIsForceMode ? gGateCtrlState : 0x3;
}

static void Gate_Online_Customize_Callback(bool* isPort1Offline, bool* isPort2Offline)
{
    if (gIsForceMode)
    {
        *isPort1Offline = (gGateCtrlState & 0x1) == 0x0;
        *isPort2Offline = (gGateCtrlState & 0x2) == 0x0;
    }
}
#endif


#ifdef SING_C_CTRL_TWO_PORTS
static bool Buck_Off_Callback(void)
{
    printf("buck off callback %d\n", !gMulPortState.bitFields.port3CurrOnline);
    return !gMulPortState.bitFields.port3CurrOnline;
}

static  bool Mos_On_Callback(void)
{
    uint16_t vol = Device_Get_Requesting_Voltage();
    printf("Mos on callback %d\n", vol);
    return vol == 5000;
}

static bool Current_Limitation_Callback(uint16_t* port1Current, uint16_t* port2Current)
{
    if (gMulPortState.bitFields.port3CurrOnline && gMulPortState.bitFields.port2Online && gMulPortState.bitFields.port1Online)
    {
        printf("current cb before 1 %d, %d\n", *port1Current, *port2Current);
        *port1Current = 3300/25;
        *port2Current = 2700/25;
        return true;
    }

    if (gMulPortState.bitFields.port3CurrOnline && gMulPortState.bitFields.port2Online && !gMulPortState.bitFields.port1Online)
    {
        printf("current cb before 2 %d, %d\n", *port1Current, *port2Current);
        *port1Current = 3300/25;
        *port2Current = 3300/25;
        return true;
    }

    return false;
}

#endif