#include "power_business.h"

#include "power_function_api.h"
#include "port_module.h"
#include "zr_systick.h"

#include "api.h"			// 快充相关的api接口
#include "config.h"			// 快充配置结构体
#include "ufcs_caps.h"		// ufcs 输出能力配置
#include "pdo.h"			// PDO广播包
#include "serial.h"			// 串口打印
#include "pd_message.h"		// PD消息包
#include "sink_policy.h"	// 受电端供应? 后续需重点阅读理解
#include "user_pd_policy.h"	// 用户PD策略 后续需重点阅读理解

#define LOG "power_business: "

Power_Business_t Power_Business_Data;

static const ufcs_output_cap_t UFCS_Supply_70W_Cap[] = {
    {
        .fields.head = UFCS_CAP_SET_HEAD(UFCS_CAP_ID1, UFCS_CURR_STEP_50MA, UFCS_VOL_MIN_STEP_10MV),
        .fields.hiMaxVolage = UFCS_CAP_SET_HI_BYTE(550),         // unit : 10mV
        .fields.loMaxVolage = UFCS_CAP_SET_LO_BYTE(550),         // unit : 10mV
        .fields.hiMinVolage = UFCS_CAP_SET_HI_BYTE(340),         // unit : 10mV
        .fields.loMinVolage = UFCS_CAP_SET_LO_BYTE(340),         // unit : 10mV
        .fields.hiMaxCurrent = UFCS_CAP_SET_HI_BYTE(300),        // unit : 10mA
        .fields.loMaxCurrent = UFCS_CAP_SET_LO_BYTE(300),        // unit : 10mA
        .fields.minCurrent = 100,                                // unit : 10mA
    },  // 5V电压档位的可编程范围是 3.4V - 5.5V、电流I ≤ 3
    {
        .fields.head = UFCS_CAP_SET_HEAD(UFCS_CAP_ID2, UFCS_CURR_STEP_50MA, UFCS_VOL_MIN_STEP_10MV),
        .fields.hiMaxVolage = UFCS_CAP_SET_HI_BYTE(1200),        // unit : 10mV
        .fields.loMaxVolage = UFCS_CAP_SET_LO_BYTE(1200),        // unit : 10mV
        .fields.hiMinVolage = UFCS_CAP_SET_HI_BYTE(550),         // unit : 10mV
        .fields.loMinVolage = UFCS_CAP_SET_LO_BYTE(550),         // unit : 10mV
        .fields.hiMaxCurrent = UFCS_CAP_SET_HI_BYTE(300),        // unit : 10mA
        .fields.loMaxCurrent = UFCS_CAP_SET_LO_BYTE(300),        // unit : 10mA
        .fields.minCurrent = 100,                                // unit : 10mV
    },  // 10V电压档位的可编程范围是 5.5V - 12V、P ＜ 40W时电流I ≤ 2、 P ≥ 40W时电流I ≤ 3
    {
        .fields.head = UFCS_CAP_SET_HEAD(UFCS_CAP_ID3, UFCS_CURR_STEP_50MA, UFCS_VOL_MIN_STEP_10MV),
        .fields.hiMaxVolage = UFCS_CAP_SET_HI_BYTE(2100),        // unit : 10mV
        .fields.loMaxVolage = UFCS_CAP_SET_LO_BYTE(2100),        // unit : 10mV
        .fields.hiMinVolage = UFCS_CAP_SET_HI_BYTE(1200),        // unit : 10mV
        .fields.loMinVolage = UFCS_CAP_SET_LO_BYTE(1200),        // unit : 10mV
        .fields.hiMaxCurrent = UFCS_CAP_SET_HI_BYTE(350),        // unit : 10mA
        .fields.loMaxCurrent = UFCS_CAP_SET_LO_BYTE(350),        // unit : 10mA
        .fields.minCurrent = 100,                                // unit : 10mA
    },  // 20V电压档位的可编程范围是 12V - 21V、电流I ≤ 3
    // {
    //     .fields.head = UFCS_CAP_SET_HEAD(UFCS_CAP_ID4, UFCS_CURR_STEP_50MA, UFCS_VOL_MIN_STEP_10MV),
    //     .fields.hiMaxVolage = UFCS_CAP_SET_HI_BYTE(3600),        // unit : 10mV
    //     .fields.loMaxVolage = UFCS_CAP_SET_LO_BYTE(3600),        // unit : 10mV
    //     .fields.hiMinVolage = UFCS_CAP_SET_HI_BYTE(2100),        // unit : 10mV
    //     .fields.loMinVolage = UFCS_CAP_SET_LO_BYTE(2100),        // unit : 10mV
    //     .fields.hiMaxCurrent = UFCS_CAP_SET_HI_BYTE(225),        // unit : 10mA
    //     .fields.loMaxCurrent = UFCS_CAP_SET_LO_BYTE(225),        // unit : 10mA
    //     .fields.minCurrent = 100,                                // unit : 10mA
    // },   // 30V电压档位的可编程范围是 21V - 36V
};

