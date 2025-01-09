#include "sunlord_api.h"
#include "func_power_alloc.h"
#include "func_hardware_api.h"
#include "api.h"

static u32 SW3566_Set_PD_Gear(u8 port,  u8 *pwrAddr, u8 volGear,  u8 setState);
static u32 SW3566_Set_PD_Vol_Cur(u8 port,  u8 volGear,  u32 cur);
static u32 SW3566_Set_UFCS_Gear(u8 port, u8 *pwrAddr, u8 pos, u8 setState); 
static u32 SW3566_Set_VID_PID(u8 port,u32 setVID,u32 setPID);
static u32 SW3566_Set_QC(u8 port,u8 qcVersion,u32 maxVol,u32 maxPower);
static u32 SW3566_On_Off_QC(u8 port,u8 version,u8 setState);
static u32 SW3566_Set_FCP_XSCP(u8 port, u8 fcpVersion, u32 maxVol, u32 maxPower);
static u32 SW3566_On_Off_FCP_XSCP(u8 port, u8 fcpVersion, u8 setState);
static u32 SW3566_Rebroadcast_PDO(u8 port, u8 way, u32 time);//
static u32 SW3566_On_Off_Charge_Protocol(u8 port,u8 num,u8 setState);
static u32 SW3566_Set_PE(u8 port, u32 maxVol);
static u32 SW3566_On_Off_PE(u8 port, u8 setState);
static u32 SW3566_Set_Line_Com (u8 port, u8 setState,  u32 setValue);//
static u32 SW3566_Set_Current_Limit(u8 port, u8 num,u32 setValue);
static u32 SW3566_Set_Unload_Current(u8 port, u8 num, u32 setValue);//
static u32 SW3566_Set_NTC_OTP_Threshold(u8 port, u32 res_value);//
static u32 SW3566_Set_Vin(u8 port, u32 setValue);//
static u32 SW3566_Get_Vin(u8 port, u32 *vinVol);
static u32 SW3566_Get_Vbus(u8 port, u16 *vbusVol);
static u32 SW3566_Get_Current(u8 port, u16 *current);
static u32 SW3566_Get_NTC_Res(u8 port, u8 adc_channel, u16 *res_value);//
static u32 SW3566_Get_Port_C_State(u8 port, u8 *portState);
static u32 SW3566_Get_Output_Power(u8 port, u16 *iout, u16 *vout, u16 *sysPower);
static u32 SW3566_Get_Port_Protocol(u8 port,  u8 *protocol);
static u32 SW3566_On_Off_Emarker(u8 port, u8 setState);
static u32 SW3566_Dpdm_Power_Same_PD(u8 port,u8 setState);//
static u32 SW3566_On_Off_Buck(u8 port, u8 setState);
static u32 SW3566_I2C_Unlock(u8 port);
static u32 SW3566_Unlock_Force (u8 port);
static u32 SW3566_Unlock_High_Reg(u8 port, u8 setState);//
static u32 SW3566_Set_Port_Mode(u8 port, u8 mode);

static u32 SW3566_Init(void);
static u32 SW3566_isExist(u8 port, u8 *existValue);
static u32 SW3566_Set_Output_Power(u8 port, u8 fast_charge_en, u16 power);
static u32 SW3566_User_Define(u8 port, u8 funcNo, u32 *dat);

/**
 * @brief 
 * @param port 从1开始计数
 * 
 */
