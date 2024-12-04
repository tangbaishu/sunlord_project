#include "LED_Function.h"
#include "LED_Driver.h"
#include "systick.h"

LED_Function_List_t LED_Function_List={LED_Driver_Init, LED_Driver_ON, LED_Driver_OFF, LED_Driver_Turn};

void LED_Check(void)
{
	LED_Function_List.LED_Function_Init();
	
	for(uint8 num=LED1;num < LED_NUMBER+LED1; num++)
	{
		LED_Function_List.LED_Function_ON(num);
	}
	delay_1ms(1000);

	for(uint8 num=LED1;num < LED_NUMBER+LED1; num++)
	{
		LED_Function_List.LED_Function_Turn(num);
	}
	delay_1ms(1000);
}