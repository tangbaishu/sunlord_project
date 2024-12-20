#include "CSU32P10.h"
#include "define_data.h"
#include "define_function.h"

//ADC模式选择
void adc()
{
	if(if_adc)					//以VDD为参考电压，测量P3.1口的信号量
	{
		SRADCON1=0X80;			//使能AD采样，VDD作参考电压
		SRADCON2=0X10;			//设置pt3.1口为输入通道
		PT3CON=0x02;			//设置pt3.1为模拟口
		ad_fun();				//调用采AD函数 
		ad_deal();				//调用AD处理函数 
	}                       	
	else if(if_adc1)			//以P3.0口的输入为参考电压，内部基准作输入信号量，反测P3.0口电压         	
	{                       	
		SRADCON1=0X81;      	//使能AD转换，设置P3.0口的输入为参考电压   
		SRADCON2=0X60;			//设置输入通道(AIN6)为内部参考电压
		PT3CON=0X01;        	//P3.0口设置为模拟口
		METCH2=0x54;				//设置内部参考电压2V
		ad_fun();				//调用采AD函数
	}
//;==============================================================	
//;差分模式AD采样，负端默认为P3.1口，差分采样有两种模式，
//一种为单口模式，其方法为把正端通过SRADCON2设置为内部接地或接基准电压，信号量从负端P3.1口输入进行测量；
//;另一种为双口模式，其方法为信号量由正端输入，通过SRADCON2给正端选择输入口，负端P3.1外部接地，这种方法更加精确，但要用到两个口
//;这里采用单口模式示例，正端内部接地，采用负端P3.1口测量信号量
//;==============================================================	
	else if(if_adc2) 
	{
		SRADCON1=0X83;      	//使能AD转换，设置内部基准为参考电压   
		SRADCON2=0XF0;			//使能差分模式并设置正端内部接地（pt3.1口默认为负端输入）
		PT3CON=0X02;        	//设置3.1为模拟口
		METCH2=0x54;			//设置内部参考电压2V
		ad_fun();				//调用采AD函数		
	}
	
}

//ADC采集函数
void ad_fun()
{
	int temp;							//AD值临时寄存器
	int i;								//计数寄存器
	OFFEX = 0;							//offset交换置零
	if(if_adc2)
	{
		OFFEX = 1;						//offset交换置1,正负端交换,采用单口模式的差分输入，负端P3.1采集信号量，正端内部接地，
										//所以OFFSET置1把正负端交换，使得到的结果仍为正，方便AD数据处理；这里不交换也可以，
										//后面差分AD数据处理也做了AD正负值的判定和相对应的处理
	}
	AdData = 0;                 		//清ADC采样数据的高低位寄存器        
	delay_10us();						//AD转换前延时10us
	for(i=1;i<=8;i++)		    		//循环采八次AD值                    
	{		                    		                                    
		SRADS=1;                		//开始AD 转换                       
		asm("nop");	            		                                   
		while(SRADS);           		//等待AD 转换                       
		if(!if_adc2)
		{
			OFFEX = !OFFEX;         	//offset 交换 
		}
		temp=SRADH;             		//取AD值的高位                      
		AdData=(temp<<8)+SRADL+AdData; 	//累加八次AD值到AdData寄存器           
	}                                                                           
	C=0;					//bcf	status,c               
	AdData>>=3;							//求八次平均，最终结果存在AdData寄存器
//----------------------------------------------------------		
//AD数据处理 选择差分输入时（正端接P3.3，负端接P3.1）
	if(if_adc2)							//判断是否差分输入
	{
		if(AdData & 0x0800)				//判断AD值的最高位（1为正数，0为负数）
		{
			AdData = AdData & 0xf7ff;			//为正数，则把符号位清零
		}
		else
		{
			AdData = 0x0800 - AdData;	//为负数，则用800H-AD值，转换为正数
		}
	C = 0;
	AdData <<=1;						//AD值乘2,恢复为12位AD采样数据
										//这个数据即为最终8次平均后的AD采样数据，计算方法为AD=信号量/2V参考*4096	
	}	
}                                                              
//ADC处理函数                                                  
void ad_deal()                                                 
{	                                                           
	asm("movlw	TabStartAddr");
	asm("movwf	_EADRL");
	asm("movlw	.high(TabStartAddr)");	//取表地址高位
	asm("movwf	_EADRH");				//赋读表地址	
	asm("movp");						//读表
	asm("nop");
	asm("movwf	_CurTabData");
	asm("movfw	_EDATH");
	asm("movwf	_CurTabData+1");		//表值
										//		读出寄存器，再进行比较，这种做法较为保险
	if(AdData<CurTabData)					//比较表值和采集到的AD值
	{									//若AD值小则关led1,led2指示灯
		R_AdcStation2_Cnt=0;			                 
		R_AdcStation3_Cnt=0;                                                        
		R_AdcStation1_Cnt++;                                                        
		if(R_AdcStation1_Cnt>=20)       //滤波计数                                    
		{                               //连续20次AD值比第一个表值小则关掉led1，led2  
			R_AdcStation1_Cnt=0;                                                    
			led1=0;                     //关led1，led2                                             
			led2=0;                                                                  
		}                                                                            
	}                                                                   
	else 			
	{									//若AD值小则亮led1
		asm("movlw	1");
		asm("addwf	_EADRL,1");
		asm("clrf	_WORK");
		asm("addwfc _EADRH,1");			//赋读表地址
		asm("movp");					//读表
		asm("nop");
		asm("movwf	_CurTabData");
		asm("movfw	_EDATH");
		asm("movwf	_CurTabData+1");	//表值

		if(AdData<CurTabData)           //比较表值和采集到的AD值                    
		{
			R_AdcStation1_Cnt=0;			                    
			R_AdcStation3_Cnt=0;                                
			R_AdcStation2_Cnt++;                       
			if(R_AdcStation2_Cnt>=10)   //滤波计数                     
			{                           //连续10次AD值比第二个表值小则亮led1，灭led2                    
				R_AdcStation2_Cnt=0;                        
				led1=1;                 //亮led1，灭led2                      
				led2=0;	                                     
			}
		}	                                    
		else                                
		{								//若AD值大则亮led2
			R_AdcStation1_Cnt=0;
			R_AdcStation2_Cnt=0;
			R_AdcStation3_Cnt++;
			if(R_AdcStation3_Cnt>=10)	//滤波计数                                  
			{                           //连续10次AD值比第二个表值大则亮led2，灭led1
				R_AdcStation3_Cnt=0;
				led1=0;					//亮led2，灭led1 
				led2=1;	
			}
		}
	}

}
	