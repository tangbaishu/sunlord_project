//============================================
//                  声明
// 该程序仅供参考和用于芯片正常工作环境下对其
// 进行简单的功能测试。若程序中存在疏漏，芯海
// 科技有限公司对此不承担任何责任。
//   (C)Shenzhen Chipsea Technologies Co.,Ltd
//============================================
// filename: main.c
// chip    : CSU32P10
// author  :
// date    : 2017-06-13
// clk=4 Mhz
// v1.0

//IO口配置:
//pt1.4：LED0闪灯				pt5.0：pwm3互补pwm3L输出 or pwm2输出
//pt1.5：电压等级显示LED1		pt5.1：pwm3互补pwm3H输出                  
//pt3.1：分压电压检测口			pt3.2：电压等级显示LED2

//功能说明：
//1、定时器0：4ms的定时中断；
//2、pt1.4的LED0每秒钟闪烁一次;
//3、pt3.1的分压电压AD检测范围	2.2V(2/3VDD)~3.3V(VDD)，使用数据查表，比较表值和电压AD值的大小，
//	电压分为三级：	2.2V~2.5V LED1不亮,LED2不亮;
//					2.5V~2.9V LED1亮,LED2不亮;
//					2.9V~3.3V LED1不亮,LED2亮;
//4、pt5.0作pwm2蜂鸣器输出时，频率为4kHz,并且每秒钟响一次蜂鸣器，响30s后停止
//		 作互补pwm3L输出时，频率为200kHz，输出30s后停止输出pwm波形;                      
//============================================
#include "CSU32P10.h"
#include "define_data.h"
#include "define_function.h"

//============================================
// program start
//============================================
void main()
{
	asm("clrwdt");					//清看门狗
	delay();						//上电延时
	MCU_Initialization();			//芯片初始化	
	Bit_Initialization();			//全局变量初始化 
		
	while(1)
	{
		asm("clrwdt");				//清看门狗
		adc();						//调用采AD函数   
		open_pwm();					//调用开启pwm函数
		sleep();
	}	

}
//============================================
// interrupt function
//中断处理函数
//中断服务函数，不推荐在函数里面使用复杂是运算和太多的if嵌套语句
//不允许定义临时变量，调用的函数也不允许定义变量，可以使用全局变量
//============================================
void INT_FUNCTION(void) interrupt
{
	if(E0IF)						//判断是否为外部中断0
	{
		E0IF=0;		
	}
	
	else if(E1IF)					//判断是否为外部中断1
	{
		E1IF=0;
	}
	
	else if(TM0IF)					//判断是否为定时器0溢出中断
	{
		TM0IF=0;					//是则清定时器0中断标志
		t1s_count++;
		if(t1s_count>=125)			//计时500ms
		{
			t1s_count=0;
			led=!led;				//每500ms闪烁一次led灯			
			if(t10s_count>=60)		//计时30s	
			{
				TM3CON=0X00;		//30s后关闭互补pwm3
				TM3CON2=0X00;		
				B_Time500ms=0;		//30s后关闭蜂鸣器
				B_Sleep=1;			//30s后置休眠标志位
			}
			else
			{
				t10s_count++;
				B_Time500ms=!B_Time500ms;//每500ms翻转一次B_Time500ms标志位
			}
		}
	}
	
	else if(TM2IF)					//判断是否为定时器2溢出中断
	{
		TM2IF=0;			
	}
	
	else if(TM3IF)					//判断是否为定时器3溢出中断
	{
		TM3IF=0;			
	}

}

