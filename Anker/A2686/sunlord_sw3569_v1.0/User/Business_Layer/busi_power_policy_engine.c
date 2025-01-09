#include "busi_power_policy_engine.h"
#include "func_hardware_api.h"
#include "busi_port_detection.h"
#include "func_power_alloc.h"
#include "busi_share_memory.h"
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
		PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_35W);
		PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_35W);
		power_on_flag = 1;
		return;
	}
	// 已通过 Busi_Port_Detection_Init() 更新各个端口插入状态
	// if(Busi_Port_Detection_Data.Port_State_Updata == true)
	// {
	// 	Busi_Port_Detection_Data.Port_State_Updata = false;

		if((I2C_SW3566_Reg_Data[0].Port_State)	&& (I2C_SW3566_Reg_Data[1].Port_State))	// 若双口同插
		{
			// 两个端口均广播35W
			PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_35W);
			PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_35W);
			// 若双口同插
			if(0 == Busi_Power_Policy_Data.Two_Stage_Check_Finish_Flag) // 按需分配功率
			{
				On_Demand_Power_Policy(config_data);	
			}
			if(( *Hardware_BSP_Data.Port1_OverTemp_Dispose_Flag == UNDISPOSED ) || ( *Hardware_BSP_Data.Port2_OverTemp_Dispose_Flag == UNDISPOSED ))
			{
				// 若双口同插，且任意口检测到过温
				if( (OVERTEMP == (NTC_Temp_State_e)*Busi_Port_Detection_Data.P_Port_BSP_Data->Port1_NTC_Temp_State) || \
							(OVERTEMP == (NTC_Temp_State_e)*Busi_Port_Detection_Data.P_Port_BSP_Data->Port2_NTC_Temp_State) )
				{
					OverTemp_Dispose(config_data);
					PORT_POWER_SETING_RIGHT_AWAY(); // 配置的PDO参数，立即生效
					*Hardware_BSP_Data.Port1_OverTemp_Dispose_Flag = PROCESSED;
					*Hardware_BSP_Data.Port2_OverTemp_Dispose_Flag = PROCESSED;
				}
			}
		}
		else if((I2C_SW3566_Reg_Data[0].Port_State)	|| (I2C_SW3566_Reg_Data[1].Port_State))// 若单口插入
		{
			PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_70W);
			PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_70W);
			Busi_Power_Policy_Data.Software_Timer_EN = 0;
			Busi_Power_Policy_Data.Two_Stage_Check_Finish_Flag = 0;
		}
		
		PORT_POWER_SETING_RIGHT_AWAY(); // 配置的PDO参数，立即生效
		
	// }
}

/**
 * @brief 过温处理
 */
static void OverTemp_Dispose(config_data_t *config_data)
{
	static uint8_t num = 0;
	// 端口1过温处理
	switch (I2C_SW3566_Reg_Data[0].Port_Real_Power)
	{
		case POWER_PDO_35W : PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_25W);PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_25W);break;
		case POWER_PDO_65W : PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_45W);PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_5W);break;
		case POWER_PDO_5W : PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_5W);PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_45W);break;
		case POWER_PDO_45W : PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_25W);PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_25W);break;
		case POWER_PDO_25W : PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_25W);PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_25W);break;
		default:	break;
	}
	num++;
	if(num >= SYS_PORT_MAX_NUMBER)
	{
		num = 0;
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
	if( ( FC_PD_PPS == Busi_Port_Detection_Data.Port_Out_Protocol_Data.Port_Info[0].Protocol_Type ) && ( FC_PD_PPS == Busi_Port_Detection_Data.Port_Out_Protocol_Data.Port_Info[1].Protocol_Type ) )
	{
		if( Busi_Port_Detection_Func.Port_RealTime_Out_Power(1).Power >  Busi_Port_Detection_Func.Port_RealTime_Out_Power(2).Power )
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
		if( ( FC_PD_PPS == Busi_Port_Detection_Data.Port_Out_Protocol_Data.Port_Info[0].Protocol_Type ) )
		{
			PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_45W);
			PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_25W);
		}
		else if( ( FC_PD_PPS == Busi_Port_Detection_Data.Port_Out_Protocol_Data.Port_Info[1].Protocol_Type ) )
		{
			PORT1_OUTPUT_POWER_POLICY(config_data, POWER_PDO_25W);
			PORT2_OUTPUT_POWER_POLICY(config_data, POWER_PDO_45W);
		}
	}
	PORT_POWER_SETING_RIGHT_AWAY(); // 配置的PDO参数，立即生效
}