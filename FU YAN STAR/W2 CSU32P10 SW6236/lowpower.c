#include "CSU32P10.h"
#include "define_data.h"
#include "define_function.h"


//sleep函数
void sleep()
{
	if (B_Sleep)					//判断Sleep标志                                             
	{                                                                                         
		INTE = 0x00;				//先关掉总中断使能                                                           
		                                                                    
		SRADCON1 = 0x00;            //关ADC模块                                          
		SRADCON2 = 0x00;                                                    
		                                                                           
		PT1 = 0x00;                                                                          
		PT1EN = 0xf7;               //pt1.3作输入口                                                          
		PT1PU = 0x08;               //I/O口上拉电阻设置                                                          
		PT1CON = 0x11;              //设置外部中断1和外部中断0                                            
		                                                                                     
		PT3 = 0x00;                                                       
		PT3EN = 0xff;                                                                        
		PT3PU = 0x00;                                               
		PT3CON = 0x00;                                                                       
		                                                                                     
		PT5 = 0x00;                                                                          
		PT5EN = 0xff;                                                                        
		PT5PU = 0x00;                                                                        
		PT5CON = 0x00;                                                                       
		                                                                                     
		TM0CON = 0x00;                                                                       
		TM2CON = 0x00;              //关闭定时/计数器2 并设置其为IO口                       
		TM3CON = 0x00;              //关闭定时/计数器3 并设置其为IO口                               
		                                                                                    
		WDTCON = 0x00;              //关闭WDT并设置溢出时间为2048ms                                                          
		CST_WDT = 1;                //关闭WDT晶振                                                            
		                                                                                     
		INTF = 0x00;                //开启外部中断1、总中断使能                                                               
		INTF2 = 0x00;               //关闭其他中断                                            
		INTE2 = 0x00;                                      
		INTE = 0x82;                                                                       
		                                                   
		asm("sleep");               //CPU执行睡眠指令后，所有的振荡器停止工作直到出现                                                                     
		asm("nop");                 //一个外部中断指令复位CPU。为了避免由中断返回                                                         
		asm("nop");                 //（Interrupt Return）引起的程序错误，建议停止指令之后                                                              
        asm("nop");                 //加一NOP指令以保证程序的正常运行

//      asm("call Clr_ram2");		//结束睡眠程序后建议重新对芯片进行初始化配置，但考虑到一些情况下，         
                                    //数据存储器的数据不想被更新，故这里不进行初始化配置，用户根据自己需要处理                                                        
	}                                                                                          
}