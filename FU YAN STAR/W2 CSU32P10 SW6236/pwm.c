#include "CSU32P10.h"
#include "define_data.h"
#include "define_function.h"

//����pwm����
void open_pwm()
{
	if(if_pwm)
	{
		if (B_Time500ms)				//��B_Time500ms��־Ϊ1ʱ�򿪷�����
		{                   			
			if (!B_pwm2)				//��pwm2ʱֻ�ܿ���һ�Σ��Ա�֤pwm���ε��ȶ�����B_pwm2����  
			{                           //  		��Ϊ1��˵���Ѿ�����pwm���򲻲���ֱ���˳�           
				B_pwm2=1;
				TM2CON=0x00;
				TM2RH=0;
				TM2INH=0;
				TM2R= CONST_PWM2_TMR;	//125,ռ�ձ�	
				TM2IN=CONST_PWM2_TMIN;	//249,Ƶ��4kHz
				TM2CON=0x92;			//����pwm2�������������
			}
		}
		else
		{
			TM2CON=0x00;     			//��B_Time500ms��־Ϊ0ʱ��pwm2			 
			TM2R=0;           
			TM2IN=0;          
			B_pwm2=0;         			
		}
	}
	else
	{
		if(!B_pwm3)      				//�򿪻���pwm3ʱֻ�ܿ���һ�Σ��Ա�֤pwm���ε��ȶ�����B_pwm3����   
		{                               //		��Ϊ1��˵���Ѿ�����pwm���򲻲���ֱ���˳�               
			B_pwm3=1;     
			TM3CON=0x00;  
			TM3RH=0;       
			TM3INH=0;     
			TM3R=CONST_PWM3_TMR;		//20,ռ�ձ�  
			TM3IN=CONST_PWM3_TMIN;		//79,Ƶ��200kHz
			METCH2=0x04;				//��ʱ��3ʱ��ѡ�� MCK  =16M
			TM3CON2=0X27; 				//��������ʱ��
			TM3CON=0X80;				//��pwm3
			t10s_count=0;				//��30s��ʱ����������ʼ30s��ʱ��30s��ر�pwm
		}		
	}   
		
}       
	    
			
        
        
        
        
        