#ifndef _SUNLORD_API_H_
#define _SUNLORD_API_H_
#include "system.h"

/**
 * 智融系列芯片应用程序接口
 * port：1对应port1、2对应port2
 */
struct SW_API_Func_t
{

	/**
	 * @brief 	设备初始化
	 * @return	u32 保留
	 */
	u32 (* SW_Init)(void);

	/**
	 * @brief 重新广播：SRC_CAP
	 * @param port 
	 * @param way	是否开启Emark检测
	 * @param time	保留
	 * @return u32 保留
	 */
	u32 (* SW_Rebroadcast_PDO)(u8 port, u8 way, u32 time);

	/**
	 * @brief 设置目标输出功率
	 * @param port 映射的端口编号；例如：1 对应该芯片的端口1
	 * @return u32 保留
	 */
	u32 (* SW_Set_Output_Power)(u8 port, u8 fast_charge_en, u16 power);

	/**
	 * @brief 设置输入母线电压
	 * @param port		保留
	 * @param setValue	母线电压值，单位：mV
	 * @return u32 保留
	 */	
	u32 (* SW_Set_Vin)(u8 port, u32 setValue);
	
	/**
	 * @brief 获取输出总线Vbus电压
	 * @param port		保留
	 * @param *vbusVol	保存变量地址，单位：mV 
	 * @return u32 保留
	 */	
	u32 (* SW_Get_Vbus)(u8 port, u16 *vbusVol);

	/**
	 * @brief 获取输出电流值
	 * @param port		保留
	 * @param *current	保存变量地址，单位：mA
	 * @return u32 保留
	 */
	u32 (* SW_Get_Current)(u8 port, u16 *current);

	/**
	 * @brief 获取NTC检测电阻值
	 * @param port			保留
	 * @param adc_channel	ADC通道
	 * @param res_value		NTC检测电阻端电压,保持变量地址
	 * @return u32 保留
	 */
	u32 (* SW_Get_NTC_Res)(u8 port, u8 adc_channel, u16 *res_value);

	/**
	 * @brief 获取端口插入/拔出状态
	 * @param port		映射的端口编号，从1开始计数
	 * @param portState	若端口存在插入事件，*portState值 对应 port bit位置1
	 * @return u32 	返回 1：对应port插入	返回 0：对应port拔出
	 */
	u32 (* SW_Get_Port_C_State)(u8 port, u8 *portState);

	/**
	 * @brief 获取输出功率
	 * @param port			映射的端口编号；例如：1 对应该芯片的端口1
	 * @param iout			输出电流指针，可输入NULL
	 * @param vout			输出电压指针，可输入NULL
	 * @param output_power	保持变量地址 单位：mW
	 * @return u32 保留
	 */
	u32 (* SW_Get_Output_Power)(u8 port, u16 *iout, u16 *vout, u16 *output_power);

	/**
	 * @brief 获取输出协议
	 * @param port			映射的端口编号；例如：1 对应该芯片的端口1
	 * @param protocol		保持变量地址
	 * @return u32 保留
	 */
	u32 (* SW_Get_Port_Protocol)(u8 port, u8 *protocol);

	// /**
	//  * @brief 执行自定义功能
	//  * @param port 映射的端口编号；例如：1 对应该芯片的端口1
	//  * @param func 自定义功能函数
	//  * @return: u32 保留
	//  */
	// u32 (* SW_User_Define)(u8 port, u32 *func);

	// /**
	//  * @brief 设置VID、PID值
	//  * @param port 
	//  * @param setVID
	//  * @param setPID
	//  * @return: u32 保留
	//  */
	// u32 (* SW_Set_VID_PID)(u8 port,u32 setVID,u32 setPID);

	// /**
	//  * @brief 设置端口输出限流值
	//  * @param port 
	//  * @param setValue	实际限流值，单位：mA
	//  * @return: u32 保留
	//  */
	// u32 (* SW_Set_Current_Limit)(u8 port, u16 setValue);

	// /**
	//  * @brief 设置端口无负载检测电流值
	//  * @param port 
	//  * @param setValue	空载检测电流阈值，单位：mA
	//  * @return: u32 保留
	//  */
	// u32 (* SW_Set_Unload_Current)(u8 port, u16 setValue);

	// /**
	//  * @brief 设置过温保护NTC阈值
	//  * @param port			保留
	//  * @param res_value		NTC过温电阻阈值，单位：mV
	//  * @return: u32 保留
	//  */	
	// u32 (* SW_Set_NTC_OTP_threshold)(u8 port, u16 res_value);

	// /**
	//  * @brief 获取输入母线电压
	//  * @param port		保留
	//  * @param *vinVol	保存变量地址，单位：mV
	//  * @return: u32 保留
	//  */	
	// u32 (* SW_Get_Vin)(u8 port, u16 *vinVol);
	
//	u32 (* SW_Set_QC)(u8 port,u8 qcVersion,u32 maxVol,u32 maxPower);
//	u32 (* SW_On_Off_QC)(u8 port,u8 version,u8 setState);	
//	u32 (* SW_On_Off_Emarker)(u8 port, u8 setState);
//	u32 (* SW_Dpdm_Power_Same_PD)(u8 port, u8 setState);
//	u32 (* SW_On_Off_Buck)(u8 port, u8 setState);
//	u32 (* SW_Set_Port_Mode)(u8 port, u8 mode);	
//	u32 (* SW_On_Off_Charge_Protocol)(u8 port,u8 num,u8 setState);
//	u32 (* SW_Set_PE)(u8 port, u32 maxVol);
//	u32 (* SW_On_Off_PE)(u8 port, u8 setState);
//	u32 (* SW_Set_PD_Gear)(u8 port, u8 *pwrAddr, u8 volGear, u8 setState); 
//	u32 (* SW_Set_PD_Vol_Cur)(u8 port,  u8 volGear,  u32 cur);
   
//	u32 (* SW_Set_UFCS_Gear)(u8 port, u8 *pwrAddr, u8 pos, u8 setState);	
//	u32 (* SW_isExist)(u8 port, u8 *existValue); 
//	u32 (* SW_I2C_Unlock)(u8 port);
//	u32 (* SW_Unlock_Force)(u8 port);
//	u32 (* SW_Unlock_High_Reg)(u8 port, u8 setState);//	
	
//	u32 (* SW_Set_FCP_XSCP)(u8 port, u8 fcpVersion, u32 maxVol, u32 maxPower);
//	u32 (* SW_On_Off_FCP_XSCP)(u8 port, u8 fcpVersion, u8 setState);
};



#endif

