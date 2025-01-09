#include "busi_port_detection.h"
#include "func_hardware_api.h"
#include "device_driver.h"
#include "zr_adc.h"
#include "api.h"

#define PORT_SUB					1	// 端口下标
#define PORT_INSETR_THRESHOLD		5	// 端口连续识别到插入次数
#define PORT_SCAN_RATE				25	// 单位：ms
#define PORT_INFO_GATHER_RATE		100	// 单位：ms

#if defined(DEBUG_PRINTF_OPEN)
    #include "stdio.h"
    #define LOG "busi_p_d: "
#endif

Port_Detection_Data_t	Busi_Port_Detection_Data={
	.Self_Reg_Data = &I2C_SW3566_Reg_Data[0],	// 每个设备都默认将寄存器0块存储区，作为自身设备寄存器存储区，主机会基于从机排序逻辑，向后顺延映射从机存储区
};

extern struct SW_API_Func_t SW3566_Func;
Busi_Port_Detection_Func_t Busi_Port_Detection_Func={
	.P_SW_API_Func = &SW3566_Func,
	.P_Get_Out_Voltage = Adc_Vout_Get,
	.P_Get_Out_Current = Adc_Iout_Get,
};

static bool Port1_Power_Adjust_Callback(uint16_t* vol, uint16_t* port1Curr, uint16_t* port2Curr);

void Busi_Port_Detection_Init(void)
{
	Device_Register_Power_Adjust_Hook(Port1_Power_Adjust_Callback);
}

void Busi_Port_Detection(void)
{
	static uint8_t port_insert_count=0,port_pullout_count=0;	// 端口连续插入次数
	static uint32_t port_scan_t=0,port_get_info_t=0;
	static uint32_t printf_t=0;
	uint16_t tmpData;
	uint8_t ram_data=0;

	if(My_GetSystemTimePass(port_scan_t) >= PORT_SCAN_RATE)
	{
		port_scan_t = My_GetSystemTimeMark();
		if(Busi_Port_Detection_Func.P_SW_API_Func->SW_Get_Port_C_State(PORT_SUB, &ram_data))// 若 PORT_SUB 对应端口插入
		{
			port_pullout_count = 0;
			port_insert_count ++;
			if(port_insert_count >= PORT_INSETR_THRESHOLD)
			{
				port_insert_count = 0;
				Busi_Port_Detection_Data.Self_Reg_Data->Port_State = 1;	// 端口插入
				if(Busi_Port_Detection_Data.Previous_Port_State != Busi_Port_Detection_Data.Self_Reg_Data->Port_State)
				{
					Busi_Port_Detection_Data.Port_Updata = 1;
					Busi_Port_Detection_Data.Previous_Port_State = Busi_Port_Detection_Data.Self_Reg_Data->Port_State;
					MY_PRINTF("port insert\r\n");
				}
			}
		}
		else // 端口拔出
		{
			port_insert_count = 0;
			port_pullout_count++;
			if(port_pullout_count >= PORT_INSETR_THRESHOLD)
			{
				port_pullout_count = 0;
				Busi_Port_Detection_Data.Self_Reg_Data->Port_State = 0;	// 端口拔出
				if(Busi_Port_Detection_Data.Previous_Port_State != Busi_Port_Detection_Data.Self_Reg_Data->Port_State)
				{
					Busi_Port_Detection_Data.Port_Updata = 1;
					Busi_Port_Detection_Data.Previous_Port_State = Busi_Port_Detection_Data.Self_Reg_Data->Port_State;
					MY_PRINTF("port pullout\r\n");
				}
				
			}
		}
	}
	if(My_GetSystemTimePass(port_get_info_t) >= PORT_INFO_GATHER_RATE)
	{
		port_get_info_t = My_GetSystemTimeMark();
		if(Busi_Port_Detection_Data.Self_Reg_Data->Port_State)// 若端口插入
		{
			Busi_Port_Detection_Func.P_SW_API_Func->SW_Get_Port_Protocol(PORT_SUB, &Busi_Port_Detection_Data.Self_Reg_Data->Port_Protocol);
			Busi_Port_Detection_Data.Self_Reg_Data->Port_Real_Voltage = Busi_Port_Detection_Func.P_Get_Out_Voltage();
			Busi_Port_Detection_Data.Self_Reg_Data->Port_Real_Current = Busi_Port_Detection_Func.P_Get_Out_Current(PORT_SUB-1);
			Busi_Port_Detection_Data.Self_Reg_Data->Port_Real_Power = Busi_Port_Detection_Data.Self_Reg_Data->Port_Real_Voltage * \
																Busi_Port_Detection_Data.Self_Reg_Data->Port_Real_Current / 1000;	//单位：mV
			// Busi_Port_Detection_Func.P_SW_API_Func->SW_Get_NTC_Res(PORT_SUB, NTC_ADC_CHANNEL, &Busi_Port_Detection_Data.Self_Reg_Data->Port_NTC_Res);
			tmpData = Adc_Get_Channel_Data(NTC_ADC_CHANNEL);								//user get channel ADC data unit:mV
			Busi_Port_Detection_Data.Self_Reg_Data->Port_NTC_Res = tmpData / *(Hardware_Driver_Data.NTC_Data.NTC_Pullup_Current);	// 单位：K
			// MY_PRINTF("port_protocol = %d\r\n", Busi_Port_Detection_Data.Self_Reg_Data->Port_Protocol);
			// MY_PRINTF("port_voltage = %d\r\n", Busi_Port_Detection_Data.Self_Reg_Data->Port_Real_Voltage);
			// MY_PRINTF("port_current = %d\r\n", Busi_Port_Detection_Data.Self_Reg_Data->Port_Real_Current);
			// MY_PRINTF("port_power = %d\r\n", Busi_Port_Detection_Data.Self_Reg_Data->Port_Real_Power);
			// MY_PRINTF("NTC_Res = %d\r\n",Busi_Port_Detection_Data.Self_Reg_Data->Port_NTC_Res);
		}
	}
	if(My_GetSystemTimePass(printf_t) > 1000)
	{
		printf_t = My_GetSystemTimeMark();
		uint8_t num=0;
		for(num = 0; num < sizeof(I2C_SW3566_Reg_Data_t); num++)
		{
			MY_PRINTF("Reg[0][%d] = %#x\r\n", num, Busi_Port_Detection_Data.Self_Reg_Data->Memory_Block[num]);
		}
	}

}

static bool Port1_Power_Adjust_Callback(uint16_t* vol, uint16_t* port1Curr, uint16_t* port2Curr)
{
	if(Busi_Port_Detection_Func.P_SW_API_Func->SW_Get_Port_Protocol(PORT_SUB, &I2C_SW3566_Reg_Data->Port_Protocol) != 0) // 若端口存在协议输出
	{
		Busi_Port_Detection_Data.Self_Reg_Data->Port_Protocol_Current = *port1Curr * 25;
		Busi_Port_Detection_Data.Self_Reg_Data->Port_Protocol_Voltage = *vol * 10;
	}
	return false;
}


/**
 * @brief 主机获取当前插入端口号的输出协议数据
 * 仅适用于所有芯片自身映射的外部端口为1的情况,且主机端口号为1
 */
static void Get_Port_Out_Protocol_Func (void)
{
		Busi_Port_Detection_Data.Self_Reg_Data->Port_Protocol_Voltage = Device_Get_Requesting_Voltage();
		Busi_Port_Detection_Data.Self_Reg_Data->Port_Protocol = Device_Get_Protocol(1);
}

