//ȫ�ֱ���������
sbit B_Time500ms;					//500ms��־λ
sbit B_pwm2;						//pwm2״̬��־λ
sbit B_pwm3;						//pwm3״̬��־λ
sbit B_Sleep;						//˯�߱�־λ
sbit  led @ 0x20*8+4;				//[�Զ���λʾ��]��led�����PT1_4
sbit  led1 @ 0x20*8+5;				//[�Զ���λʾ��]��led�����PT1_5
sbit  led2 @ 0x28*8+2;				//[�Զ���λʾ��]��led�����PT3_2
sbit if_adc,if_adc1,if_adc2,if_pwm;	//ADC��pwm��ģʽ�л���־�Ĵ���

unsigned int AdData;				//ADֵ�Ĵ���
unsigned int CurTabData;			//�������ݼĴ���
unsigned int t1s_count,t10s_count;	//��ʱ�����Ĵ���							
unsigned int R_AdcStation1_Cnt,R_AdcStation2_Cnt,R_AdcStation3_Cnt;//����������ѹADC�˲�����
								
//=================================
//  λ�����ʼ��                    
//=================================
void Bit_Initialization(void)
{
	if_adc=1;						//if_adcΪ1ʱ���ɼ�����pt3.1�ڵĵ�ѹֵ��if_adc1Ϊ1ʱ���ɼ�����оƬ�ڲ��ο���ѹ��ADֵ;
	if_adc1=0;
	if_adc2=0;						//if_adc2Ϊ1ʱ�����õ��ǲ������ɼ�����p3.1�ڵĵ�ѹֵ;
	if_pwm=0;						//if_pwmΪ1ʱ�����pwm2�������������if_pwmΪ0ʱ���������pwm3
}
