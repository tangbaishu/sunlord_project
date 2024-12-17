#include "power_business.h"

#include "zr_systick.h"

#include "api.h"			// 快充相关的api接口
#include "config.h"			// 快充配置结构体
#include "ufcs_caps.h"		// ufcs 输出能力配置
#include "pdo.h"			// PDO广播包
#include "serial.h"			// 串口打印
#include "pd_message.h"		// PD消息包
#include "sink_policy.h"	// 受电端供应? 后续需重点阅读理解
#include "user_pd_policy.h"	// 用户PD策略 后续需重点阅读理解

static config_data_t* Power_Policy_Config;

static const ufcs_output_cap_t UFCS_75W_Output_Cap[]={
{
	.fields.head = UFCS_CAP_SET_HEAD(UFCS_CAP_ID1, UFCS_CURR_STEP_10MA, UFCS_VOL_MIN_STEP_10MV),
	.fields.hiMaxVolage = UFCS_CAP_SET_HI_BYTE(500),	// unit: 10mV
	.fields.loMaxVolage = UFCS_CAP_SET_LO_BYTE(500),	// unit: 10mV
	.fields.hiMinVolage = UFCS_CAP_SET_HI_BYTE(350),	// uint: 10mV
	.fields.loMinVolage = UFCS_CAP_SET_LO_BYTE(350),	// uint: 10mV
	.fields.hiMaxCurrent = UFCS_CAP_SET_HI_BYTE(300),	// uint: 10mA
	.fields.loMaxCurrent = UFCS_CAP_SET_LO_BYTE(300),	// uint: 10mA
	.fields.minCurrent = 100,							// uint: 10mA
},
{
	.fields.head = UFCS_CAP_SET_HEAD(UFCS_CAP_ID2, UFCS_CURR_STEP_10MA, UFCS_VOL_MIN_STEP_10MV),
	.fields.hiMaxVolage = UFCS_CAP_SET_HI_BYTE(1100),	// unit: 10mV
	.fields.loMaxVolage = UFCS_CAP_SET_LO_BYTE(1100),	// unit: 10mV
	.fields.hiMinVolage = UFCS_CAP_SET_HI_BYTE(500),	// uint: 10mV
	.fields.loMinVolage = UFCS_CAP_SET_LO_BYTE(500),	// uint: 10mV
	.fields.hiMaxCurrent = UFCS_CAP_SET_HI_BYTE(300),	// uint: 10mA
	.fields.loMaxCurrent = UFCS_CAP_SET_LO_BYTE(300),	// uint: 10mA
	.fields.minCurrent = 100,							// uint: 10mA
},
{
	.fields.head = UFCS_CAP_SET_HEAD(UFCS_CAP_ID3, UFCS_CURR_STEP_10MA, UFCS_VOL_MIN_STEP_10MV),
	.fields.hiMaxVolage = UFCS_CAP_SET_HI_BYTE(2000),	// unit: 10mV
	.fields.loMaxVolage = UFCS_CAP_SET_LO_BYTE(2000),	// unit: 10mV
	.fields.hiMinVolage = UFCS_CAP_SET_HI_BYTE(1100),	// uint: 10mV
	.fields.loMinVolage = UFCS_CAP_SET_LO_BYTE(1100),	// uint: 10mV
	.fields.hiMaxCurrent = UFCS_CAP_SET_HI_BYTE(350),	// uint: 10mA
	.fields.loMaxCurrent = UFCS_CAP_SET_LO_BYTE(350),	// uint: 10mA
	.fields.minCurrent = 100,							// uint: 10mA
}
};

static const ufcs_output_cap_t UFCS_45W_Output_Cap[] ={
{
	.fields.head = UFCS_CAP_SET_HEAD(UFCS_CAP_ID1, UFCS_CURR_STEP_10MA, UFCS_VOL_MIN_STEP_10MV),
	.fields.hiMaxVolage = UFCS_CAP_SET_HI_BYTE(500),	// unit: 10mV
	.fields.loMaxVolage = UFCS_CAP_SET_LO_BYTE(500),	// unit: 10mV
	.fields.hiMinVolage = UFCS_CAP_SET_HI_BYTE(350),	// uint: 10mV
	.fields.loMinVolage = UFCS_CAP_SET_LO_BYTE(350),	// uint: 10mV
	.fields.hiMaxCurrent = UFCS_CAP_SET_HI_BYTE(300),	// uint: 10mA
	.fields.loMaxCurrent = UFCS_CAP_SET_LO_BYTE(300),	// uint: 10mA
	.fields.minCurrent = 100,							// uint: 10mA
},
{
	.fields.head = UFCS_CAP_SET_HEAD(UFCS_CAP_ID2, UFCS_CURR_STEP_10MA, UFCS_VOL_MIN_STEP_10MV),
	.fields.hiMaxVolage = UFCS_CAP_SET_HI_BYTE(1100),	// unit: 10mV
	.fields.loMaxVolage = UFCS_CAP_SET_LO_BYTE(1100),	// unit: 10mV
	.fields.hiMinVolage = UFCS_CAP_SET_HI_BYTE(500),	// uint: 10mV
	.fields.loMinVolage = UFCS_CAP_SET_LO_BYTE(500),	// uint: 10mV
	.fields.hiMaxCurrent = UFCS_CAP_SET_HI_BYTE(300),	// uint: 10mA
	.fields.loMaxCurrent = UFCS_CAP_SET_LO_BYTE(300),	// uint: 10mA
	.fields.minCurrent = 100,							// uint: 10mA
},
{
	.fields.head = UFCS_CAP_SET_HEAD(UFCS_CAP_ID3, UFCS_CURR_STEP_10MA, UFCS_VOL_MIN_STEP_10MV),
	.fields.hiMaxVolage = UFCS_CAP_SET_HI_BYTE(2000),	// unit: 10mV
	.fields.loMaxVolage = UFCS_CAP_SET_LO_BYTE(2000),	// unit: 10mV
	.fields.hiMinVolage = UFCS_CAP_SET_HI_BYTE(1100),	// uint: 10mV
	.fields.loMinVolage = UFCS_CAP_SET_LO_BYTE(1100),	// uint: 10mV
	.fields.hiMaxCurrent = UFCS_CAP_SET_HI_BYTE(230),	// uint: 10mA
	.fields.loMaxCurrent = UFCS_CAP_SET_LO_BYTE(230),	// uint: 10mA
	.fields.minCurrent = 100,							// uint: 10mA
}
};

