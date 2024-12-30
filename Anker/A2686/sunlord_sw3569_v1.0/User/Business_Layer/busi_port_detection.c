#include "busi_port_detection.h"
#include "busi_i2c_bus.h"
#include "zr_adc.h"
#include "api.h"

#if defined(DEBUG_PRINTF_OPEN)
    #include "stdio.h"
    #define LOG "busi_p_d: "
#endif
static void Port_Attach_Hook(u8 portIndex);
static void Port_Dttach_Hook(u8 portIndex);
static bool Port1_Power_Adjust_Callback(uint16_t* vol, uint16_t* port1Curr, uint16_t* port2Curr);

static Port_Link_State_e Get_Port1_Link_State(void);
static Port_Link_State_e Get_Port2_Link_State(void);
static Port_Out_Protocol_Data_t Get_Port1_Out_Protocol_Func (void);
static Port_Out_Protocol_Data_t Get_Port2_Out_Protocol_Func (void);
static Port_RealTime_Out_Power_t Get_Port1_RealTime_Out_Power_Func (void);
static Port_RealTime_Out_Power_t Get_Port2_RealTime_Out_Power_Func (void);

Port_Detection_Data_t	Busi_Port_Detection_Data={
	.P_Port_BSP_Data		 	= &Port_BSP_Data,
	.Port2_Out_Protocol_Data 	= &Busi_I2C_Bus_Data.Port2_Out_Protocol_Data,
	.Port2_RealTime_Out_Power 	= &Busi_I2C_Bus_Data.Port2_RealTime_Out_Power,
};


Busi_Port_Detection_Func_t Busi_Port_Detection_Func = {
	.Port1_Link_Detection_Func = Get_Port1_Link_State,
	.Port1_Out_Protocol_Func = Get_Port1_Out_Protocol_Func,
	.Port1_RealTime_Out_Power_Func = Get_Port1_RealTime_Out_Power_Func,

	.Port2_Link_Detection_Func = Get_Port2_Link_State,
	.Port2_Out_Protocol_Func = Get_Port2_Out_Protocol_Func,
	.Port2_RealTime_Out_Power_Func = Get_Port2_RealTime_Out_Power_Func
};

void Busi_Port_Detection_Init(void)
{
	Typec_Init_Hook_Func(Port_Attach_Hook, Port_Dttach_Hook);
	Device_Register_Power_Adjust_Hook(Port1_Power_Adjust_Callback);
	Get_All_Port_Link_State();
}

void Get_All_Port_Link_State(void)
{
	Busi_Port_Detection_Func.Port1_Link_Detection_Func();
	Busi_Port_Detection_Func.Port2_Link_Detection_Func();
}

static void Port_Attach_Hook(u8 portIndex)
{
	if(portIndex == 0)
	{
		if(Busi_Port_Detection_Data.Port1_Out_Protocol_Data.Port_Link_State != Port_1_Insert)
		{
			Busi_Port_Detection_Data.Port_State_Updata = true;
			Busi_Port_Detection_Data.Port1_Out_Protocol_Data.Port_Link_State = Port_1_Insert;
			#if defined(DEBUG_PRINTF_OPEN)
				printf(LOG"Port1 Attach\r\n");
			#endif
			
		}
	}
	else if(portIndex == 1)
	{
		if(Busi_Port_Detection_Data.Port2_Out_Protocol_Data->Port_Link_State != Port_2_Insert)
		{
			Busi_Port_Detection_Data.Port_State_Updata = true;
			Busi_Port_Detection_Data.Port2_Out_Protocol_Data->Port_Link_State = Port_2_Insert;
			#if defined(DEBUG_PRINTF_OPEN)
				printf(LOG"Port2 Attach\r\n");
			#endif
		}
	}
}

static void Port_Dttach_Hook(u8 portIndex)
{
	if(portIndex == 0)
	{
		if(Busi_Port_Detection_Data.Port1_Out_Protocol_Data.Port_Link_State != Port_1_Pull_Out)
		{
			Busi_Port_Detection_Data.Port_State_Updata = true;
			Busi_Port_Detection_Data.Port1_Out_Protocol_Data.Port_Link_State = Port_1_Pull_Out;
			#if defined(DEBUG_PRINTF_OPEN)
				printf(LOG"Prt1 Dttach\r\n");
			#endif
		}
	}
	else if(portIndex == 1)
	{
		if(Busi_Port_Detection_Data.Port2_Out_Protocol_Data->Port_Link_State != Port_2_Pull_Out)
		{
			Busi_Port_Detection_Data.Port_State_Updata = true;
			Busi_Port_Detection_Data.Port2_Out_Protocol_Data->Port_Link_State = Port_2_Pull_Out;
			#if defined(DEBUG_PRINTF_OPEN)
				printf(LOG"Port2 Dttach\r\n");
			#endif
		}
	}
}

