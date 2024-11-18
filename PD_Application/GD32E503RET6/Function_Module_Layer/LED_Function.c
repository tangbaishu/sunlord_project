#include "LED_Function.h"
#include "LED_Driver.h"
#include "systick.h"

LED_Function_List_t LED_Function_List={LED_Driver_Init, LED_Driver_ON, LED_Driver_OFF, LED_Driver_Turn};

void LED_Check(void)
{
	LED_Driver_Init();
	LED_Driver_OFF(LED1);LED_Driver_OFF(LED2);LED_Driver_OFF(LED3);LED_Driver_OFF(LED4);
	delay_1ms(1000);
	LED_Driver_ON(LED1);
	delay_1ms(1000);
	LED_Driver_ON(LED2);
	delay_1ms(1000);
	LED_Driver_ON(LED3);
	delay_1ms(1000);
	LED_Driver_ON(LED4);

	delay_1ms(1000);
	LED_Driver_OFF(LED1);LED_Driver_OFF(LED2);LED_Driver_OFF(LED3);LED_Driver_OFF(LED4);
	delay_1ms(1000);
	LED_Driver_Turn(LED1);
	delay_1ms(1000);
	LED_Driver_Turn(LED2);
	delay_1ms(1000);
	LED_Driver_Turn(LED3);
	delay_1ms(1000);
	LED_Driver_Turn(LED4);
	delay_1ms(1000);
}