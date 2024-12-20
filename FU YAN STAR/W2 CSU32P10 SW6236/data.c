//全局变量定义区
sbit B_Time500ms;					//500ms标志位
sbit B_pwm2;						//pwm2状态标志位
sbit B_pwm3;						//pwm3状态标志位
sbit B_Sleep;						//睡眠标志位
sbit  led @ 0x20*8+4;				//[自定义位示例]，led定义成PT1_4
sbit  led1 @ 0x20*8+5;				//[自定义位示例]，led定义成PT1_5
sbit  led2 @ 0x28*8+2;				//[自定义位示例]，led定义成PT3_2
sbit if_adc,if_adc1,if_adc2,if_pwm;	//ADC和pwm的模式切换标志寄存器

unsigned int AdData;				//AD值寄存器
unsigned int CurTabData;			//读表数据寄存器
unsigned int t1s_count,t10s_count;	//计时计数寄存器							
unsigned int R_AdcStation1_Cnt,R_AdcStation2_Cnt,R_AdcStation3_Cnt;//三级采样电压ADC滤波计数
								
//=================================
//  位定义初始化                    
//=================================
void Bit_Initialization(void)
{
	if_adc=1;						//if_adc为1时，采集的是pt3.1口的电压值；if_adc1为1时，采集的是芯片内部参考电压的AD值;
	if_adc1=0;
	if_adc2=0;						//if_adc2为1时，采用的是差分输入采集负端p3.1口的电压值;
	if_pwm=0;						//if_pwm为1时，输出pwm2作蜂鸣器输出；if_pwm为0时，输出互补pwm3
}