static const ufcs_output_cap_t UFCS_Supply_45W_Cap[] = {
    {
        .fields.head = UFCS_CAP_SET_HEAD(UFCS_CAP_ID1, UFCS_CURR_STEP_50MA, UFCS_VOL_MIN_STEP_10MV),
        .fields.hiMaxVolage = UFCS_CAP_SET_HI_BYTE(550),         // unit : 10mV
        .fields.loMaxVolage = UFCS_CAP_SET_LO_BYTE(550),         // unit : 10mV
        .fields.hiMinVolage = UFCS_CAP_SET_HI_BYTE(340),         // unit : 10mV
        .fields.loMinVolage = UFCS_CAP_SET_LO_BYTE(340),         // unit : 10mV
        .fields.hiMaxCurrent = UFCS_CAP_SET_HI_BYTE(300),        // unit : 10mA
        .fields.loMaxCurrent = UFCS_CAP_SET_LO_BYTE(300),        // unit : 10mA
        .fields.minCurrent = 100,                                // unit : 10mA
    },  // 5V电压档位的可编程范围是 3.4V - 5.5V
    {
        .fields.head = UFCS_CAP_SET_HEAD(UFCS_CAP_ID2, UFCS_CURR_STEP_50MA, UFCS_VOL_MIN_STEP_10MV),
        .fields.hiMaxVolage = UFCS_CAP_SET_HI_BYTE(1200),        // unit : 10mV
        .fields.loMaxVolage = UFCS_CAP_SET_LO_BYTE(1200),        // unit : 10mV
        .fields.hiMinVolage = UFCS_CAP_SET_HI_BYTE(550),         // unit : 10mV
        .fields.loMinVolage = UFCS_CAP_SET_LO_BYTE(550),         // unit : 10mV
        .fields.hiMaxCurrent = UFCS_CAP_SET_HI_BYTE(300),        // unit : 10mA
        .fields.loMaxCurrent = UFCS_CAP_SET_LO_BYTE(300),        // unit : 10mA
        .fields.minCurrent = 100,                                // unit : 10mV
    },  // 10V电压档位的可编程范围是 5.5V - 12V
	{
        .fields.head = UFCS_CAP_SET_HEAD(UFCS_CAP_ID3, UFCS_CURR_STEP_50MA, UFCS_VOL_MIN_STEP_10MV),
        .fields.hiMaxVolage = UFCS_CAP_SET_HI_BYTE(2100),        // unit : 10mV
        .fields.loMaxVolage = UFCS_CAP_SET_LO_BYTE(2100),        // unit : 10mV
        .fields.hiMinVolage = UFCS_CAP_SET_HI_BYTE(1200),        // unit : 10mV
        .fields.loMinVolage = UFCS_CAP_SET_LO_BYTE(1200),        // unit : 10mV
        .fields.hiMaxCurrent = UFCS_CAP_SET_HI_BYTE(225),        // unit : 10mA
        .fields.loMaxCurrent = UFCS_CAP_SET_LO_BYTE(225),        // unit : 10mA
        .fields.minCurrent = 100,                                // unit : 10mA
    },  // 20V电压档位的可编程范围是 12V - 21V
    // {
    //     .fields.head = UFCS_CAP_SET_HEAD(UFCS_CAP_ID4, UFCS_CURR_STEP_50MA, UFCS_VOL_MIN_STEP_10MV),
    //     .fields.hiMaxVolage = UFCS_CAP_SET_HI_BYTE(3600),        // unit : 10mV
    //     .fields.loMaxVolage = UFCS_CAP_SET_LO_BYTE(3600),        // unit : 10mV
    //     .fields.hiMinVolage = UFCS_CAP_SET_HI_BYTE(2100),        // unit : 10mV
    //     .fields.loMinVolage = UFCS_CAP_SET_LO_BYTE(2100),        // unit : 10mV
    //     .fields.hiMaxCurrent = UFCS_CAP_SET_HI_BYTE(225),        // unit : 10mA
    //     .fields.loMaxCurrent = UFCS_CAP_SET_LO_BYTE(225),        // unit : 10mA
    //     .fields.minCurrent = 100,                                // unit : 10mA
    // },   // 30V电压档位的可编程范围是 21V - 36V
};

