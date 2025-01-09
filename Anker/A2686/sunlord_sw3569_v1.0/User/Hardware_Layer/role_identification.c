#include "role_identification.h"
#include "zr_rcc.h"



#if IDENTIFY_WAY == LEVEL_IDENTIFY
#include "zr_gpio.h"
	static void Role_Identification_Init(void)
	{
		Rcc_Module_Clk_Enable(GPIO_ID);
		Gpio_Direction_Cfg(IDENTIFY_GPIO_PIN, GPIO_DIRECTION_INPUT);
		Gpio_Pulldown_Pullup_Cfg(IDENTIFY_GPIO_PIN, PULLDOWN_DIS, PULLUP_EN);	// 输入上拉
	}
	/**
	 * @brief 角色识别函数
	 * 
	 * @param device_type 1：主机、2：从机
	 */
	void Role_Identification(Device_Type_e *device_type)
	{
		static bool initialize=0;
		if(initialize == 0)
		{
			Role_Identification_Init();
			initialize = 1;
		}
		if(Gpio_Get_Input_Value(IDENTIFY_GPIO_PIN))
		{
			*device_type = SLAVE_ROLE_1;
		}
		else
		{
			*device_type = HOST_ROLE;
		}
	}
#else
#include "zr_adc.h"
	/**
	 * @brief 角色识别函数
	 * 
	 * @param device_type 1：主机、2：从机
	 */
	void Role_Identification(Device_Type_e *device_type)
	{

	}
#endif