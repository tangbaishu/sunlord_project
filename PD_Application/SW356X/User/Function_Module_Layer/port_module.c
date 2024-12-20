#include "port_module.h"
#include "api.h"
#include "zr_adc.h"
#include "zr_btm.h"
#include "zr_nvic.h"
#include "stdio.h"

#define LOG		"port_module: "

Port_Module_Data_t	Port_Module_Data;

#if ((CONFIG_PORT_MODE == SET_SINGLE_C_MODE) || (CONFIG_PORT_MODE == SET_DUO_CC_MODE))
	static void Typec_Attach_Hook(u8 portIndex);
	static void Typec_Detach_Hook(u8 portIndex);
#else
	// static void Noload_Trigger_Callback(bool* isDisableNoLoad);
    // static bool Current_Limitation_Callback(uint16_t* port1Current, uint16_t* port2Current);
#endif

#if CONFIG_PORT_MODE == SET_DUO_CA_MODE || CONFIG_PORT_MODE == SET_DUO_AA_MODE

	static void A_C_Noload_Trigger_Callback(bool* isDisableNoLoad);

	#if CONFIG_PORT_MODE == SET_DUO_CA_MODE
	static void Port_A_NoLoad_X_Second_Customize_Callback(uint8_t* xSecond);
	#endif
	static void A_NoLoad_Customize_Callback(bool* isPort1Offline, bool* isPort2Offline);

	static void Start_Noload_Timer(void);
	static void Stop_Noload_Timer(void);
	static uint32_t gNoLoadCount = 0;			// 周期扫描，连续检测到空载次数
	static bool Disable_NoLoad_Checking = 0;	// 禁用自动空载检测
	static bool gNoLoadChecking = false;		// 检测到空载
	static bool gNoLoadSpecHandle = false;		// 空载规格手柄？
	static bool gSimPort2Offline = false;		// 端口2离线

	// typedef enum 
	// {
	// 	NOLOAD_STEP_UNKNOWN,
	// 	NOLOAD_STEP_A_A,
	// 	NOLOAD_STEP_A_X,
	// 	NOLOAD_STEP_A_X_PLUG_OFF,
	// } noload_step_e;
	// static noload_step_e gNoLoadStep = NOLOAD_STEP_UNKNOWN;
	// static bool gNoSleep = false;
#endif

void Port_Scan_Func_Init(void)
{
	#if ((CONFIG_PORT_MODE == SET_SINGLE_C_MODE) || (CONFIG_PORT_MODE == SET_DUO_CC_MODE))
		Typec_Init_Hook_Func(Typec_Attach_Hook, Typec_Detach_Hook);
	#else
		Register_Noload_Trigger_Callback(A_C_Noload_Trigger_Callback);				// 仅适用于 A+A / A+C 模式

		#if (CONFIG_PORT_MODE == SET_DUO_CA_MODE)
			Register_Noload_X_Second_Config_Callback(Port_A_NoLoad_X_Second_Customize_Callback);	// 注册x次二次配置回调，自定义x次二次配置，然后进行端口A无负载检查
		#endif

		Register_Noload_State_Customize_Callback(A_NoLoad_Customize_Callback);	// 注册未加载状态自定义回调
        // Register_Current_Limitation_Callback(Current_Limitation_Callback);
	#endif

	// Register_Noload_Trigger_Callback(A_C_Noload_Trigger_Callback);
	
	// debug only
	// Rcc_Module_Clk_Enable(GPIO_ID);
	// Gpio_Function_Select(GPIO2, FUNCTION_SEL0); 
	// Gpio_Direction_Cfg(GPIO2, GPIO_DIRECTION_OUTPUT);
	// Gpio_Set_Output_Value(GPIO2, false);

	#if (CONFIG_PORT_MODE == SET_DUO_AA_MODE)
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
	printf("port state = %d\n", Device_Get_Port_State());
}

