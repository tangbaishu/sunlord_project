#include "BSP/PD_Protocol_IC/sw6233_driver.h"
#include "i2c_api.h"
#include "zr_systick.h"
#include "BSP/OLED/oled.h"
#include "stdio.h"
#include "LED_Driver/led_driver.h"

#define DEVICE_ADDRESS		0x3C	//0x3C	//0x3C

uint32 				Coulombmeter_Value=0;
SW6233_Driver_t		SW6233_Driver;

typedef enum
{
	ONLY_PORTA = 1,
	ONLY_PORTC,
	DUAL_PORTA,
	DUAL_PORTC,
}Power_Out_Mode_e;							// 支持的端口输出类型

#ifdef READ_DEVICE_VERSION
	void Read_Device_Version(void)
	{
		uint8	Version_Num=0;
		IIC_MASTER_INIT(DEVICE_ADDRESS);
		IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, 0x01, &Version_Num);
		printf("Version_Num = 0x%x\r\n", Version_Num);
	}
#endif // !READ_DEVICE_VERSION


void SW6233_Get_State(void)
{
	uint8	num = 0;
	uint16	ram_data = 0;
	for(;num<3;num++)
	{
		IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, FAST_CHARGE_INDICATION + num, (&SW6233_Driver.IC_Work_State.Fast_Charge_State + num));
	}
	for(num=0; num<2; num++)
	{
		ram_data = 0;
		IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, PORT_1_CURRENT_LIMITING + num, (uint8*)&ram_data);
		ram_data &= 0x7F;	//舍弃Bit7,有效位：Bit0~6
		ram_data = 1000 + ram_data * 50;	//转换为电流值，单位：mA
		*(uint16 *)(&SW6233_Driver.IC_Work_State.Port1_Limiting_Value + num) *= ram_data;
		printf("PORT_%d_CURRENT_LIMITING = %d mA", num, *(uint16 *)(&SW6233_Driver.IC_Work_State.Port1_Limiting_Value + num));
	}
}

// void SW6233_Mode_En(void)
// {
// 	rcu_periph_clock_enable(SW6233_IIC_EN_GPIO_CLOCK);
// 	gpio_init(SW6233_IIC_EN_PORT_PIN, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_MAX, SW6233_IIC_EN_GPIO_PIN);
// 	Systick_Delay_Ms(100);
// 	gpio_bit_write(SW6233_IIC_EN_PORT_PIN, SW6233_IIC_EN_GPIO_PIN, RESET);
// 	gpio_init(SW6233_IIC_EN_PORT_PIN, GPIO_MODE_OUT_PP, GPIO_OSPEED_MAX, SW6233_IIC_EN_GPIO_PIN);
// 	Systick_Delay_Ms(100);
// 	gpio_init(SW6233_IIC_EN_PORT_PIN, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_MAX, SW6233_IIC_EN_GPIO_PIN);
// }

void SW6233_Working_State_Check(void)
{
	uint8 ram_data=0;
	uint8 printf_data[3]={0};

	IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, 0xC4, &ram_data);			// 读取: 0xC4 放电配置 3 默认值：0x89

	IIC_READ_MULTI_DATA(DEVICE_ADDRESS, 0x0A, &printf_data[0],3);	// 读取: 0x0A~0x0C	默认值：0x12、0x02、0x02

	IIC_READ_MULTI_DATA(DEVICE_ADDRESS, 0x19, &printf_data[0],2);	// 读取: 0x19~0x1A	默认值：0x00、0x00、0x00

	IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, 0x1C, &ram_data);			// 读取: 0x1C		默认值：0x00

	IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, 0x20, &ram_data);			// 读取: 0x20		默认值：0x00

	IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, 0x2B, &ram_data);			// 读取: 0x2B		默认值：0x00

	IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, 0x2D, &ram_data);			// 读取: 0x2D		默认值：0x03

	IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, 0x31, &ram_data);			// 读取: 0x31		默认值：0x00

	IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, 0x34, &ram_data);			// 读取: 0x34	设置输出限流	默认值：0x00

	IIC_READ_MULTI_DATA(DEVICE_ADDRESS, 0x58, &printf_data[0], 2);	// 读取: 0x58~0x59	默认值：0x64、0x64


	Coulombmeter_Value = Get_Coulombmeter();
	printf("Get_Coulombmeter = %d\r\n", Coulombmeter_Value);		// 默认值: 532479

	IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, 0xB2, &ram_data);			// 读取: 0xB2						默认值：0x80

	IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, 0xB4, &ram_data);			// 读取: 0xB4	无线充和 PD 配置 2	默认值：0x18

	IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, 0xBA, &ram_data);			// 读取: 0xBA	端口快充配置		默认值：

	IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, 0xC4, &ram_data);			// 读取: 0xC4						默认值：0x89

	IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, 0x1A, &ram_data);			// 默认值：0x00
	printf("IBUS = %f\r\n", Transfor_ADC_Data(0x41, 0x42, (float)25/11));
	
}

