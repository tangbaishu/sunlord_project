#include "LED_Function.h"
#include "LED_Driver.h"
#include "systick.h"

LED_Function_List_t LED_Function_List={LED_Driver_Init, LED_Driver_ON, LED_Driver_OFF, LED_Driver_Turn};

void LED_Check(void)
{
	LED_Function_List.LED_Function_Init();
	
	LED_Function_List.LED_Function_OFF(1);
	LED_Function_List.LED_Function_OFF(2);
	LED_Function_List.LED_Function_OFF(3);
	LED_Function_List.LED_Function_OFF(4);
	delay_1ms(1000);
	LED_Function_List.LED_Function_ON(1);
	delay_1ms(1000);
	LED_Function_List.LED_Function_ON(2);
	delay_1ms(1000);
	LED_Function_List.LED_Function_ON(3);
	delay_1ms(1000);
	LED_Function_List.LED_Function_ON(4);
	delay_1ms(1000);

	LED_Function_List.LED_Function_Turn(1);
	delay_1ms(1000);
	LED_Function_List.LED_Function_Turn(2);
	delay_1ms(1000);
	LED_Function_List.LED_Function_Turn(3);
	delay_1ms(1000);
	LED_Function_List.LED_Function_Turn(4);
	delay_1ms(1000);
}