#include "CSU32P10.h"
#ifndef  	_define_data_h
#define 	_define_data_h 1

#define led      PT1_4
#define led1     PT1_5
#define led2     PT3_2

extern unsigned int t1s_count;
extern volatile sbit led;
extern unsigned int AdData;
extern unsigned int CurTabData;		
extern unsigned int R_AdcStation1_Cnt,R_AdcStation2_Cnt,R_AdcStation3_Cnt;
extern volatile sbit led1;
extern volatile sbit led2;
extern sbit if_adc,if_adc1,if_adc2;

extern sbit B_Time500ms;
extern sbit B_pwm2;
extern sbit B_pwm3;
extern sbit if_pwm;
extern unsigned int t10s_count;

extern sbit B_Sleep;						

//============================================
// 宏定义文件
//============================================
//[宏定义示例]，只支持一种形式，即#define 标识符 数值
#define CONST_PWM2_TMR 125	
#define CONST_PWM2_TMIN 249
#define CONST_PWM3_TMR 20
#define CONST_PWM3_TMIN 79

#endif   