void SW6233_Init(void)
{
	uint8 num=0, ram_data=0;
	// SW6233_Mode_En();
	#ifdef READ_DEVICE_VERSION
		Read_Device_Version();
	#endif
	printf("running\r\n");

	IIC_WRITE_SINGLE_DATA(DEVICE_ADDRESS, 0X2D, 0x01);	// 退出低功耗模式
	Systick_Delay_Ms(200);
	/**I2C 写操作使能 操作寄存器 REG0xA0~D1 */
	for(;num < 3;num++)
	{
		IIC_WRITE_SINGLE_DATA(DEVICE_ADDRESS, IIC_ENABLE_CONTROL, 0x20<<num);
	}
	IIC_WRITE_SINGLE_DATA(DEVICE_ADDRESS, 0x40, 0x01);	// ADC配置 输入/输出 IBUS 电流 单位:25/11=2.27 mA/step

	IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, 0xC4, &ram_data);		// 读取：放电配置 3
	ram_data &= 0xFD;
	IIC_WRITE_SINGLE_DATA(DEVICE_ADDRESS, 0xC4, ram_data);		// 写入：放电配置 3 -> bit1: 放电限流门限偏移量 = 5%

	LED_Driver_ON(1);						// 初始化完成状态指示灯

	while (1)
	{
		SW6233_Working_State_Check();
		Systick_Delay_Ms(200);
	}
	
	
}

void SW6233_Abnormal_Reset(void)
{
	uint8 ram_data=0;
	IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, 0x2B, &ram_data);			// 读取: 0x2B		默认值：0x00
	if((ram_data & 0x20 )== 0x20)
	{
		printf("Type-C Output\r\n");
		IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, 0x2B, &ram_data);			// 读取: 0x2B		默认值：0x00
	}
}


/**
 * @brief 关闭除PD以为的所有协议
 * 
 */
void Close_All_Besides_PD(void)
{
	uint8 ram_data=0;

	IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, 0xB6, &ram_data);
	ram_data |= 0x08;
	IIC_WRITE_SINGLE_DATA(DEVICE_ADDRESS, 0xB6, ram_data);	// 端口检测配置 2 -> 关闭 C1/C2 口 DM 接入检测
	
	IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, 0xB1, &ram_data);
	ram_data |= 0xEC;
	IIC_WRITE_SINGLE_DATA(DEVICE_ADDRESS, 0xB1, ram_data);	// 快充协议开关 1 -> 关闭该寄存器下的所有协议

	IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, 0xCD, &ram_data);
	ram_data |= 0xF8;
	IIC_WRITE_SINGLE_DATA(DEVICE_ADDRESS, 0xCD, ram_data);	// 快充协议开关 2 -> 关闭该寄存器下的所有协议
}

/**
 * @brief 获取库仑计当前容量
 * 
 * @return uint32 
 */