static const u32 PD_Supply_70W_Cap[]={
    PDO_FIXED_VSAFE5V(3000, PDO_FIXED_UNCONSTRAINED_POWER, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_FIXED_OTHERS(9000, 3000, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_FIXED_OTHERS(12000, 3000, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_FIXED_OTHERS(20000, 3500, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_PPS(3500, 5000, 21000,PDO_SPR_PPS_POWER_LIMITED)
};

static const u32 PD_Supply_45W_Cap[]={
    PDO_FIXED_VSAFE5V(3000, PDO_FIXED_UNCONSTRAINED_POWER, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_FIXED_OTHERS(9000, 3000, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_FIXED_OTHERS(12000, 3000, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_FIXED_OTHERS(20000, 2250, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_PPS(3000, 5000, 15000,PDO_SPR_PPS_POWER_LIMITED),
	PDO_PPS(2250, 15020, 21000,PDO_SPR_PPS_POWER_LIMITED)
};


static void LowPowerHook_OnBefore(void);
static void LowPowerHook_OnAfter(void);
static bool Power_Adjust_Hook(uint16_t* vol, uint16_t* port1Curr, uint16_t* port2Curr);
void Power_Supply_Switch(Power_Supply_Mode_e mode_select, bool right_away);

void Power_Business_Init(void)
{
	Power_Business_Data.P_Init_Config = Config_Get();
	Power_Business_Data.P_Init_Config->portMode = CONFIG_PORT_MODE;

	// power
    Power_Business_Data.P_Init_Config->fixVoltageOffset = FIX_VOLTAGE_OFFSET_200MV;
    Power_Business_Data.P_Init_Config->fixCurrentOffset = FIX_CURRENT_OFFSET_300MA;
    Power_Business_Data.P_Init_Config->cableCompensationValue = CABLE_COMPENSAVE_VALUE_100MOHM;
    Power_Business_Data.P_Init_Config->p1CurrentLimit = PORT_CURR_LIMIT_3P0A;
    Power_Business_Data.P_Init_Config->p2CurrentLimit = PORT_CURR_LIMIT_3P0A;
    Power_Business_Data.P_Init_Config->isCableCompensaved = true;  
    Power_Business_Data.P_Init_Config->isAbnLpsEnabled = false;

    // dpdm Apple、Samsung、QC、PE、
    Power_Business_Data.P_Init_Config->isAppleEnabled = true;
    Power_Business_Data.P_Init_Config->isSamsungEnabled = true;
    Power_Business_Data.P_Init_Config->isQc2p0Enabled = true;
    Power_Business_Data.P_Init_Config->isQc3p0Enabled = true;
    Power_Business_Data.P_Init_Config->isQc3plusEnabled = true;
    Power_Business_Data.P_Init_Config->isPeEnabled = true;
    Power_Business_Data.P_Init_Config->isSfcpEnabled = false;
    Power_Business_Data.P_Init_Config->qc2p0MaxVoltage = QC2P0_MAX_VOL_20V;
    Power_Business_Data.P_Init_Config->qc3p0MaxVoltage = QC3P0_MAX_VOL_20V;
    Power_Business_Data.P_Init_Config->peMaxVoltage = PE_MAX_VOL_12V;
    Power_Business_Data.P_Init_Config->afcMaxVoltage = AFC_MAX_VOL_12V;
    Power_Business_Data.P_Init_Config->sfcpMaxVoltage = SFCP_MAX_VOL_12V;
	Power_Business_Data.P_Init_Config->isPdCertWithApple = false;

    // tfcp
    Power_Business_Data.P_Init_Config->isTfcpEnabled = false;

    // scp
    Power_Business_Data.P_Init_Config->isFcpEnabled = true;
    Power_Business_Data.P_Init_Config->isHvScpEnabled = true;
    Power_Business_Data.P_Init_Config->isLvScpEnabled = true;
    Power_Business_Data.P_Init_Config->isAfcEnabled = true;
    Power_Business_Data.P_Init_Config->scpAdapterType = 0x0A;  
    Power_Business_Data.P_Init_Config->isPdGatingScpEnabled = false;

    // vooc
    Power_Business_Data.P_Init_Config->isVooc1p0Enabled = true;
    Power_Business_Data.P_Init_Config->isVooc4p0Enabled = true;
    Power_Business_Data.P_Init_Config->isSuperVooc2p0Enabled = true;
    Power_Business_Data.P_Init_Config->isPdGatingVoocEnabled = false;
    Power_Business_Data.P_Init_Config->voocPathImpedanceCommonCableTyp150 = 150;
    Power_Business_Data.P_Init_Config->voocPathImpedanceCommonCableTyp180 = 180;
    Power_Business_Data.P_Init_Config->voocPathImpedanceCommonCableTyp250 = 250;
    Power_Business_Data.P_Init_Config->voocPathImpedanceGenuineCableTyp150 = 150;
    Power_Business_Data.P_Init_Config->voocPathImpedanceGenuineCableTyp180 = 180;
    Power_Business_Data.P_Init_Config->voocPathImpedanceGenuineCableTyp250 = 250;
    Power_Business_Data.P_Init_Config->isVoocCableStricMode = false;

    // typec
    Power_Business_Data.P_Init_Config->isTypecCertEnabled = false;
    Power_Business_Data.P_Init_Config->typecCcTerm = RP_3A;
    Power_Business_Data.P_Init_Config->isTypecVconnEnabled = true;
    Power_Business_Data.P_Init_Config->isVconnLowPowerModeEnabled = false;
    Power_Business_Data.P_Init_Config->typecConnectTime = 130;
    Power_Business_Data.P_Init_Config->isPortCNoLoad = true;

    // pd
    Power_Business_Data.P_Init_Config->xid = 0;
    Power_Business_Data.P_Init_Config->vid = 0x2FE6;
    Power_Business_Data.P_Init_Config->pid = 0;
    Power_Business_Data.P_Init_Config->bcdDevice = 0;
    Power_Business_Data.P_Init_Config->isVconnSwapEnabled = false;
    Power_Business_Data.P_Init_Config->isResponseDiscoveryId = false;

    Power_Business_Data.P_Init_Config->isPdCertEnabled = false;
    Power_Business_Data.P_Init_Config->pdVersionMinor = 6;          //for pps 5v min vol cert,must set 8 or above
    Power_Business_Data.P_Init_Config->isEmarkEnabled = true;
    Power_Business_Data.P_Init_Config->isPdo5v2aEnabled = false;
    Power_Business_Data.P_Init_Config->isPdAlertEnabled = true;
    Power_Business_Data.P_Init_Config->isInvalidPpsEnabled = true;
    Power_Business_Data.P_Init_Config->pdVdmVer = 4;
	Power_Business_Data.P_Init_Config->isIgnoreEprCable = false;
	Power_Business_Data.P_Init_Config->vPpsShutDownInMv = 3100;

    Port_Module_Data.Power_Supply_Select = Power_Supply_Switch;             // 将电源供应切换逻辑函数地址 赋值给 电源功率选择函数指针
    Port_Module_Data.Power_Supply_Select(POWER_SUPPLY_MODE_A, FALSE);	    // 配置 ufcs 和 PD 协议输出功率

    Config_Apply();
    
	Low_Power_Register_Hook(LowPowerHook_OnBefore, LowPowerHook_OnAfter);
    // Pd_Init_Hook_Func(PD_Request_Info_func, PD_Policy_Hook);                // 仅支持PD协议
	Device_Register_Power_Adjust_Hook(Power_Adjust_Hook);                   // sink请求任意协议，均会回调该函数
	//Abnormal_Init_Hook_Func(); //暂不修改，使用默认值

    Port_Scan_Func_Init();
}

void Power_Supply_Switch(Power_Supply_Mode_e mode_select, bool right_away)
{
	if(mode_select == POWER_SUPPLY_MODE_A)
	{
        // tfcp
        Power_Business_Data.P_Init_Config->tfcpMaxPower = 65;
        // dpdm
        Power_Business_Data.P_Init_Config->dpdmFixPower = 60;
        //qc3
        Power_Business_Data.P_Init_Config->qc3MaxPower = QC3_MAX_POWER_27W;
        // scp
        Power_Business_Data.P_Init_Config->scpMaxCurrent = 40;
        Power_Business_Data.P_Init_Config->scpMaxPower = 40;
        Power_Business_Data.P_Init_Config->scpMaxVoltage = 120;  
        // fcp
        Power_Business_Data.P_Init_Config->fcpMaxVoltage = FCP_MAX_VOL_9V;
        // vooc
        Power_Business_Data.P_Init_Config->voocMaxPower = 30;
        Power_Business_Data.P_Init_Config->superVoocMaxPower = 65;

        // ufcs
        Power_Business_Data.P_Init_Config->isUfcsEnabled = true;
		Power_Business_Data.P_Init_Config->ufcsSrcCap = (uint8_t*)UFCS_Supply_70W_Cap;
		Power_Business_Data.P_Init_Config->ufcsSrcCapNum = 3;
		Power_Business_Data.P_Init_Config->isUfcsCertEnabled = false;
		Power_Business_Data.P_Init_Config->ufcsDevTemp = 25;
		Power_Business_Data.P_Init_Config->ufcsUsbTemp = 25;

        // pd
         Power_Business_Data.P_Init_Config->isPdEnabled = true;
		Power_Business_Data.P_Init_Config->pdSrcCap = PD_Supply_70W_Cap;
		Power_Business_Data.P_Init_Config->pdMaxPower = 70;
		Power_Business_Data.P_Init_Config->sprPdoNumber = 5;
		Power_Business_Data.P_Init_Config->sprFixPdoNumber = 4;
		Power_Business_Data.P_Init_Config->eprPdoNumber = 0;
		Power_Business_Data.P_Init_Config->eprFixPdoNumber = 0;
	}
	else if(mode_select == POWER_SUPPLY_MODE_B)
	{
        // tfcp
        Power_Business_Data.P_Init_Config->tfcpMaxPower = 45;
        // dpdm
        Power_Business_Data.P_Init_Config->dpdmFixPower = 45;
        //qc3
        Power_Business_Data.P_Init_Config->qc3MaxPower = QC3_MAX_POWER_27W;
        // scp
        Power_Business_Data.P_Init_Config->scpMaxCurrent = 40;
        Power_Business_Data.P_Init_Config->scpMaxPower = 40;
        Power_Business_Data.P_Init_Config->scpMaxVoltage = 120;  
        // fcp
        Power_Business_Data.P_Init_Config->fcpMaxVoltage = FCP_MAX_VOL_9V;
        // vooc
        Power_Business_Data.P_Init_Config->voocMaxPower = 30;
        Power_Business_Data.P_Init_Config->superVoocMaxPower = 45;

        //ufcs
        Power_Business_Data.P_Init_Config->isUfcsEnabled = true;
		Power_Business_Data.P_Init_Config->ufcsSrcCap = (uint8_t*)UFCS_Supply_45W_Cap;
		Power_Business_Data.P_Init_Config->ufcsSrcCapNum = 3;
		Power_Business_Data.P_Init_Config->isUfcsCertEnabled = false;
		Power_Business_Data.P_Init_Config->ufcsDevTemp = 25;
		Power_Business_Data.P_Init_Config->ufcsUsbTemp = 25;
        
        //pd
         Power_Business_Data.P_Init_Config->isPdEnabled = true;
		Power_Business_Data.P_Init_Config->pdSrcCap = PD_Supply_45W_Cap;
		Power_Business_Data.P_Init_Config->pdMaxPower = 45;
		Power_Business_Data.P_Init_Config->sprPdoNumber = 6;
		Power_Business_Data.P_Init_Config->sprFixPdoNumber = 4;
		Power_Business_Data.P_Init_Config->eprPdoNumber = 0;
		Power_Business_Data.P_Init_Config->eprFixPdoNumber = 0;
	}

    if(right_away == TRUE)
    {
        Config_Apply();             // 立即应用 配置表
        Ufcs_Set_Power_Change();    // ufcs 
        Pd_Rebroadcast_Src_Cap();   // PD重新广播
    }
}

static void LowPowerHook_OnBefore(void)
{
	printf("ready to enter low power\r\n");
	Low_Power_Not_Sleep();
}

static void LowPowerHook_OnAfter(void)
{
	printf("low power waken\r\n");
}

/**
 * @brief 输出功率调整 (钩子函数)
 * 若需要调整，需在每次插入后重复修改调整值
 * 当port1Curr / port2Curr值为2528时，表明该端口空闲无协议获取。
 * @param vol           输出电压    单位：10mV
 * @param port1Curr     端口1限流   单位：25mA
 * @param port2Curr     端口2限流   单位：25mA
 * @return true         执行本次调整值
 * @return false        不执行本次调整值
 */
static bool Power_Adjust_Hook(uint16_t* vol, uint16_t* port1Curr, uint16_t* port2Curr)
{
    static uint8_t port1_adjust_log=0,port2_adjust_log=0;
    printf(LOG"Power_Adjust_Hook() vol=%d port1Curr=%d port2Curr=%d \r\n", *vol, *port1Curr, *port2Curr);
    if(Port_Module_Data.Port1_Insert_State.Current_State == PORT_ATTACH)
    {
        Power_Business_Data.Port1_Current_FC_Protocol = Device_Get_Protocol(1);
        printf("Port1_Protocol = %d\r\n",Power_Business_Data.Port1_Current_FC_Protocol);
        switch (Power_Business_Data.Port1_Current_FC_Protocol)
        {
            case FC_PD_Fix5V:   *port1Curr = 120; port1_adjust_log = 1; break;
            case FC_PD_FixHV:   *port1Curr = 144; port1_adjust_log = 1; break;
            default: break;
        }
    }
    if(Port_Module_Data.Port2_Insert_State.Current_State == PORT_ATTACH)
    {
        Power_Business_Data.Port2_Current_FC_Protocol = Device_Get_Protocol(2);
        printf("Port2_Protocol = %d\r\n",Power_Business_Data.Port2_Current_FC_Protocol);
        switch (Power_Business_Data.Port2_Current_FC_Protocol)
        {
            case FC_PD_Fix5V:   *port2Curr = 120; port2_adjust_log = 1; break;
            case FC_PD_FixHV:   *port2Curr = 144; port2_adjust_log = 1; break;
            default: break;
        }
    }
    printf(LOG"Power_Adjust_Hook() vol=%d port1Curr=%d port2Curr=%d \r\n", *vol, *port1Curr, *port2Curr);
    if(port1_adjust_log == 1)
    {
        printf("Power_Adjust_Hook() return TRUE\r\n");
        port1_adjust_log = 2;
        return TRUE;
    }
	if(port2_adjust_log == 1)
    {
        printf("Power_Adjust_Hook() return TRUE\r\n");
        port2_adjust_log = 2;
        return TRUE;
    }
	return FALSE; // ture 若原始值发生改变, false 原始值未发生改变
}