const struct SW_API_Func_t SW3566_Func = 
{
	.SW_Init = SW3566_Init,	
	.SW_Rebroadcast_PDO = SW3566_Rebroadcast_PDO,
	.SW_Set_Output_Power  = SW3566_Set_Output_Power,
	.SW_Set_Vin = SW3566_Set_Vin,
	
	.SW_Get_Vbus = SW3566_Get_Vbus,
	.SW_Get_Current = SW3566_Get_Current, 
	.SW_Get_NTC_Res = SW3566_Get_NTC_Res,
	.SW_Get_Port_C_State = SW3566_Get_Port_C_State,
	.SW_Get_Output_Power = SW3566_Get_Output_Power,	
	.SW_Get_Port_Protocol = SW3566_Get_Port_Protocol,
	
	// .SW_User_Define = SW3566_User_Define,
	
	// .SW_Set_Current_Limit = SW3566_Set_Current_Limit,
	// .SW_Set_Unload_Current = SW3566_Set_Unload_Current,
	// .SW_Set_NTC_OTP_threshold = SW3566_Set_NTC_OTP_Threshold,

	// .SW_Get_Vin = SW3566_Get_Vin,



//	.SW_On_Off_Emarker = SW3566_On_Off_Emarker,
//	.SW_Dpdm_Power_Same_PD = SW3566_Dpdm_Power_Same_PD,
//	.SW_On_Off_Buck = SW3566_On_Off_Buck,
//	.SW_I2C_Unlock = SW3566_I2C_Unlock,
//	.SW_Unlock_Force = SW3566_Unlock_Force,
//	.SW_Unlock_High_Reg = SW3566_Unlock_High_Reg, 
//	.SW_Set_Port_Mode = SW3566_Set_Port_Mode,
//	.SW_Set_PD_Gear = SW3566_Set_PD_Gear, 
//	.SW_Set_PD_Vol_Cur = SW3566_Set_PD_Vol_Cur,	
//   
//	.SW_Set_UFCS_Gear = SW3566_Set_UFCS_Gear,	
	
//	.SW_Set_VID_PID = SW3566_Set_VID_PID,
//	.SW_Set_QC = SW3566_Set_QC,
//	.SW_On_Off_QC = SW3566_On_Off_QC,
	
//	.SW_Set_FCP_XSCP = 	SW3566_Set_FCP_XSCP,
//	.SW_On_Off_FCP_XSCP = SW3566_On_Off_FCP_XSCP,
//	.SW_On_Off_Charge_Protocol = SW3566_On_Off_Charge_Protocol,
//	.SW_Set_PE = SW3566_Set_PE,
//	.SW_On_Off_PE = SW3566_On_Off_PE,
//	.SW_isExist = SW3566_Is_Exist,
};



static u32 SW3566_Set_PD_Gear(u8 port,  u8 *pwrAddr, u8 volGear,  u8 setState)
{
	return 1;
}

static u32 SW3566_Set_PD_Vol_Cur(u8 port,  u8 volGear,  u32 cur)
{
	return 1;
}


static u32 SW3566_Set_UFCS_Gear(u8 port, u8 *pwrAddr, u8 pos, u8 setState)
{
	return 2;
}

static u32 SW3566_Set_VID_PID(u8 port,u32 setVID,u32 setPID)
{
	return 1;
}

static u32 SW3566_Set_QC(u8 port,u8 qcVersion,u32 maxVol,u32 maxPower)//QC
{
	return 1;
}

static u32 SW3566_On_Off_QC(u8 port,u8 version,u8 setState)//QC version:1=QC2.0   2=QC3.0
{	
	return 1;
}

static u32 SW3566_Set_FCP_XSCP (u8 port, u8 fcpVersion, u32 maxVol, u32 maxPower)
{
	return 1;
}

static u32 SW3566_On_Off_FCP_XSCP(u8 port, u8 fcpVersion, u8 setState)//fcp:3  scp:4
{
	return 1;
}

static u32 SW3566_Rebroadcast_PDO(u8 port, u8 way, u32 time)//
{
	//是否开启emark检测
	Pd_Src_Cap_Ignore_Emarker(way);
	//PD重新广播
	Pd_Rebroadcast_Src_Cap();
	return 1;
}

static u32 SW3566_On_Off_Charge_Protocol(u8 port,u8 num,u8 setState)
{
	return 2;
}

static u32 SW3566_Set_PE(u8 port, u32 maxVol)
{
	return 1;
}

static u32 SW3566_On_Off_PE(u8 port, u8 setState)
{
	return 1;
}

static u32 SW3566_Set_Line_Com(u8 port,u8 setState, u32 setValue)
{
	return 1;
}

static u32 SW3566_Set_Current_Limit(u8 port, u8 num, u32 setValue)
{
	return 1;
}

static u32 SW3566_Set_Unload_Current(u8 port, u8 num, u32 setValue)
{
	return 2;
}

