/****************************************************************************
 * @copyright Copyright(C) 2020-2023 Ismartware Limited. All rights reserved.
 * @file main.c
 * @brief Main program body
 * @author SD1 & SW team
 ****************************************************************************/
#include "base_time_driver.h"
#include "OLED/oled_check.h"
#include "uart_driver.h"
#include "LED_Driver/led_driver.h"
#include "i2c_master_driver.h"

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
#define USER_DRIVER_CHECK
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
#define CUSTOMIZE_CFG

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


int main(void)
{
    /// device init shall be called firstly    
    Device_Init();
    Log_Pin_Init();

#ifdef CUSTOMIZE_CFG
    /// get system config, it shall be after Device_Init()
    config_data_t* config = Config_Get();
    config->portMode = DUO_CC_MODE;        // set to single c mode

#ifdef A_C_NOLOAD_TRIGGER
    config->portMode = DUO_CA_MODE;        // set to A+C mode
#endif

#ifdef A_A_NOLOAD_TRIGGER
    config->portMode = DUO_AA_MODE;        // set to A+A mode
#endif

#ifdef GATE_FORCE_CTRL
    config->portMode = DUO_AA_MODE;        // set to A+A mode
#endif

    // power
    config->fixVoltageOffset = FIX_VOLTAGE_OFFSET_200MV;
    config->fixCurrentOffset = FIX_CURRENT_OFFSET_300MA;
    config->cableCompensationValue = CABLE_COMPENSAVE_VALUE_100MOHM;
    config->p1CurrentLimit = PORT_CURR_LIMIT_3P0A;
    config->p2CurrentLimit = PORT_CURR_LIMIT_3P0A;
    config->isCableCompensaved = true;  
    config->isAbnLpsEnabled = false;

    // dpdm
    config->dpdmFixPower = 60;
    config->isAppleEnabled = true;
    config->isSamsungEnabled = true;
    config->isQc2p0Enabled = true;
    config->isQc3p0Enabled = true;
    config->isQc3plusEnabled = true;
    config->isPeEnabled = true;
    config->isSfcpEnabled = false;
    config->qc2p0MaxVoltage = QC2P0_MAX_VOL_20V;
    config->qc3p0MaxVoltage = QC3P0_MAX_VOL_20V;
    config->peMaxVoltage = PE_MAX_VOL_12V;
    config->qc3MaxPower = QC3_MAX_POWER_27W;
    config->afcMaxVoltage = AFC_MAX_VOL_12V;
    config->sfcpMaxVoltage = SFCP_MAX_VOL_12V;
	config->isPdCertWithApple = false;

    // tfcp
    config->isTfcpEnabled = false;
    config->tfcpMaxPower = 65;

    // scp
    config->isFcpEnabled = true;
    config->isHvScpEnabled = true;
    config->isLvScpEnabled = true;
    config->isAfcEnabled = true;
    config->scpMaxCurrent = 40;
    config->scpMaxPower = 40;
    config->scpMaxVoltage = 120;
    config->scpAdapterType = 0x0A;    
    config->fcpMaxVoltage = FCP_MAX_VOL_9V;
    config->isPdGatingScpEnabled = false;

    // vooc
    config->isVooc1p0Enabled = true;
    config->isVooc4p0Enabled = true;
    config->isSuperVooc2p0Enabled = true;
    config->voocMaxPower = 30;
    config->superVoocMaxPower = 65;
    config->isPdGatingVoocEnabled = false;
    config->voocPathImpedanceCommonCableTyp150 = 150;
    config->voocPathImpedanceCommonCableTyp180 = 180;
    config->voocPathImpedanceCommonCableTyp250 = 250;
    config->voocPathImpedanceGenuineCableTyp150 = 150;
    config->voocPathImpedanceGenuineCableTyp180 = 180;
    config->voocPathImpedanceGenuineCableTyp250 = 250;
    config->isVoocCableStricMode = false;
   
    // ufcs
    config->isUfcsEnabled = true;
    config->ufcsSrcCap = (const uint8_t*)ufcs100wOutputCap;
    config->ufcsSrcCapNum = 3;
    config->isUfcsCertEnabled = false;
    config->ufcsDevTemp = 25;
    config->ufcsUsbTemp = 25;

    // typec
    config->isTypecCertEnabled = false;
    config->typecCcTerm = RP_3A;
    config->isTypecVconnEnabled = true;
    config->isVconnLowPowerModeEnabled = false;
    config->typecConnectTime = 130;
    config->isPortCNoLoad = true;

    // pd
    config->pdSrcCap = pd140wCap;        // pd src cap dtat pointer
    config->xid = 0;
    config->vid = 0x2FE6;
    config->pid = 0;
    config->bcdDevice = 0;
    config->pdMaxPower = 140;            // 140w power
    config->sprPdoNumber = 6;            // spr pdo num
    config->sprFixPdoNumber = 5;         // spr fix pdo num
    config->eprPdoNumber = 2;            // epr pdo num,it is eprFixPdoNumber add avs pdo num
    config->eprFixPdoNumber = 1;         // epr fix pdo num
    config->isVconnSwapEnabled = false;
    config->isResponseDiscoveryId = false;

    config->isPdEnabled = true;
    config->isPdCertEnabled = false;
    config->pdVersionMinor = 6;          //for pps 5v min vol cert,must set 8 or above
    config->isEmarkEnabled = true;
    config->isPdo5v2aEnabled = false;
    config->isPdAlertEnabled = true;
    config->isInvalidPpsEnabled = true;
    config->pdVdmVer = 4;
	config->isIgnoreEprCable = false;
	config->vPpsShutDownInMv = 3100;
    Config_Apply();
   
    // User_Pd_Policy_Init();
//    //init sink policy 
//    Sink_Policy_Init();   
#endif

#if defined(DEBUG_PRINTF_OPEN)
    Serail_Init(460800);
    printf("\nzr2067_app: %s %s\n", __DATE__, __TIME__);
#endif

#if defined(USER_DRIVER_CHECK)
    LED_Driver_Check();
    I2C_Master_Check();
    Base_Time_Driver_Check();
#endif

#ifdef POWER_CALLBACK
    Device_Register_Power_Adjust_Hook(PowerAdjust);
#endif

#ifdef TRACE_PROTOCOL
    fast_charge_protocol_e fc = 0xFF;
#endif

#ifdef VD_DETECT
    vd_result_e result = Vd_Detect_Result_Get(TH_1P825V_1P300V, true, false);
    printf("vd = %d\n", result);
#endif 

#if  defined(A_C_NOLOAD_TRIGGER) || defined(A_A_NOLOAD_TRIGGER)
    Register_Noload_Trigger_Callback(A_C_Noload_Trigger_Callback);
    
    #ifdef A_C_NOLOAD_TRIGGER
    Register_Noload_X_Second_Config_Callback(Port_A_NoLoad_X_Second_Customize_Callback);
    #endif

    Register_Noload_State_Customize_Callback(A_NoLoad_Customize_Callback);
    // debug only
    Rcc_Module_Clk_Enable(GPIO_ID);
    Gpio_Function_Select(GPIO2, FUNCTION_SEL0); 
    Gpio_Direction_Cfg(GPIO2, GPIO_DIRECTION_OUTPUT);
    Gpio_Set_Output_Value(GPIO2, false);
#endif

#ifdef TRACE_PORT_STATE
    uint8_t portState = 0;
#endif   

#ifdef TRACE_BUCK_CFG
    Print_Buck_Cfg();
    Buck_Config_Test(BUCK_CFG_181KHZ_10UH);
    Buck_Config_Test(BUCK_CFG_333KHZ_4UH7);
    Buck_Config_Test(BUCK_CFG_125KHZ_22UH);
#endif

#ifdef GATE_FORCE_CTRL
    Register_Mos_State_Callback(Gate_State_Callback);
    Register_Noload_State_Customize_Callback(Gate_Online_Customize_Callback);
#endif    


#ifdef SING_C_CTRL_TWO_PORTS
    Register_Buck_Off_Callback(Buck_Off_Callback);
    Register_Mos_On_Callback(Mos_On_Callback);
    Register_Current_Limitation_Callback(Current_Limitation_Callback);
#endif    

#ifdef I2C_SLAVE_TEST
    I2c_Slave_Policy_Init();
#endif   


    while (1)
    {
        // the following sequence shall NOT change
        Device_Run();

#ifdef GATE_FORCE_CTRL
        // enter force mode
        if (gIsForceMode && (gIsQuitForceMode == 0))
        {
            Gate_Ctrl_Force_Mode_Set(true);
            gIsQuitForceMode++;
        }

        if (gIsForceMode)
        {
            Power_Gate_Ctrl(1, gGateCtrlState & 0x1);
            Power_Gate_Ctrl(2, gGateCtrlState & 0x2);
        }

        // quit force mode
        if (gIsQuitForceMode && !gIsForceMode)
        {
            Gate_Ctrl_Force_Mode_Set(false);
            gIsQuitForceMode = 0;
            Abn_Force_Ctrl(true);
            gIsAbnCount = 1;
        }

        if (gIsAbnCount > 0)
        {
            Low_Power_Not_Sleep();
            gIsAbnCount++;
            if (gIsAbnCount > 2000)
            {
                Abn_Force_Ctrl(false);
                gIsAbnCount = 0;
            }
        }

        if(gIsAdcPrint)
        {
            gIsAdcPrint = false;
            if(Adc_Is_Enabled())
            {
                printf("vout:%d\r\n",Adc_Vout_Get());
                printf("Iout0:%d\r\n",Adc_Iout_Get(0));
                printf("Iout1:%d\r\n",Adc_Iout_Get(1));
            }
        }

#endif

#ifdef A_A_NOLOAD_TRIGGER
        if (gNoSleep)
        {
            // when the port2 device plug-off, system can go to sleep
            if (Is_Port2_Plugoff())
            {
                gNoSleep = false;
                gNoLoadStep = NOLOAD_STEP_A_X_PLUG_OFF;
                printf("port2 plugoff, can sleep\n");
            }

            Low_Power_Not_Sleep();
        }
#endif        

#ifdef TRACE_PROTOCOL
        fast_charge_protocol_e temp = Device_Get_Protocol(2);
        if (temp != fc)
        {
            fc = temp;
            printf("protocol: %d\n", fc);
        }
#endif

#if defined(TRACE_PORT_STATE) || defined(A_C_NOLOAD_TRIGGER) || defined(A_A_NOLOAD_TRIGGER)
        uint8_t tempPortState = Device_Get_Port_State();
        #if defined(TRACE_PORT_STATE)
        if (tempPortState != portState)
        {
            portState = tempPortState;
            printf("port state = %d\n", portState);
        }
        #endif

        #if defined(A_C_NOLOAD_TRIGGER) 
        if (tempPortState != 3 && gNoLoadChecking)
        {
            gNoLoadChecking = false;
            gNoLoadCount = 0;
            Stop_Noload_Timer();
            printf("stop no load checking\n");
        }
        #endif

        #if defined(A_A_NOLOAD_TRIGGER) 
        if (tempPortState == 0 && gNoLoadSpecHandle)
        {
            gNoLoadChecking = false;
            gNoLoadCount = 0;
            gNoLoadSpecHandle = false;
            Stop_Noload_Timer();
            printf("stop no load checking\n");
        }
        #endif

#endif    


#ifdef SING_C_CTRL_TWO_PORTS
    if (gMulPortState.bitFields.port3CurrOnline != gMulPortState.bitFields.port3NewOnline)
    {
        gMulPortState.bitFields.waitPowerAjustFinished  = 0;
        gMulPortState.bitFields.port3CurrOnline = gMulPortState.bitFields.port3NewOnline;
        if (gMulPortState.bitFields.port3CurrOnline && gMulPortState.bitFields.port2Online)
        {
            printf("adjust to 5v\n");
            Power_Adjust(5000/10, 3000/25);
            gMulPortState.bitFields.waitPowerAjustFinished = 1;
        }
        else if (gMulPortState.bitFields.port3CurrOnline && !gMulPortState.bitFields.port2Online)
        {
            Power_Buck_Ctrl(true);
            Power_Adjust(5000/10, 3000/25);
            gMulPortState.bitFields.waitPowerAjustFinished = 1;
        }
        else if (!gMulPortState.bitFields.port3CurrOnline)
        {
            printf("close gate2 ->1\n");
            Power_Gate_Ctrl(2, false);
        }

    }

    if (gMulPortState.bitFields.waitPowerAjustFinished)
    {
        if (Power_Is_Adjust_Finished())
        {
           gMulPortState.bitFields.waitPowerAjustFinished = false;
           if (gMulPortState.bitFields.port3CurrOnline) 
           {
                if (!Power_Is_Gate_On(2))
                {
                    printf("open gate2 ->2\n");
                    Power_Gate_Ctrl(2, true);
                }
           }
        }
    }

    gMulPortState.bitFields.port2Online = Device_Get_Port_State() == 1;

    if (Power_Is_Buck_On() && !gMulPortState.bitFields.port2Online && !gMulPortState.bitFields.port3CurrOnline)
    {
        Power_Buck_Ctrl(false);
        printf("close buck\n");
    }
#endif

#ifdef I2C_SLAVE_TEST
        I2c_Slave_Policy_Run();
#endif        
        
        // sleep process
        Low_Power_Sleep();
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