#include "port_scan_function.h"
#include "api.h"
#include "zr_adc.h"
#include "zr_btm.h"
#include "zr_nvic.h"
#include "stdio.h"

#define LOG		"Port_Scan_Function:"

Port_Scan_Func_Data_t	Port_Scan_Func_Data;

// #if CONFIG_PORT_MODE == SET_DUO_CC_MODE
// 	static void Typec_Attach_Hook(u8 portIndex);
// 	static void Typec_Detach_Hook(u8 portIndex);
// #else
// 	static void Noload_Trigger_Callback(bool* isDisableNoLoad);
//     static bool Current_Limitation_Callback(uint16_t* port1Current, uint16_t* port2Current);
// #endif

#if CONFIG_PORT_MODE == SET_DUO_CA_MODE || CONFIG_PORT_MODE == SET_DUO_AA_MODE

	static void A_C_Noload_Trigger_Callback(bool* isDisableNoLoad);

	#if CONFIG_PORT_MODE == SET_DUO_CA_MODE
	static void Port_A_NoLoad_X_Second_Customize_Callback(uint8_t* xSecond);
	#endif
	static void A_NoLoad_Customize_Callback(bool* isPort1Offline, bool* isPort2Offline);

	static void Start_Noload_Timer(void);
	static void Stop_Noload_Timer(void);
	static uint32_t gNoLoadCount = 0;
	static bool gNoLoadChecking = false;
	static bool gNoLoadSpecHandle = false;
	static bool gSimPort2Offline = false;

	typedef enum 
	{
		NOLOAD_STEP_UNKNOWN,
		NOLOAD_STEP_A_A,
		NOLOAD_STEP_A_X,
		NOLOAD_STEP_A_X_PLUG_OFF,
	} noload_step_e;
	static noload_step_e gNoLoadStep = NOLOAD_STEP_UNKNOWN;
	static bool gNoSleep = false;
#endif

void Port_Scan_Func_Init(void)
{
	// #if CONFIG_PORT_MODE == SET_DUO_CC_MODE
	// 	Typec_Init_Hook_Func(Typec_Attach_Hook, Typec_Detach_Hook);
	// #else
	// 	Register_Noload_Trigger_Callback(Noload_Trigger_Callback);		// 仅适用于 A+A / A+C 模式
    //     Register_Current_Limitation_Callback(Current_Limitation_Callback);
	// #endif

	Register_Noload_Trigger_Callback(A_C_Noload_Trigger_Callback);
	
	#if (CONFIG_PORT_MODE == SET_DUO_CA_MODE)
	Register_Noload_X_Second_Config_Callback(Port_A_NoLoad_X_Second_Customize_Callback);
	#endif

	Register_Noload_State_Customize_Callback(A_NoLoad_Customize_Callback);
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
	uint8_t tempPortState = Device_Get_Port_State();
		// #if defined(TRACE_PORT_STATE)
		// if (tempPortState != portState)
		// {
		// 	portState = tempPortState;
		// 	printf("port state = %d\n", portState);
		// }
	

	#if (CONFIG_PORT_MODE == SET_DUO_CA_MODE)
		if (tempPortState != 3 && gNoLoadChecking)
		{
			gNoLoadChecking = false;
			gNoLoadCount = 0;
			Stop_Noload_Timer();
			printf("stop no load checking\n");
		}
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

void Port_Scan_Func_Logic(void)
{

}


#if CONFIG_PORT_MODE == SET_DUO_CC_MODE
	static void Typec_Attach_Hook(u8 portIndex)
	{
		printf(LOG"portIndex = %#x\r\n",portIndex);
		if(portIndex == 0)
		{
			Port_Scan_Func_Data.Port1_Insert_State = PORT_ATTACH;
			printf(LOG"Port1 Attach\r\n");
		}
		else if(portIndex == 1)
		{
			Port2_Insert_State = PORT_ATTACH;
			printf(LOG"Port2 Attach\r\n");
		}
		if((Port_Scan_Func_Data.Port1_Insert_State == PORT_ATTACH) && (Port2_Insert_State == PORT_ATTACH))
		{
			Power_Supply_Select(POWER_SUPPLY_MODE_B);
			Config_Apply();
		}
	}

	static void Typec_Detach_Hook(u8 portIndex)
	{
		printf(LOG"portIndex = %#x\r\n",portIndex);
		if(portIndex == 0)
		{
			Port_Scan_Func_Data.Port1_Insert_State = PORT_DETACH;
			printf(LOG"Port1 Detach\r\n");
		}
		else if(portIndex == 1)
		{
			Port2_Insert_State = PORT_DETACH;
			printf(LOG"Port2 Detach\r\n");
		}
		Power_Supply_Select(POWER_SUPPLY_MODE_A);
		Config_Apply();
	}
#elif (CONFIG_PORT_MODE == SET_DUO_CA_MODE) || (CONFIG_PORT_MODE == SET_DUO_AA_MODE)
static void A_C_Noload_Trigger_Callback(bool* isDisableNoLoad)
{
	#if (CONFIG_PORT_MODE == SET_DUO_CA_MODE)
	*isDisableNoLoad = false;
	printf("no load: disable auto checking, manual checking start, port state = %d\n", Device_Get_Port_State());
	gNoLoadChecking = true;
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
		*xSecond = 0;
		printf("x Second old : %d set to 0, port state = %d\n", temp, Device_Get_Port_State());
	}
#endif


	#define NOLOAD_CHECKING_INTERVAL (200)
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
				printf("iout = %d, count:%d\n", temp, gNoLoadCount);
			}
			else
			{
				gNoLoadCount = 0;
				printf("iout = %d", temp);
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