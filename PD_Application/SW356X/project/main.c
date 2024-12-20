/****************************************************************************
 * @copyright Copyright(C) 2020-2023 Ismartware Limited. All rights reserved.
 * @file main.c
 * @brief Main program body
 * @author SD1 & SW team
 ****************************************************************************/
#include "power_coroutine.h"
#include "power_business.h"
#include "BSP/OLED/oled_check.h"
#include "uart_driver.h"
#include "LED_Driver/led_driver.h"
#include "BSP/PD_Protocol_IC/sw6233_driver.h"

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
#include "i2c_slave.h"

#include <stdio.h>

// #define EXTEND_SW_PD_IC      // 外挂智融 PD协议 芯片
// #define USER_DRIVER_CHECK    // 
#define DEBUG_PRINTF_OPEN
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

#undef CUSTOMIZE_CFG
// #define CUSTOMIZE_CFG

#ifdef CUSTOMIZE_CFG

static const ufcs_output_cap_t ufcs100wOutputCap[] = {
    {
        .fields.head = UFCS_CAP_SET_HEAD(UFCS_CAP_ID1, UFCS_CURR_STEP_50MA, UFCS_VOL_MIN_STEP_10MV),
        .fields.hiMaxVolage = UFCS_CAP_SET_HI_BYTE(550),         // unit : 10mV
        .fields.loMaxVolage = UFCS_CAP_SET_LO_BYTE(550),         // unit : 10mV
        .fields.hiMinVolage = UFCS_CAP_SET_HI_BYTE(340),         // unit : 10mV
        .fields.loMinVolage = UFCS_CAP_SET_LO_BYTE(340),         // unit : 10mV
        .fields.hiMaxCurrent = UFCS_CAP_SET_HI_BYTE(300),        // unit : 10mA
        .fields.loMaxCurrent = UFCS_CAP_SET_LO_BYTE(300),        // unit : 10mA
        .fields.minCurrent = 100,                                // unit : 10mA
    },
    {
        .fields.head = UFCS_CAP_SET_HEAD(UFCS_CAP_ID2, UFCS_CURR_STEP_50MA, UFCS_VOL_MIN_STEP_10MV),
        .fields.hiMaxVolage = UFCS_CAP_SET_HI_BYTE(1100),        // unit : 10mV
        .fields.loMaxVolage = UFCS_CAP_SET_LO_BYTE(1100),        // unit : 10mV
        .fields.hiMinVolage = UFCS_CAP_SET_HI_BYTE(550),         // unit : 10mV
        .fields.loMinVolage = UFCS_CAP_SET_LO_BYTE(550),         // unit : 10mV
        .fields.hiMaxCurrent = UFCS_CAP_SET_HI_BYTE(300),        // unit : 10mA
        .fields.loMaxCurrent = UFCS_CAP_SET_LO_BYTE(300),        // unit : 10mA
        .fields.minCurrent = 100,                                // unit : 10mV
    },
    {
        .fields.head = UFCS_CAP_SET_HEAD(UFCS_CAP_ID3, UFCS_CURR_STEP_50MA, UFCS_VOL_MIN_STEP_10MV),
        .fields.hiMaxVolage = UFCS_CAP_SET_HI_BYTE(2100),        // unit : 10mV
        .fields.loMaxVolage = UFCS_CAP_SET_LO_BYTE(2100),        // unit : 10mV
        .fields.hiMinVolage = UFCS_CAP_SET_HI_BYTE(1100),        // unit : 10mV
        .fields.loMinVolage = UFCS_CAP_SET_LO_BYTE(1100),        // unit : 10mV
        .fields.hiMaxCurrent = UFCS_CAP_SET_HI_BYTE(500),        // unit : 10mA
        .fields.loMaxCurrent = UFCS_CAP_SET_LO_BYTE(500),        // unit : 10mA
        .fields.minCurrent = 100,                                // unit : 10mA
    },
};

/*attention:response according to src cap setting
            eg.when PDO_FIXED_DUAL_DATA_ROLE set,when rx dr_swap message,response with accept
            so,src cap must be set correctly
*/
static const u32 pd140wCap[9] = {
    PDO_FIXED_VSAFE5V(3000, PDO_FIXED_EPR_MODE | PDO_FIXED_UNCONSTRAINED_POWER | PDO_FIXED_DUAL_DATA_ROLE,
        PDO_FIXED_NO_PEAK_CURRENT),        // support epr mode, accept dr swap, not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(9000, 3000, PDO_FIXED_NO_PEAK_CURRENT),         // not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(12000, 3000, PDO_FIXED_NO_PEAK_CURRENT),        // not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(15000, 3000, PDO_FIXED_NO_PEAK_CURRENT),        // not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(20000, 5000, PDO_FIXED_NO_PEAK_CURRENT),        // not support peak currrent!!!!!!
    PDO_PPS(5000, 3300, 21000, 0), 
    0,                                                               // it shall set to zero when the SPR pdo is invlaid
    PDO_FIXED_OTHERS(28000, 5000, PDO_FIXED_NO_PEAK_CURRENT),        // not support peak currrent!!!!!!
    PDO_AVS(15000, 28000, 140),
};