static bool Port1_Power_Adjust_Callback(uint16_t* vol, uint16_t* port1Curr, uint16_t* port2Curr)
{
	Busi_Port_Detection_Func.Port1_Link_Detection_Func();
	#if defined(DEBUG_PRINTF_OPEN)
		printf(LOG"Port_Power_Adjust_Callback V=%d, I1=%d, I2=%d\r\n", *vol, *port1Curr, *port2Curr);
	#endif
	if(Busi_Port_Detection_Data.Port1_Out_Protocol_Data.Port_Link_State == Port_1_Insert)
	{
		Busi_Port_Detection_Data.Port1_Out_Protocol_Data.Protocol_Type = Device_Get_Protocol(1);
		if(0 == Busi_Port_Detection_Data.Port1_Out_Protocol_Data.Protocol_Type)
		{
			Busi_Port_Detection_Data.Port1_Out_Protocol_Data.Protocol_Voltage = 0;
		}
		else
		{
			Busi_Port_Detection_Data.Port1_Out_Protocol_Data.Protocol_Current = *port1Curr * 25;
		}
		Busi_Port_Detection_Data.Port1_Out_Protocol_Data.Protocol_Voltage = *vol * 10;
		#if defined(DEBUG_PRINTF_OPEN)
		printf(LOG"Port1_Power_Adjust_Callback V=%d, I=%d, Protocol=%d\r\n",\
						Busi_Port_Detection_Data.Port1_Out_Protocol_Data.Protocol_Voltage,\
						Busi_Port_Detection_Data.Port1_Out_Protocol_Data.Protocol_Current,\
						(uint8_t)Busi_Port_Detection_Data.Port1_Out_Protocol_Data.Protocol_Type);
		#endif
	}

	if(Busi_Port_Detection_Data.Port2_Out_Protocol_Data->Port_Link_State == Port_2_Insert)
	{
		Busi_Port_Detection_Data.Port2_Out_Protocol_Data->Protocol_Type = Device_Get_Protocol(2);
		if(0 == Busi_Port_Detection_Data.Port2_Out_Protocol_Data->Protocol_Type)
		{
			Busi_Port_Detection_Data.Port2_Out_Protocol_Data->Protocol_Current = 0;
		}
		else
		{
			Busi_Port_Detection_Data.Port2_Out_Protocol_Data->Protocol_Current = *port2Curr *25;
		}
		Busi_Port_Detection_Data.Port2_Out_Protocol_Data->Protocol_Voltage = *vol * 10;
		#if defined(DEBUG_PRINTF_OPEN)
		printf(LOG"Port2_Power_Adjust_Callback V=%d, I=%d, Protocol=%d\r\n",\
						Busi_Port_Detection_Data.Port2_Out_Protocol_Data->Protocol_Voltage,\
						Busi_Port_Detection_Data.Port2_Out_Protocol_Data->Protocol_Current,\
						(uint8_t)Busi_Port_Detection_Data.Port2_Out_Protocol_Data->Protocol_Type);
		#endif
	}
	return false;
}

static Port_Link_State_e Get_Port1_Link_State(void)
{
	if((Device_Get_Port_State() & 0x01) == 0x01) // PORT1_ATTACH
	{
		if(Busi_Port_Detection_Data.Port1_Out_Protocol_Data.Port_Link_State != Port_1_Insert)
		{
			Busi_Port_Detection_Data.Port_State_Updata = true;
			Busi_Port_Detection_Data.Port1_Out_Protocol_Data.Port_Link_State = Port_1_Insert;
			#if defined(DEBUG_PRINTF_OPEN)
			printf(LOG"Get_Port1_Link_State() Port1 Attach\r\n");
			#endif
		}	
		return Port_1_Insert;
	}
	else	// PORT1_DETACH
	{
		if(Busi_Port_Detection_Data.Port1_Out_Protocol_Data.Port_Link_State != Port_1_Pull_Out)
		{
			Busi_Port_Detection_Data.Port_State_Updata = true;
			Busi_Port_Detection_Data.Port1_Out_Protocol_Data.Port_Link_State = Port_1_Pull_Out;
			#if defined(DEBUG_PRINTF_OPEN)
			printf(LOG"Get_Port1_Link_State() Port1 Dttach\r\n");
			#endif
		}
		return Port_1_Pull_Out;
	}
}

