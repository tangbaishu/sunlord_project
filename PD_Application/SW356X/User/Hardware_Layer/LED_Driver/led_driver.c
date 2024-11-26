#include "LED_Driver/led_driver.h"
#include "zr_systick.h"

#define LED0_OUTPUT_H()		Gpio_Set_Output_Value(LED0_GPIO_PIN, TRUE)
#define LED1_OUTPUT_H()		Gpio_Set_Output_Value(LED1_GPIO_PIN, TRUE)
#define LED2_OUTPUT_H()		Gpio_Set_Output_Value(LED2_GPIO_PIN, TRUE)

#define LED0_OUTPUT_L()		Gpio_Set_Output_Value(LED0_GPIO_PIN, FALSE)
#define LED1_OUTPUT_L()		Gpio_Set_Output_Value(LED1_GPIO_PIN, FALSE)
#define LED2_OUTPUT_L()		Gpio_Set_Output_Value(LED2_GPIO_PIN, FALSE)

#define LED0_ON_LEVEL()		LED0_OUTPUT_H()
#define LED1_ON_LEVEL()		LED1_OUTPUT_H()
#define LED2_ON_LEVEL()		LED2_OUTPUT_H()

#define LED0_OFF_LEVEL()	LED0_OUTPUT_L()
#define LED1_OFF_LEVEL()	LED1_OUTPUT_L()
#define LED2_OFF_LEVEL()	LED2_OUTPUT_L()

#define LED0_TURN()			Gpio_Toggle_Output(LED0_GPIO_PIN)
#define LED1_TURN()			Gpio_Toggle_Output(LED1_GPIO_PIN)
#define LED2_TURN()			Gpio_Toggle_Output(LED2_GPIO_PIN)

void LED_Driver_Check(void)
{
	LED_Driver_Init();
	LED_Driver_ON(0);LED_Driver_ON(1);LED_Driver_ON(2);
	Systick_Delay_Ms(100);
	LED_Driver_OFF(0);LED_Driver_OFF(1);LED_Driver_OFF(2);
	Systick_Delay_Ms(100);
	LED_Driver_Turn(0);LED_Driver_Turn(1);LED_Driver_Turn(2);
}

void LED_Driver_Init(void)
{
	gpio_config_t led_gpio_config;

	Rcc_Module_Clk_Enable(LED0_GPIO_CLOCK);
	Rcc_Module_Clk_Enable(LED1_GPIO_CLOCK);
	Rcc_Module_Clk_Enable(LED2_GPIO_CLOCK);
	Gpio_Deinit(LED0_GPIO_PIN);
	Gpio_Deinit(LED1_GPIO_PIN);
	Gpio_Deinit(LED2_GPIO_PIN);
	
	led_gpio_config.gpioFunc = FUNCTION_SEL0;
	led_gpio_config.direction = GPIO_DIRECTION_OUTPUT;
	led_gpio_config.pulldown = PULLDOWN_DIS;
	led_gpio_config.pullup = PULLUP_DIS;
	led_gpio_config.pmosOd = PMOS_OPDRAIN_DIS;
	led_gpio_config.nmosOd = NMOS_OPDRAIN_DIS;
	Gpio_Init(LED0_GPIO_PIN, &led_gpio_config);
	Gpio_Init(LED1_GPIO_PIN, &led_gpio_config);
	Gpio_Init(LED2_GPIO_PIN, &led_gpio_config);
	LED_Driver_OFF(0);LED_Driver_OFF(1);LED_Driver_OFF(2);
}

/**
 * @brief LED_Driver_ON
 * 
 * @param led_num:	LED_BSP_Num_e
 */
void LED_Driver_ON(uint8 led_num)
{
	if(LED0 == led_num)
	{
		LED0_ON_LEVEL();
	}
	else if (LED1 == led_num)
	{
		LED1_ON_LEVEL();
	}
	else if (LED2 == led_num)
	{
		LED2_ON_LEVEL();
	}
}

/**
 * @brief LED_Driver_OFF
 * 
 * @param led_num:	LED_BSP_Num_e
 */
void LED_Driver_OFF(uint8 led_num)
{
	if(LED0 == led_num)
	{
		LED0_OFF_LEVEL();
	}
	else if (LED1 == led_num)
	{
		LED1_OFF_LEVEL();
	}
	else if (LED2 == led_num)
	{
		LED2_OFF_LEVEL();
	}
}

/**
 * @brief LED_Driver_Turn
 * 
 * @param led_num:	LED_BSP_Num_e
 */
void LED_Driver_Turn(uint8 led_num)
{
	if(LED0 == led_num)
	{
		LED0_TURN();
	}
	else if (LED1 == led_num)
	{
		LED1_TURN();
	}
	else if (LED2 == led_num)
	{
		LED2_TURN();
	}
}