static const u32 pd140wCapTmp[9] = {
    PDO_FIXED_VSAFE5V(3000, PDO_FIXED_EPR_MODE | PDO_FIXED_UNCONSTRAINED_POWER | PDO_FIXED_DUAL_DATA_ROLE,
        PDO_FIXED_NO_PEAK_CURRENT),        // support epr mode, accept dr swap, not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(9000, 3000, PDO_FIXED_NO_PEAK_CURRENT),         // not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(12000, 3000, PDO_FIXED_NO_PEAK_CURRENT),        // not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(15000, 3000, PDO_FIXED_NO_PEAK_CURRENT),        // not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(20000, 4000, PDO_FIXED_NO_PEAK_CURRENT),        // not support peak currrent!!!!!!
    0,
    0,                                                               // it shall set to zero when the SPR pdo is invlaid
    PDO_FIXED_OTHERS(28000, 5000, PDO_FIXED_NO_PEAK_CURRENT),        // not support peak currrent!!!!!!
    PDO_AVS(15000, 28000, 140),
};

static const u32 pd60wCap[6] = {
    PDO_FIXED_VSAFE5V(3000, PDO_FIXED_UNCONSTRAINED_POWER,
        PDO_FIXED_NO_PEAK_CURRENT),        // not support epr mode, not support dr swap, not support peak
                                           // currrent!!!!!!!!!
    PDO_FIXED_OTHERS(9000, 3000, PDO_FIXED_NO_PEAK_CURRENT),         // not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(12000, 3000, PDO_FIXED_NO_PEAK_CURRENT),        // not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(15000, 3000, PDO_FIXED_NO_PEAK_CURRENT),        // not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(20000, 3000,
        PDO_FIXED_PEAK_CURRENT_MODE1),        // enable peak currrent when sink request this fix!!!!!!
    PDO_PPS(3000, 3300, 21000, PDO_SPR_PPS_POWER_LIMITED),        // support constant power(max 60w)!!!!!!!!
};

static const u32 pd60wCapTmp[6] = {
    PDO_FIXED_VSAFE5V(3000, PDO_FIXED_UNCONSTRAINED_POWER, PDO_FIXED_NO_PEAK_CURRENT),// not support epr mode, not support dr swap, not support peak currrent!!!!!!!!!
    PDO_FIXED_OTHERS(9000, 3000, PDO_FIXED_NO_PEAK_CURRENT),         // not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(15000, 3000, PDO_FIXED_NO_PEAK_CURRENT),        // not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(20000, 3000, PDO_FIXED_NO_PEAK_CURRENT),        // not support peak currrent!!!!!!
	PDO_SPR_AVS(3000, 3000),                                      //support 15-20v and 9-15v spr avs 
    PDO_PPS(3000, 3300, 21000, PDO_SPR_PPS_POWER_LIMITED),        // support constant power(max 60w)!!!!!!!!
};

static bool powerChange = FALSE;
#endif


#ifdef POWER_CALLBACK
static bool PowerAdjust(uint16_t* vol, uint16_t* port1Curr, uint16_t* port2Curr);
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


int main(void)
{
    /// device init shall be called firstly    
    Device_Init();
    
    Log_Pin_Init();
   
    Serail_Init(460800);
    printf("\nzr2067_app: %s %s\n", __DATE__, __TIME__);

    Power_Business_Init();
    #include "log_function.h"
	LOG_Abnormal_Occurred_Event(0x7FFFF);
    LOG_Abnormal_Release_Event(0x3FFF);
    while (1)
    {
        // the following sequence shall NOT change
        Device_Run();
        #if ((CONFIG_PORT_MODE == SET_SINGLE_A_MODE) || (CONFIG_PORT_MODE == SET_DUO_AA_MODE) || (CONFIG_PORT_MODE == SET_DUO_CA_MODE))
            Port_Scan_Func_Logic();
        #endif
        Power_Supply_Charge_Routine();
        // sleep process
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