static Port_Link_State_e Get_Port2_Link_State(void)
{
	if((Device_Get_Port_State() & 0x02) == 0x02) // PORT2_ATTACH
	{
		if(Busi_Port_Detection_Data.Port2_Out_Protocol_Data->Port_Link_State != Port_2_Insert)
		{
			Busi_Port_Detection_Data.Port_State_Updata = true;
			Busi_Port_Detection_Data.Port2_Out_Protocol_Data->Port_Link_State = Port_2_Insert;
			#if defined(DEBUG_PRINTF_OPEN)
			printf(LOG"Get_Port2_Link_State() Port2 Attach\r\n");
			#endif
		}	
		return Port_2_Insert;
	}
	else	// PORT2_DETACH
	{
		if(Busi_Port_Detection_Data.Port2_Out_Protocol_Data->Port_Link_State != Port_2_Pull_Out)
		{
			Busi_Port_Detection_Data.Port_State_Updata = true;
			Busi_Port_Detection_Data.Port2_Out_Protocol_Data->Port_Link_State = Port_2_Pull_Out;
			#if defined(DEBUG_PRINTF_OPEN)
			printf(LOG"Get_Port2_Link_State() Port2 Dttach\r\n");
			#endif
		}
		return Port_2_Pull_Out;
	}
}

static Port_Out_Protocol_Data_t Get_Port1_Out_Protocol_Func (void)
{
	Busi_Port_Detection_Data.Port1_Out_Protocol_Data.Protocol_Voltage = Device_Get_Requesting_Voltage();
	Busi_Port_Detection_Data.Port1_Out_Protocol_Data.Protocol_Type = Device_Get_Protocol(1);
	return Busi_Port_Detection_Data.Port1_Out_Protocol_Data;	// 注协议电流未更新
}

static Port_Out_Protocol_Data_t Get_Port2_Out_Protocol_Func (void)
{
	Busi_Port_Detection_Data.Port2_Out_Protocol_Data->Protocol_Voltage = Device_Get_Requesting_Voltage();
	Busi_Port_Detection_Data.Port2_Out_Protocol_Data->Protocol_Type = Device_Get_Protocol(2);
	return *Busi_Port_Detection_Data.Port2_Out_Protocol_Data;	// 注协议电流未更新
}

static Port_RealTime_Out_Power_t Get_Port1_RealTime_Out_Power_Func (void)
{
	Busi_Port_Detection_Data.Port1_RealTime_Out_Power.Voltage = Adc_Vout_Get();		// unit:mV
	Busi_Port_Detection_Data.Port1_RealTime_Out_Power.Current = Adc_Iout_Get(0);	// unit:mA
	Busi_Port_Detection_Data.Port1_RealTime_Out_Power.Power = \
		(Busi_Port_Detection_Data.Port1_RealTime_Out_Power.Voltage * Busi_Port_Detection_Data.Port1_RealTime_Out_Power.Current) \
		/ 1000;	// 将其变成 mW 
	#if defined(DEBUG_PRINTF_OPEN)
	printf(LOG"Port1_Real_Power V=%d, I=%d, P=%d\r\n", Busi_Port_Detection_Data.Port1_RealTime_Out_Power.Voltage,\
							Busi_Port_Detection_Data.Port1_RealTime_Out_Power.Current, \
							Busi_Port_Detection_Data.Port1_RealTime_Out_Power.Power);
	#endif
	return Busi_Port_Detection_Data.Port1_RealTime_Out_Power;
}

static Port_RealTime_Out_Power_t Get_Port2_RealTime_Out_Power_Func (void)
{
	Busi_Port_Detection_Data.Port2_RealTime_Out_Power->Voltage = Adc_Vout_Get();		// unit:mV
	Busi_Port_Detection_Data.Port2_RealTime_Out_Power->Current = Adc_Iout_Get(1);	// unit:mA
	Busi_Port_Detection_Data.Port2_RealTime_Out_Power->Power = \
		(Busi_Port_Detection_Data.Port2_RealTime_Out_Power->Voltage * Busi_Port_Detection_Data.Port2_RealTime_Out_Power->Current) \
		/ 1000;	// 将其变成 mW 
	#if defined(DEBUG_PRINTF_OPEN)
	printf(LOG"Port2_Real_Power V=%d, I=%d, P=%d\r\n", Busi_Port_Detection_Data.Port2_RealTime_Out_Power->Voltage,\
							Busi_Port_Detection_Data.Port2_RealTime_Out_Power->Current, \
							Busi_Port_Detection_Data.Port2_RealTime_Out_Power->Power);
	#endif
	return *Busi_Port_Detection_Data.Port2_RealTime_Out_Power;
}