/**
 * @brief 设置过温保护NTC阈值
 * 
 * @param port 
 * @param res_value NTC过温电阻值
 * @return u32 
 */
static u32 SW3566_Set_NTC_OTP_Threshold(u8 port, u32 res_value)
{
	
	return 2;
}

static u32 SW3566_Set_Vin(u8 port, u32 setValue)
{
	return 2;
}

static u32 SW3566_Get_Vin(u8 port, u32 *vinVol)
{
	*vinVol = Adc_Vin_Get();
	return 1;
}

static u32 SW3566_Get_Vbus(u8 port, u16 *vbusVol)
{
	MY_PRINTF("V_in\r\n");
	*vbusVol = Adc_Vout_Get();
	MY_PRINTF("Vbus= %d\r\n", *vbusVol);
	return 1;
}
	
static u32 SW3566_Get_Current(u8 port, u16 *current)
{
	*current = Adc_Iout_Get(port);
	return 1;
}

//此处返回值为电阻值	
static u32 SW3566_Get_NTC_Res(u8 port, u8 adc_channel, u16 *res_value)
{
	u32 tmpData = 0;
	tmpData = Adc_Get_Channel_Data(adc_channel);								//user get channel ADC data unit:mV
	*res_value = tmpData / *(Hardware_Driver_Data.NTC_Data.NTC_Pullup_Current);	// 单位：K
	return *res_value;
}

static u32 SW3566_Get_Port_C_State(u8 port, u8 *portState)
{
	*portState = Device_Get_Port_State() & (0x01 << (port - 1));
	if(0 != *portState)
	{
		return 1;
	}
	return 0;
}

static u32 SW3566_Get_Output_Power(u8 port, u16 *iout, u16 *vout, u16 *sysPower)
{
	uint16_t v_out=0, i_out=0;
	port -= 1;						// 已满足 Adc_Iout_Get 参数0/1的要求
	v_out = Adc_Vout_Get();
	i_out = Adc_Iout_Get(port);
	if(vout != NULL)
	{
		*vout = v_out;
	}

	if(iout != NULL)
	{
		*iout = i_out;
	}
	*sysPower = v_out * i_out / 1000; // 单位：mW
	return *sysPower;
}

static u32 SW3566_Get_Port_Protocol(u8 port,  u8 *protocol)
{
	*protocol = Device_Get_Protocol(port);
	return *protocol;
}

static u32 SW3566_On_Off_Emarker(u8 port, u8 setState)
{
	return 1;
}

	
static u32 SW3566_Dpdm_Power_Same_PD(u8 port,u8 setState)
{
	return 2;
}

static u32 SW3566_On_Off_Buck(u8 port, u8 setState)
{
	return 1;
}

static u32 SW3566_I2C_Unlock(u8 port)
{
	return 2;
}

static u32 SW3566_Unlock_Force (u8 port)
{
	return 2;
}

static u32 SW3566_Unlock_High_Reg(u8 port, u8 setState)
{
	return 2;
}
	
static u32 SW3566_Set_Port_Mode(u8 port, u8 mode)
{
	return 1;
}

static u32 SW3566_Init(void)
{
	//NTC初始化
//	Adc_Enabled();
//	adc_config_t adcCfg;
//	//Gpio_Function_Select(GPIO2, GPIO2_FUNC_SEL_ADC);
//	Adc_Get_Config(&adcCfg);
//	adcCfg.adc3Pullup = ADC_PULLUP_CURRENT_20UA;//100KΩ（≈1.9V-2V）
//	adcCfg.adc3Source = ADC3_SIG_SRC_CC12,//singal source should equal channel
//	Adc_Set_Config(&adcCfg);
//	Adc_Start_Adc_Channel(ADC_CHL_3);//channel should equal singal source	
	return 1;
}

static u32 SW3566_isExist(u8 port, u8 *existValue)
{
	*existValue = 1;
	return 2;
}

static u32 SW3566_Set_Output_Power(u8 port, u8 fast_charge_en, u16 power)
{
	Func_Power_Alloc(fast_charge_en, power);
	return 1;
}

static u32 SW3566_User_Define(u8 port, u8 funcNo, u32 *dat)
{	
	return 1;
}