static const uint32_t PD_70W_Output_Cap[]=
{
	PDO_FIXED_VSAFE5V(3000, (PDO_FIXED_DUAL_POWER_ROLE| PDO_FIXED_UNCONSTRAINED_POWER |\
							 PDO_FIXED_COMM_CAP | PDO_FIXED_DUAL_DATA_ROLE), PDO_FIXED_PEAK_CURRENT_MODE1),
	PDO_FIXED_OTHERS(9000, 3000, PDO_FIXED_PEAK_CURRENT_MODE1),
	PDO_FIXED_OTHERS(12000, 3000, PDO_FIXED_PEAK_CURRENT_MODE1),
	PDO_FIXED_OTHERS(15000, 3000, PDO_FIXED_PEAK_CURRENT_MODE1),
	PDO_FIXED_OTHERS(20000, 3500, PDO_FIXED_PEAK_CURRENT_MODE1),
	PDO_PPS(3500, 5000, 20000, PDO_SPR_PPS_POWER_LIMITED)
};


void Power_Business_Init(void)
{
	Power_Policy_Config = Config_Get();
	
	Power_Policy_Config->pdSrcCap = (const uint32_t*)PD_70W_Output_Cap;
	Power_Policy_Config->xid = 0;
	Power_Policy_Config->vid = 0;
	Power_Policy_Config->pid = 0x1234;
	Power_Policy_Config->bcdDevice = 0;
	Power_Policy_Config->pdMaxPower = 70;
	Power_Policy_Config->tfcpMaxPower = 70;
	Power_Policy_Config->sprPdoNumber = 6;		// 标准功率范围 总PDO（包含PPS）
	Power_Policy_Config->sprFixPdoNumber = 5;	// 标准功率范围 固定PDO
	Power_Policy_Config->eprPdoNumber = 0;		// 扩展功率范围 总PDO
	Power_Policy_Config->eprFixPdoNumber = 0;	// 扩展功率范围 固定PDO

	Power_Policy_Config->ufcsSrcCap = (const uint8_t*)UFCS_75W_Output_Cap;
	Power_Policy_Config->ufcsSrcCapNum = 3;

	// Power_Policy_Config->scpMaxCurrent = 23;
	// Power_Policy_Config->scpMaxPower = 70;
	// Power_Policy_Config->scpMaxVoltage = 20;
	
	Power_Policy_Config->portMode = DUO_CC_MODE;
	Power_Policy_Config->typecCcTerm = RP_3A;
	Power_Policy_Config->fixVoltageOffset = FIX_VOLTAGE_OFFSET_0MV;
	Power_Policy_Config->fixCurrentOffset = FIX_CURRENT_OFFSET_300MA;

	Power_Policy_Config->p1CurrentLimit = PORT_CURR_LIMIT_3P0A;
	Power_Policy_Config->p2CurrentLimit = PORT_CURR_LIMIT_3P0A;

	Power_Policy_Config->cableCompensationValue = CABLE_COMPENSAVE_VALUE_65MOHM;
	Power_Policy_Config->isCableCompensaved = ENABLE;
	Power_Policy_Config->isTypecVconnEnabled = ENABLE;
	Power_Policy_Config->isVconnSwapEnabled = ENABLE;
	Power_Policy_Config->isResponseDiscoveryId =  ENABLE;

	Power_Policy_Config->isPdGatingVoocEnabled = ENABLE;
	Power_Policy_Config->isPdGatingScpEnabled= ENABLE;
	Power_Policy_Config->isEmarkEnabled = ENABLE;
	Power_Policy_Config->isPdEnabled = ENABLE;
	
	Power_Policy_Config->isVconnLowPowerModeEnabled = ENABLE;

}

void Port_Scan(void)
{
	
}