uint32 Get_Coulombmeter(void)
{
	uint8 capacity[3]={0};
	uint32 ram_coulombmeter_value=0;
	IIC_READ_MULTI_DATA(DEVICE_ADDRESS, 0x5C, &capacity[0],3);
	ram_coulombmeter_value = (uint32)capacity[2]<<16;
	ram_coulombmeter_value |= (uint16)capacity[1]<<8;
	ram_coulombmeter_value |= capacity[0];
	if(Coulombmeter_Value == 0)												// 若系统库仑计容量值 == 0,立即更新
	{
		Coulombmeter_Value = ram_coulombmeter_value;
	}
	if(ram_coulombmeter_value < (Coulombmeter_Value - NORMAL_POWER))		// 若剩余容量正常减少，更新 系统库仑计容量值
	{
		Coulombmeter_Value = ram_coulombmeter_value;
	}
	else
	{
		printf("更新芯片库仑计值 = %d\r\n", Coulombmeter_Value);
		capacity[2] = Coulombmeter_Value >> 16;
		capacity[1] = Coulombmeter_Value >> 8;
		capacity[0] = Coulombmeter_Value;
	}
	return ram_coulombmeter_value;
}


/**
 * @brief 将芯片ADC的数据转换为实际的物理量
 * 
 * @param low_8bit_reg 		ADC低8位
 * @param high_4bit_reg 	ADC高4位
 * @param step 				ADC每一位对应的物理步进值，即12bitADC 1bit对应的单位值
 * @return float 			返回实际物理量
 */
float Transfor_ADC_Data(uint8 low_8bit_reg, uint8 high_4bit_reg, float step)
{
	uint8 adc_data[2]={0};
	IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, low_8bit_reg, &adc_data[0]); 	// 获取低8位
	IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, high_4bit_reg, &adc_data[1]); 	// 获取高4位
	adc_data[1] &= 0x0F;
	return (((uint16)adc_data[1]<<8 | adc_data[0]) * step);
}

/**
 * @brief SW6233电源端口设置
 * 
 * @param param 
 * 0：单A口
 * 1：双A口
 * 2: 单C口
 * 3: C+A口
 */
void SW6233_Power_Port_Config(uint8 param)
{
	uint8 ram_reg=0;
	IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, PORT_MODE_CONFIG, &ram_reg);
	ram_reg &= 0xF3;	//0B1111 0011
	ram_reg |= param << 2;
	IIC_WRITE_SINGLE_DATA(DEVICE_ADDRESS, PORT_MODE_CONFIG, ram_reg);
}


/**
 * @brief SW6233端口限流配置
 * 
 * @param port1 	0:2.6A、 1:2.2A、 2:1.7A、 3:3.2A	5:无需配置
 * @param port2 	0:2.6A、 1:2.2A、 2:1.7A、 3:3.2A、 4:与1口配置一致、 5:无需配置
 * @param offset	0:150mA、1:450mA、2:600mA、3:300mA、5:无需配置
 */
void SW6233_Current_Limiting(uint8 port1, uint8 port2, uint8 offset)
{
	uint8 ram_data=0;
	if(port1 != 5)
	{
		IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, CURRENT_LIMITING_0, &ram_data);
		ram_data &= 0xCF;		// 0B11001111 将待设置位置0
		ram_data |= port1 << 4;	// 写入对应配置
		IIC_WRITE_SINGLE_DATA(DEVICE_ADDRESS, CURRENT_LIMITING_0, ram_data);
	}

	if(port2 == 5 && offset ==5)
	{
		return;
	}

	IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, CURRENT_LIMITING_1, &ram_data);
	if(port2 != 5)
	{
		if(port2 == 4)	//与1口配置一致 
		{
			ram_data |= 0x10;		// Bit4 置1
		}
		else
		{
			ram_data &= 0xEF;		// Bit4 置0
			ram_data &= 0xF3;		// 0B11110011 将待设置位置0
			ram_data |= port2 << 2;	// 写入对应配置
		}
	}
	if(offset != 5)
	{
		ram_data &= 0xFC;
		ram_data |= offset;
	}
	IIC_WRITE_SINGLE_DATA(DEVICE_ADDRESS, CURRENT_LIMITING_0, ram_data);
}