#include "define_data.h"
#include "define_function.h"

void delay()//�����ʱ,Լ32ms
{
	int i;
	for(i=0;i<5000;i++)
	{
		asm("nop");
	}
}
void delay_10us()//��ʱ10us
{
	int j;
	for(j=0;j<1;j++)
	{
		asm("nop");
	}
}
