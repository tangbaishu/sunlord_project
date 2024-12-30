#include "app_fast_charge.h"
#include "busi_base_fast_charge_func.h"
#include "busi_power_policy_engine.h"
#include "busi_Fast_Charge_Config.h"
#include "busi_i2c_bus.h"

#include "api.h"
#include "zr_systick.h"

#if defined(DEBUG_PRINTF_OPEN)
    #include "stdio.h"
    #define LOG "app_f_c: "
#endif

App_Fast_Charge_Data_t	App_Fast_Charge_Data;
config_data_t* Fast_Charge_Config;

void temporary_func(void)
{

}

App_Fast_Charge_Func_t App_Fast_Charge_Func={
    .P_Fast_Charge_Driver_Init      =   Busi_Fast_Charge_Driver_Init,	// 基于客户需求，对快充模块进行硬件初始化配置
    .P_Vin_Voltage_Control          =   Busi_Vin_Voltage_Control,       // 母线电压控制
	.P_Real_Time_Allocation_Power   =   Real_Time_Allocation_Power,     // 实时功率分配
    .P_P_Dual_Port_Insert_Dispose   =   temporary_func,                 // 双端口插入处理	单双口功率切换处理
    .P_Temperature_Adjust_Power     =   temporary_func,                 // 温度调控			基于温度点，调整功率输出挡位
    .P_Set_Buck_Frequence           =   Device_Buck_Config,             // 设置降压频率		按照客户需求，初始化配置Buck频率
    .P_LowPowerHook_OnBefore        =   Busi_LowPower_OnBefore,         // 进入低功耗之前的处理函数
    .p_LowPowerHook_OnAfter         =   Busi_LowPower_OnAfter,          // 从低功耗唤醒后的处理函数
};

/**
 * @brief 快充参数配置；快充回调函数/钩子函数配置；快充相关外设初始化
 * 输出默认功率
 */