void Port_Scan_Func_Logic(void)
{
	if((Device_Get_Port_State() & 0x01) == 0x01) // PORT_ATTACH
	{
		if(Port_Module_Data.Port1_Insert_State.Current_State != PORT_ATTACH)
		{
			Port_Module_Data.Port1_Insert_State.Current_State = PORT_ATTACH;
			Port_Module_Data.Port1_Insert_State.State_Change = TRUE;
			printf("port1 attach\r\n");
		}	
	}
	else	// PORT_DETACH
	{
		if(Port_Module_Data.Port1_Insert_State.Current_State != PORT_DETACH)
		{
			Port_Module_Data.Port1_Insert_State.Current_State = PORT_DETACH;
			Port_Module_Data.Port1_Insert_State.State_Change = TRUE;
			printf("port1 detach\r\n");
		}	
	}

	if((Device_Get_Port_State() & 0x02) == 0x02) // PORT_ATTACH
	{
		if(Port_Module_Data.Port2_Insert_State.Current_State != PORT_ATTACH)
		{
			Port_Module_Data.Port2_Insert_State.Current_State = PORT_ATTACH;
			Port_Module_Data.Port2_Insert_State.State_Change = TRUE;
			printf("port2 attach\r\n");
		}
	}
	else	// PORT_DETACH
	{
		if(Port_Module_Data.Port2_Insert_State.Current_State != PORT_DETACH)
		{
			Port_Module_Data.Port2_Insert_State.Current_State = PORT_DETACH;
			Port_Module_Data.Port2_Insert_State.State_Change = TRUE;
			printf("port2 detach\r\n");
		}
	}
	if((Port_Module_Data.Port1_Insert_State.Current_State == PORT_DETACH) && (Port_Module_Data.Port2_Insert_State.Current_State == PORT_DETACH))
	{
		printf("Low_Power_Flash_Ctrl\r\n");
		Low_Power_Flash_Ctrl(true);
		printf("return deep_sleep() = %d\r\n", (uint8_t)Low_Power_Is_Deep_Sleep());
	}
	#if (CONFIG_PORT_MODE == SET_DUO_CA_MODE)
		// if (tempPortState != 3 && gNoLoadChecking)
		// {
		// 	gNoLoadChecking = false;
		// 	gNoLoadCount = 0;
		// 	Stop_Noload_Timer();
		// 	printf("stop no load checking\n");
		// }
		// if(Port_Module_Data.Port2_Insert_State == PORT_DETACH)
		// {
			
		// }
		// else if(Disable_NoLoad_Checking)
		// {
		// 	Start_Noload_Timer();
		// }
	#endif
	#if (CONFIG_PORT_MODE == SET_DUO_AA_MODE)
		if (tempPortState == 0 && gNoLoadSpecHandle)
		{
			gNoLoadChecking = false;
			gNoLoadCount = 0;
			gNoLoadSpecHandle = false;
			Stop_Noload_Timer();
			printf("stop no load checking\n");
		}
	#endif
}

#if ((CONFIG_PORT_MODE == SET_SINGLE_C_MODE) || (CONFIG_PORT_MODE == SET_DUO_CC_MODE))
	static void Typec_Attach_Hook(u8 portIndex)
	{
		// printf(LOG"Typec_Attach_Hook() portIndex = %#x\r\n",portIndex);
		if(portIndex == 0)
		{
			if(Port_Module_Data.Port1_Insert_State.Current_State != PORT_ATTACH)
			{
				Port_Module_Data.Port1_Insert_State.Current_State = PORT_ATTACH;
				Port_Module_Data.Port1_Insert_State.State_Change = TRUE;
				printf(LOG"port1 attach\r\n");
			}
		}
		else if(portIndex == 1)
		{
			if(Port_Module_Data.Port2_Insert_State.Current_State != PORT_ATTACH)
			{
				Port_Module_Data.Port2_Insert_State.Current_State = PORT_ATTACH;
				Port_Module_Data.Port2_Insert_State.State_Change = TRUE;
				printf(LOG"port2 attach\r\n");
			}
		}
		// if((Port_Module_Data.Port1_Insert_State.Current_State == PORT_ATTACH) && (Port_Module_Data.Port2_Insert_State.Current_State == PORT_ATTACH))
		// {
		// 	Port_Module_Data.Power_Supply_Select(POWER_SUPPLY_MODE_B, TRUE);
		// }
	}

	static void Typec_Detach_Hook(u8 portIndex)
	{
		// printf(LOG"Typec_Detach_Hook() portIndex = %#x\r\n",portIndex);
		if(portIndex == 0)
		{
			if(Port_Module_Data.Port1_Insert_State.Current_State != PORT_DETACH)
			{
				Port_Module_Data.Port1_Insert_State.Current_State = PORT_DETACH;
				Port_Module_Data.Port1_Insert_State.State_Change = TRUE;
				printf(LOG"Port1 Detach\r\n");
			}
		}
		else if(portIndex == 1)
		{
			if(Port_Module_Data.Port2_Insert_State.Current_State != PORT_DETACH)
			{
				Port_Module_Data.Port2_Insert_State.Current_State = PORT_DETACH;
				Port_Module_Data.Port2_Insert_State.State_Change = TRUE;
				printf(LOG"Port2 Detach\r\n");
			}
		}
		// Port_Module_Data.Power_Supply_Select(POWER_SUPPLY_MODE_A, TRUE);
	}
