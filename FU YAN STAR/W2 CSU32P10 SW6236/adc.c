#include "CSU32P10.h"
#include "define_data.h"
#include "define_function.h"

//ADCģʽѡ��
void adc()
{
	if(if_adc)					//��VDDΪ�ο���ѹ������P3.1�ڵ��ź���
	{
		SRADCON1=0X80;			//ʹ��AD������VDD���ο���ѹ
		SRADCON2=0X10;			//����pt3.1��Ϊ����ͨ��
		PT3CON=0x02;			//����pt3.1Ϊģ���
		ad_fun();				//���ò�AD���� 
		ad_deal();				//����AD������ 
	}                       	
	else if(if_adc1)			//��P3.0�ڵ�����Ϊ�ο���ѹ���ڲ���׼�������ź���������P3.0�ڵ�ѹ         	
	{                       	
		SRADCON1=0X81;      	//ʹ��ADת��������P3.0�ڵ�����Ϊ�ο���ѹ   
		SRADCON2=0X60;			//��������ͨ��(AIN6)Ϊ�ڲ��ο���ѹ
		PT3CON=0X01;        	//P3.0������Ϊģ���
		METCH2=0x54;				//�����ڲ��ο���ѹ2V
		ad_fun();				//���ò�AD����
	}
//;==============================================================	
//;���ģʽAD����������Ĭ��ΪP3.1�ڣ���ֲ���������ģʽ��
//һ��Ϊ����ģʽ���䷽��Ϊ������ͨ��SRADCON2����Ϊ�ڲ��ӵػ�ӻ�׼��ѹ���ź����Ӹ���P3.1��������в�����
//;��һ��Ϊ˫��ģʽ���䷽��Ϊ�ź������������룬ͨ��SRADCON2������ѡ������ڣ�����P3.1�ⲿ�ӵأ����ַ������Ӿ�ȷ����Ҫ�õ�������
//;������õ���ģʽʾ���������ڲ��ӵأ����ø���P3.1�ڲ����ź���
//;==============================================================	
	else if(if_adc2) 
	{
		SRADCON1=0X83;      	//ʹ��ADת���������ڲ���׼Ϊ�ο���ѹ   
		SRADCON2=0XF0;			//ʹ�ܲ��ģʽ�����������ڲ��ӵأ�pt3.1��Ĭ��Ϊ�������룩
		PT3CON=0X02;        	//����3.1Ϊģ���
		METCH2=0x54;			//�����ڲ��ο���ѹ2V
		ad_fun();				//���ò�AD����		
	}
	
}

//ADC�ɼ�����
void ad_fun()
{
	int temp;							//ADֵ��ʱ�Ĵ���
	int i;								//�����Ĵ���
	OFFEX = 0;							//offset��������
	if(if_adc2)
	{
		OFFEX = 1;						//offset������1,�����˽���,���õ���ģʽ�Ĳ�����룬����P3.1�ɼ��ź����������ڲ��ӵأ�
										//����OFFSET��1�������˽�����ʹ�õ��Ľ����Ϊ��������AD���ݴ������ﲻ����Ҳ���ԣ�
										//������AD���ݴ���Ҳ����AD����ֵ���ж������Ӧ�Ĵ���
	}
	AdData = 0;                 		//��ADC�������ݵĸߵ�λ�Ĵ���        
	delay_10us();						//ADת��ǰ��ʱ10us
	for(i=1;i<=8;i++)		    		//ѭ���ɰ˴�ADֵ                    
	{		                    		                                    
		SRADS=1;                		//��ʼAD ת��                       
		asm("nop");	            		                                   
		while(SRADS);           		//�ȴ�AD ת��                       
		if(!if_adc2)
		{
			OFFEX = !OFFEX;         	//offset ���� 
		}
		temp=SRADH;             		//ȡADֵ�ĸ�λ                      
		AdData=(temp<<8)+SRADL+AdData; 	//�ۼӰ˴�ADֵ��AdData�Ĵ���           
	}                                                                           
	C=0;					//bcf	status,c               
	AdData>>=3;							//��˴�ƽ�������ս������AdData�Ĵ���
//----------------------------------------------------------		
//AD���ݴ��� ѡ��������ʱ�����˽�P3.3�����˽�P3.1��
	if(if_adc2)							//�ж��Ƿ�������
	{
		if(AdData & 0x0800)				//�ж�ADֵ�����λ��1Ϊ������0Ϊ������
		{
			AdData = AdData & 0xf7ff;			//Ϊ��������ѷ���λ����
		}
		else
		{
			AdData = 0x0800 - AdData;	//Ϊ����������800H-ADֵ��ת��Ϊ����
		}
	C = 0;
	AdData <<=1;						//ADֵ��2,�ָ�Ϊ12λAD��������
										//������ݼ�Ϊ����8��ƽ�����AD�������ݣ����㷽��ΪAD=�ź���/2V�ο�*4096	
	}	
}                                                              
//ADC������                                                  
void ad_deal()                                                 
{	                                                           
	asm("movlw	TabStartAddr");
	asm("movwf	_EADRL");
	asm("movlw	.high(TabStartAddr)");	//ȡ���ַ��λ
	asm("movwf	_EADRH");				//�������ַ	
	asm("movp");						//����
	asm("nop");
	asm("movwf	_CurTabData");
	asm("movfw	_EDATH");
	asm("movwf	_CurTabData+1");		//��ֵ
										//		�����Ĵ������ٽ��бȽϣ�����������Ϊ����
	if(AdData<CurTabData)					//�Ƚϱ�ֵ�Ͳɼ�����ADֵ
	{									//��ADֵС���led1,led2ָʾ��
		R_AdcStation2_Cnt=0;			                 
		R_AdcStation3_Cnt=0;                                                        
		R_AdcStation1_Cnt++;                                                        
		if(R_AdcStation1_Cnt>=20)       //�˲�����                                    
		{                               //����20��ADֵ�ȵ�һ����ֵС��ص�led1��led2  
			R_AdcStation1_Cnt=0;                                                    
			led1=0;                     //��led1��led2                                             
			led2=0;                                                                  
		}                                                                            
	}                                                                   
	else 			
	{									//��ADֵС����led1
		asm("movlw	1");
		asm("addwf	_EADRL,1");
		asm("clrf	_WORK");
		asm("addwfc _EADRH,1");			//�������ַ
		asm("movp");					//����
		asm("nop");
		asm("movwf	_CurTabData");
		asm("movfw	_EDATH");
		asm("movwf	_CurTabData+1");	//��ֵ

		if(AdData<CurTabData)           //�Ƚϱ�ֵ�Ͳɼ�����ADֵ                    
		{
			R_AdcStation1_Cnt=0;			                    
			R_AdcStation3_Cnt=0;                                
			R_AdcStation2_Cnt++;                       
			if(R_AdcStation2_Cnt>=10)   //�˲�����                     
			{                           //����10��ADֵ�ȵڶ�����ֵС����led1����led2                    
				R_AdcStation2_Cnt=0;                        
				led1=1;                 //��led1����led2                      
				led2=0;	                                     
			}
		}	                                    
		else                                
		{								//��ADֵ������led2
			R_AdcStation1_Cnt=0;
			R_AdcStation2_Cnt=0;
			R_AdcStation3_Cnt++;
			if(R_AdcStation3_Cnt>=10)	//�˲�����                                  
			{                           //����10��ADֵ�ȵڶ�����ֵ������led2����led1
				R_AdcStation3_Cnt=0;
				led1=0;					//��led2����led1 
				led2=1;	
			}
		}
	}

}
	