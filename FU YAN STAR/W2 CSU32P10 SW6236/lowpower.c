#include "CSU32P10.h"
#include "define_data.h"
#include "define_function.h"


//sleep����
void sleep()
{
	if (B_Sleep)					//�ж�Sleep��־                                             
	{                                                                                         
		INTE = 0x00;				//�ȹص����ж�ʹ��                                                           
		                                                                    
		SRADCON1 = 0x00;            //��ADCģ��                                          
		SRADCON2 = 0x00;                                                    
		                                                                           
		PT1 = 0x00;                                                                          
		PT1EN = 0xf7;               //pt1.3�������                                                          
		PT1PU = 0x08;               //I/O��������������                                                          
		PT1CON = 0x11;              //�����ⲿ�ж�1���ⲿ�ж�0                                            
		                                                                                     
		PT3 = 0x00;                                                       
		PT3EN = 0xff;                                                                        
		PT3PU = 0x00;                                               
		PT3CON = 0x00;                                                                       
		                                                                                     
		PT5 = 0x00;                                                                          
		PT5EN = 0xff;                                                                        
		PT5PU = 0x00;                                                                        
		PT5CON = 0x00;                                                                       
		                                                                                     
		TM0CON = 0x00;                                                                       
		TM2CON = 0x00;              //�رն�ʱ/������2 ��������ΪIO��                       
		TM3CON = 0x00;              //�رն�ʱ/������3 ��������ΪIO��                               
		                                                                                    
		WDTCON = 0x00;              //�ر�WDT���������ʱ��Ϊ2048ms                                                          
		CST_WDT = 1;                //�ر�WDT����                                                            
		                                                                                     
		INTF = 0x00;                //�����ⲿ�ж�1�����ж�ʹ��                                                               
		INTF2 = 0x00;               //�ر������ж�                                            
		INTE2 = 0x00;                                      
		INTE = 0x82;                                                                       
		                                                   
		asm("sleep");               //CPUִ��˯��ָ������е�����ֹͣ����ֱ������                                                                     
		asm("nop");                 //һ���ⲿ�ж�ָ�λCPU��Ϊ�˱������жϷ���                                                         
		asm("nop");                 //��Interrupt Return������ĳ�����󣬽���ָֹͣ��֮��                                                              
        asm("nop");                 //��һNOPָ���Ա�֤�������������

//      asm("call Clr_ram2");		//����˯�߳���������¶�оƬ���г�ʼ�����ã������ǵ�һЩ����£�         
                                    //���ݴ洢�������ݲ��뱻���£������ﲻ���г�ʼ�����ã��û������Լ���Ҫ����                                                        
	}                                                                                          
}