void APP_Fast_Charge_Init(void)
{
    // App_Fast_Charge_Func.P_Set_Buck_Frequence(BUCK_CFG_181KHZ_10UH);
	Fast_Charge_Config = Config_Get();
	Fast_Charge_Config->portMode = SINGLE_C_MODE;

/*** 无协议 ***/
    /*** power ***/
    Fast_Charge_Config->fixVoltageOffset = FIX_VOLTAGE_OFFSET_200MV;
    Fast_Charge_Config->fixCurrentOffset = FIX_CURRENT_OFFSET_300MA;
    Fast_Charge_Config->cableCompensationValue = CABLE_COMPENSAVE_VALUE_100MOHM;
    Fast_Charge_Config->p1CurrentLimit = PORT_CURR_LIMIT_3P0A;
    Fast_Charge_Config->p2CurrentLimit = PORT_CURR_LIMIT_3P0A;
    Fast_Charge_Config->isCableCompensaved = true;  
    Fast_Charge_Config->isAbnLpsEnabled = true;     // 开启LPS异常保护

    /*** dpdm ***/
    Fast_Charge_Config->dpdmFixPower = 24;

/*** 苹果 ***/
    /*** Apple ***/
    Fast_Charge_Config->isAppleEnabled = true;
    
/*** 三星 ***/  
    /*** BC1.2 ***/                 
    Fast_Charge_Config->isSamsungEnabled = true;    // BC1.2V
    /*** AFC ***/
    Fast_Charge_Config->isAfcEnabled = true;
    // Fast_Charge_Config->afcMaxVoltage = AFC_MAX_VOL_12V;

/*** 华为 ***/    
    /*** SCP ***/
    Fast_Charge_Config->isHvScpEnabled = true;
    Fast_Charge_Config->isLvScpEnabled = false;
    // Fast_Charge_Config->scpMaxCurrent = 23;                 // uint: 100mA
    // Fast_Charge_Config->scpMaxPower = 23;                   // unit: W
    // Fast_Charge_Config->scpMaxVoltage = 10;                 // unit: 100mV
    Fast_Charge_Config->scpAdapterType = 0x0A;    
    Fast_Charge_Config->isPdGatingScpEnabled = false;          // 待确认 是否在使用PD协议时抑制 SCP

    /*** FCP ***/
    Fast_Charge_Config->isFcpEnabled = true;
    // Fast_Charge_Config->fcpMaxVoltage = FCP_MAX_VOL_12V;

/*** 展讯 ***/     
    /*** Sfcp ***/
    Fast_Charge_Config->isSfcpEnabled = false;
    Fast_Charge_Config->sfcpMaxVoltage = SFCP_MAX_VOL_12V;

// /*** ？？？未知 ***/ 
//     /*** Tfcp ***/
    Fast_Charge_Config->isTfcpEnabled = false;
    Fast_Charge_Config->tfcpMaxPower = 18;                  // unit : W

/*** 联发科 ***/ 
    /*** PE ***/
    Fast_Charge_Config->isPeEnabled = false;
    Fast_Charge_Config->peMaxVoltage = PE_MAX_VOL_12V;

/*** OPPO ***/ 
    /*** VOOC ***/
    Fast_Charge_Config->isVooc1p0Enabled = false;
    Fast_Charge_Config->isVooc4p0Enabled = false;
    Fast_Charge_Config->isSuperVooc2p0Enabled = false;
    // Fast_Charge_Config->voocMaxPower = 30;                  // unit : W
    // Fast_Charge_Config->superVoocMaxPower = 65;             // unit : W
    Fast_Charge_Config->isPdGatingVoocEnabled = false;
    Fast_Charge_Config->voocPathImpedanceCommonCableTyp150 = 150;
    Fast_Charge_Config->voocPathImpedanceCommonCableTyp180 = 180;
    Fast_Charge_Config->voocPathImpedanceCommonCableTyp250 = 250;
    Fast_Charge_Config->voocPathImpedanceGenuineCableTyp150 = 150;
    Fast_Charge_Config->voocPathImpedanceGenuineCableTyp180 = 180;
    Fast_Charge_Config->voocPathImpedanceGenuineCableTyp250 = 250;
    Fast_Charge_Config->isVoocCableStricMode = false;

/*** 公有部分 ***/ 
    /*** typec ***/
    Fast_Charge_Config->isTypecCertEnabled = false;
    Fast_Charge_Config->typecCcTerm = RP_3A;
    Fast_Charge_Config->isTypecVconnEnabled = true;
    Fast_Charge_Config->isVconnLowPowerModeEnabled = true;
    Fast_Charge_Config->typecConnectTime = 130;
    Fast_Charge_Config->isPortCNoLoad = true;

    /*** QC ***/
    Fast_Charge_Config->isQc2p0Enabled = true;
    Fast_Charge_Config->isQc3p0Enabled = true;
    Fast_Charge_Config->isQc3plusEnabled = true;
    // Fast_Charge_Config->qc2p0MaxVoltage = QC2P0_MAX_VOL_12V;
    // Fast_Charge_Config->qc3p0MaxVoltage = QC2P0_MAX_VOL_12V;
    // Fast_Charge_Config->qc3MaxPower = QC3_MAX_POWER_18W;

    /*** ufcs ***/
    Fast_Charge_Config->isUfcsEnabled = true;
    // Fast_Charge_Config->ufcsSrcCap = (const uint8_t*)ufcs100wOutputCap;
    // Fast_Charge_Config->ufcsSrcCapNum = 3;
    // Fast_Charge_Config->isUfcsCertEnabled = false;
    // Fast_Charge_Config->ufcsDevTemp = 25;
    // Fast_Charge_Config->ufcsUsbTemp = 25;

    /*** PD ***/
    Fast_Charge_Config->isPdEnabled = true;
    // Fast_Charge_Config->pdSrcCap = pd140wCap;        // pd src cap dtat pointer
    Fast_Charge_Config->xid = 0;
    Fast_Charge_Config->vid = 0x291A;
    Fast_Charge_Config->pid = 0x2686;
    Fast_Charge_Config->bcdDevice = 0;
    // Fast_Charge_Config->pdMaxPower = 140;            // 140w power
    // Fast_Charge_Config->sprPdoNumber = 6;            // spr pdo num
    // Fast_Charge_Config->sprFixPdoNumber = 5;         // spr fix pdo num
    // Fast_Charge_Config->eprPdoNumber = 2;            // epr pdo num,it is eprFixPdoNumber add avs pdo num
    // Fast_Charge_Config->eprFixPdoNumber = 1;         // epr fix pdo num

    Fast_Charge_Config->isVconnSwapEnabled = false;
    Fast_Charge_Config->isResponseDiscoveryId = false;

    Fast_Charge_Config->isPdCertWithApple = true;
    Fast_Charge_Config->isPdCertEnabled = false;
    Fast_Charge_Config->pdVersionMinor = 6;          //for pps 5v min vol cert,must set 8 or above
    Fast_Charge_Config->isEmarkEnabled = true;
    Fast_Charge_Config->isPdo5v2aEnabled = false;
    Fast_Charge_Config->isPdAlertEnabled = true;
    Fast_Charge_Config->isInvalidPpsEnabled = true;
    Fast_Charge_Config->pdVdmVer = 4;
	Fast_Charge_Config->isIgnoreEprCable = false;
	Fast_Charge_Config->vPpsShutDownInMv = 3100;

    App_Fast_Charge_Func.P_Real_Time_Allocation_Power(Fast_Charge_Config);
    Config_Apply();
    App_Fast_Charge_Func.P_Fast_Charge_Driver_Init();
    
	Low_Power_Register_Hook(App_Fast_Charge_Func.P_LowPowerHook_OnBefore, App_Fast_Charge_Func.p_LowPowerHook_OnAfter);

    // Pd_Init_Hook_Func(PD_Request_Info_func, PD_Policy_Hook);               // 仅支持PD协议
	// Device_Register_Power_Adjust_Hook(Power_Adjust_Hook);                   // sink请求任意协议，均会回调该函数
	//Abnormal_Init_Hook_Func(); //暂不修改，使用默认值
}

void APP_Fast_Charge_Running(void)
{
    App_Fast_Charge_Func.P_Real_Time_Allocation_Power(Fast_Charge_Config);    // 配置 ufcs 和 PD 协议输出功率
    Busi_I2C_Bus();
}