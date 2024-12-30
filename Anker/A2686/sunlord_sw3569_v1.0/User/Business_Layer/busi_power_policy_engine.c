#include "busi_power_policy_engine.h"
#include "func_hardware_init.h"
#include "busi_port_detection.h"
#include "busi_fast_charge_config.h"
#include "zr_systick.h"

Busi_Power_Policy_Data_t Busi_Power_Policy_Data;

/**
 * @brief 过温处理
 */
static void OverTemp_Dispose(config_data_t *config_data);

/**
 * @brief 按需功率分配逻辑
 */
static void On_Demand_Power_Policy(config_data_t *config_data);

void Real_Time_Allocation_Power(config_data_t *config_data)
{
	static bool power_on_flag = 0;	// 首次上电，选择 POWER_PDO_5W,即关闭所有协议,待获取各个端口状态后在切换 PDO (防止双口先插入，AC上电时，单口广播70W)
	if(power_on_flag == 0)	
	{
		PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_5W);
		PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_5W);
		power_on_flag = 1;
		return;
	}
	// 已通过 Busi_Port_Detection_Init() 更新各个端口插入状态
	if(Busi_Port_Detection_Data.Port_State_Updata == true)
	{
		Busi_Port_Detection_Data.Port_State_Updata = false;
		// 若任意 端口 拔出
		if( (Busi_Port_Detection_Data.Port1_Out_Protocol_Data.Port_Link_State == Port_1_Pull_Out)	||\
				(Busi_Port_Detection_Data.Port2_Out_Protocol_Data->Port_Link_State == Port_2_Pull_Out) )	
		{
			PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_70W);
			PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_70W);
			Busi_Power_Policy_Data.Software_Timer_EN = 0;
			Busi_Power_Policy_Data.Two_Stage_Check_Finish_Flag = 0;
		}
		else	// 若双口同插
		{
			// 两个端口均广播35W
			PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_35W);
			PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_35W);
		}
		PORT_POWER_SETING_RIGHT_AWAY(); // 配置的PDO参数，立即生效
	}

	// 若双口同插
	if( (Busi_Port_Detection_Data.Port1_Out_Protocol_Data.Port_Link_State == Port_1_Insert)	&&\
				(Busi_Port_Detection_Data.Port2_Out_Protocol_Data->Port_Link_State == Port_2_Insert) )	
	{
		if(0 == Busi_Power_Policy_Data.Two_Stage_Check_Finish_Flag) // 按需分配功率
		{
			On_Demand_Power_Policy(config_data);	
		}
		if(( *Port_BSP_Data.Port1_OverTemp_Dispose_Flag == UNDISPOSED ) || ( *Port_BSP_Data.Port2_OverTemp_Dispose_Flag == UNDISPOSED ))
		{
			// 若双口同插，且任意口检测到过温
			if( (OVERTEMP == (NTC_Temp_State_e)*Busi_Port_Detection_Data.P_Port_BSP_Data->Port1_NTC_Temp_State) || \
						(OVERTEMP == (NTC_Temp_State_e)*Busi_Port_Detection_Data.P_Port_BSP_Data->Port2_NTC_Temp_State) )
			{
				OverTemp_Dispose(config_data);
				PORT_POWER_SETING_RIGHT_AWAY(); // 配置的PDO参数，立即生效
				*Port_BSP_Data.Port1_OverTemp_Dispose_Flag = PROCESSED;
				*Port_BSP_Data.Port2_OverTemp_Dispose_Flag = PROCESSED;
			}
		}
	}
}

/**
 * @brief 过温处理
 */
static void OverTemp_Dispose(config_data_t *config_data)
{
	// 端口1过温处理
	switch (Busi_Port_Detection_Data.Port1_Out_Protocol_Data.Current_Power_Grade)
	{
		case POWER_PDO_35W : PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_25W);PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_25W);break;
		case POWER_PDO_65W : PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_45W);PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_5W);break;
		case POWER_PDO_5W : PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_5W);PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_45W);break;
		case POWER_PDO_45W : PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_25W);PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_25W);break;
		case POWER_PDO_25W : PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_25W);PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_25W);break;
		default:	break;
	}
}

/**
 * @brief 按需功率分配逻辑
 */