#elif (CONFIG_PORT_MODE == SET_DUO_CA_MODE) || (CONFIG_PORT_MODE == SET_DUO_AA_MODE)
static void A_C_Noload_Trigger_Callback(bool* isDisableNoLoad)
{
	#if (CONFIG_PORT_MODE == SET_DUO_CA_MODE)
	*isDisableNoLoad = true;
	printf(LOG"no load: disable auto checking, port state = %d\n", Device_Get_Port_State());
	gNoLoadChecking = true;
	Disable_NoLoad_Checking = true;
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

#if (CONFIG_PORT_MODE == SET_DUO_CA_MODE)
	static void Port_A_NoLoad_X_Second_Customize_Callback(uint8_t* xSecond)
	{
		uint8_t temp = *xSecond;
		*xSecond = true;
		printf("x Second old : %d set to 0, port state = %d\n", temp, Device_Get_Port_State());
	}
#endif


	#define NOLOAD_CHECKING_INTERVAL (2000)
	#define NOLOAD_MAX_COUNT (3*1000/NOLOAD_CHECKING_INTERVAL)
	#define IOUT_NOLOAD_TH (100)


	static void A_NoLoad_Customize_Callback(bool* isPort1Offline, bool* isPort2Offline)
	{
		#if (CONFIG_PORT_MODE == SET_DUO_CA_MODE)
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
					if(Port_Module_Data.Port2_Insert_State.Current_State != PORT_DETACH)
					{
						Port_Module_Data.Port2_Insert_State.Current_State = PORT_DETACH; 
						Port_Module_Data.Port2_Insert_State.State_Change = TRUE;
					}
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
				// printf("iout = %d, count:%d\n", temp, gNoLoadCount);
			}
			else
			{
				gNoLoadCount = 0;
				// printf("iout = %d", temp);
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

	// void Port_Scan(void)
	// {
	// 	uint8_t port_state=0;
	// 	port_state = Device_Get_Port_State();
	// 	printf("port_state = %#x\r\n", port_state);
	// 	if((port_state & 0x01) == 0x01) // port 1
	// 	{
	// 		printf("port1 attached\r\n\r\n");
	// 	}
	// 	else
	// 	{
	// 		printf("port1 no attached\r\n");
	// 	}
	// 	if((port_state & 0x02) == 0x02) // port 2
	// 	{
	// 		printf("port2 attached\r\n");
	// 	}
	// 	else
	// 	{
	// 		printf("port2 no attached\r\n");
	// 	}
	// }

	// static void Noload_Trigger_Callback(bool* isDisableNoLoad)
	// {
	// 	printf(LOG"noload_trigger_callback() param %d\r\n", (uint8_t)*isDisableNoLoad);
	// 	Port_Scan();
	// }

    // static bool Current_Limitation_Callback(uint16_t* port1Current, uint16_t* port2Current)
    // {
    //     printf(LOG"Current_Limitation_Callback() param port1Current=%d port2Current=%d\r\n", *port1Current, *port2Current);
    //     *port1Current = 25;
    //     *port2Current = 25;
    //     return true;   // true 立即应用参数 false 暂不应用参数
    // }