static void On_Demand_Power_Policy(config_data_t *config_data)
{
	if(Busi_Power_Policy_Data.Software_Timer_EN == 0)
	{
		Busi_Power_Policy_Data.Software_Timer_EN = 1;
		Busi_Power_Policy_Data.Device_Type_Check_Timer = 0;
		Busi_Power_Policy_Data.Output_Voltage_Sample_Timer = 0;
		Busi_Power_Policy_Data.One_Stage_Check_Finish_Flag = 0;
		Busi_Power_Policy_Data.Two_Stage_Check_Finish_Flag = 0;
		Busi_Power_Policy_Data.Port_1_Output_Voltage_Peak = 0;
		Busi_Power_Policy_Data.Port_2_Output_Voltage_Peak = 0;
	}
	// 等待设备类型检测走完 一阶段
	if( 0 == Busi_Power_Policy_Data.One_Stage_Check_Finish_Flag )
	{
		if( ONE_STAGE_CHECK_TIME >= Busi_Power_Policy_Data.Device_Type_Check_Timer )
		{
			return;
		}
		Busi_Power_Policy_Data.One_Stage_Check_Finish_Flag = 1;
		if(Busi_Power_Policy_Data.Device_Type_Check_Timer >= 60000)	// 防止定时器值即将溢出时，赋值给其它变量造成异常
		{
			Busi_Power_Policy_Data.Device_Type_Check_Timer = 0;
		}
		Busi_Power_Policy_Data.Output_Voltage_Sample_Timer = Busi_Power_Policy_Data.Device_Type_Check_Timer;
	}

	// 基于 OUTPUT_VOLTAGE_SAMPLE_RATE 周期性采样
	if( OUTPUT_VOLTAGE_SAMPLE_RATE <= ( Busi_Power_Policy_Data.Device_Type_Check_Timer - Busi_Power_Policy_Data.Output_Voltage_Sample_Timer))
	{
		Busi_Power_Policy_Data.Output_Voltage_Sample_Timer = Busi_Power_Policy_Data.Device_Type_Check_Timer;
		if( Busi_Power_Policy_Data.Port_1_Output_Voltage_Peak < GET_PORT1_OUTPUT_VOLTAGE() )
		{
			Busi_Power_Policy_Data.Port_1_Output_Voltage_Peak = GET_PORT1_OUTPUT_VOLTAGE();
		}

		if( Busi_Power_Policy_Data.Port_2_Output_Voltage_Peak < GET_PORT2_OUTPUT_VOLTAGE() )
		{
			Busi_Power_Policy_Data.Port_2_Output_Voltage_Peak = GET_PORT2_OUTPUT_VOLTAGE();
		}
	}
	// 等待设备类型检测走完 二阶段
	if( TWO_STAGE_CHECK_TIME >= Busi_Power_Policy_Data.Device_Type_Check_Timer )
	{
		return;
	}
	Busi_Power_Policy_Data.Two_Stage_Check_Finish_Flag = 1;

	if( 18 < Busi_Power_Policy_Data.Port_1_Output_Voltage_Peak)	// Port1 大功率设备
	{
		if( 18 < Busi_Power_Policy_Data.Port_2_Output_Voltage_Peak )	// Port2 大功率设备
		{
			PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_35W);
			PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_35W);
		}
		else if( ( 7 < Busi_Power_Policy_Data.Port_2_Output_Voltage_Peak) && (Busi_Power_Policy_Data.Port_2_Output_Voltage_Peak < 17) )	// Port2 中功率设备
		{
			PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_45W);
			PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_25W);
		}
		else if( Busi_Power_Policy_Data.Port_2_Output_Voltage_Peak < 6 )	// Port2 小功率设备
		{
			PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_65W);
			PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_5W);
		}
	}
	else if( ( 7 < Busi_Power_Policy_Data.Port_1_Output_Voltage_Peak) && (Busi_Power_Policy_Data.Port_1_Output_Voltage_Peak < 17) )		// Port1 中功率设备
	{
		if( 18 < Busi_Power_Policy_Data.Port_2_Output_Voltage_Peak )	// Port2 大功率设备
		{
			PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_25W);
			PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_45W);
		}
		else if( ( 7 < Busi_Power_Policy_Data.Port_2_Output_Voltage_Peak) && (Busi_Power_Policy_Data.Port_2_Output_Voltage_Peak < 17) )	// Port2 中功率设备
		{
			PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_35W);
			PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_35W);
		}
		else if( Busi_Power_Policy_Data.Port_2_Output_Voltage_Peak < 6 )	// Port2 小功率设备
		{
			PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_45W);
			PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_25W);
		}
	}
	else if( Busi_Power_Policy_Data.Port_1_Output_Voltage_Peak < 6 )		// Port1 小功率设备
	{
		if( 18 < Busi_Power_Policy_Data.Port_2_Output_Voltage_Peak )	// Port2 大功率设备
		{
			PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_5W);
			PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_65W);
		}
		else if( ( 7 < Busi_Power_Policy_Data.Port_2_Output_Voltage_Peak) && (Busi_Power_Policy_Data.Port_2_Output_Voltage_Peak < 17) )	// Port2 中功率设备
		{
			PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_25W);
			PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_45W);
		}
		else if( Busi_Power_Policy_Data.Port_2_Output_Voltage_Peak < 6 )	// Port2 小功率设备
		{
			PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_35W);
			PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_35W);
		}
	}
	// 当前两个端口充电协议都为PPS
	if( ( FC_PD_PPS == Busi_Port_Detection_Data.Port1_Out_Protocol_Data.Protocol_Type ) && ( FC_PD_PPS == Busi_Port_Detection_Data.Port2_Out_Protocol_Data->Protocol_Type ) )
	{
		if( Busi_Port_Detection_Func.Port1_RealTime_Out_Power_Func().Power >  Busi_Port_Detection_Func.Port2_RealTime_Out_Power_Func().Power )
		{
			PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_45W);
			PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_25W);
		}
		else
		{
			PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_25W);
			PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_45W);
		}
		// 注：若当前输出功率相同，则维持之前大、中、小功率配置
	}
	else
	{
		if( ( FC_PD_PPS == Busi_Port_Detection_Data.Port1_Out_Protocol_Data.Protocol_Type ) )
		{
			PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_45W);
			PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_25W);
		}
		else if( ( FC_PD_PPS == Busi_Port_Detection_Data.Port2_Out_Protocol_Data->Protocol_Type ) )
		{
			PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_25W);
			PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_45W);
		}
	}
	PORT_POWER_SETING_RIGHT_AWAY(); // 配置的PDO参数，立即生效
}