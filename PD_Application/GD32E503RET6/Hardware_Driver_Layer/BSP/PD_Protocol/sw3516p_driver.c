#include "sw3516p_driver.h"
#include "IIC_Driver/iic_api.h"
#include "OLED/oled.h"
#include "stdio.h"

#define DEVICE_ADDRESS		0x3C	//0x3C	//0x3C

SW3516P_Driver_Info_t		SW3516P_Driver_Info;

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
		IIC_SOFTWARE_INIT();
		IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, 0x01, &Version_Num);
		SW3516P_Get_State();
		printf("Version_Num = 0x%x\r\n", Version_Num);
	}
#endif // !READ_DEVICE_VERSION


void SW3516P_Get_State(void)
{
	uint8	num = 0;
	uint16	ram_data = 0;
	for(;num<3;num++)
	{
		IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, FAST_CHARGE_INDICATION + num, (&SW3516P_Driver_Info.IC_Work_State.Fast_Charge_State + num));
		printf("%#x = %#X ", 0x06+num, *(uint8 *)(&SW3516P_Driver_Info.IC_Work_State.Fast_Charge_State + num));
	}
	printf("\r\n");
	for(num=0; num<2; num++)
	{
		ram_data = 0;
		IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, PORT_1_CURRENT_LIMITING + num, (uint8*)&ram_data);
		ram_data &= 0x7F;	//舍弃Bit7,有效位：Bit0~6
		ram_data = 1000 + ram_data * 50;	//转换为电流值，单位：mA
		*(uint16 *)(&SW3516P_Driver_Info.IC_Work_State.Port1_Limiting_Value + num) *= ram_data;
		printf("PORT_%d_CURRENT_LIMITING = %d", num, *(uint16 *)(&SW3516P_Driver_Info.IC_Work_State.Port1_Limiting_Value + num));
	}
	printf("\r\n");
}

void SW3516P_Init(void)
{
	uint8 num=0;
	Read_Device_Version();
	/**I2C 写操作使能 操作寄存器 REG0xA0~D1 */
	for(;num < 3;num++)
	{
		IIC_WRITE_SINGLE_DATA(DEVICE_ADDRESS, IIC_ENABLE_CONTROL, 0x20<<num);
	}

	for(uint16 i=0; i<4; i++)
	{
		SW3516P_Power_Port_Config(i);
		SW3516P_Get_State();
	}


	while(1);
}

/**
 * @brief SW3516P电源端口设置
 * 
 * @param param 
 * 0：单A口
 * 1：双A口
 * 2: 单C口
 * 3: C+A口
 */
void SW3516P_Power_Port_Config(uint8 param)
{
	uint8 ram_reg=0;
	IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, PORT_MODE_CONFIG, &ram_reg);
	printf("READ: ram_reg = %#x \r\n", ram_reg);
	ram_reg &= 0xF3;	//0B1111 0011
	ram_reg |= param << 2;
	printf("WRITE: ram_reg = %#x \r\n", ram_reg);
	IIC_WRITE_SINGLE_DATA(DEVICE_ADDRESS, PORT_MODE_CONFIG, ram_reg);
}


/**
 * @brief SW3516P端口限流配置
 * 
 * @param port1 	0:2.6A、 1:2.2A、 2:1.7A、 3:3.2A	5:无需配置
 * @param port2 	0:2.6A、 1:2.2A、 2:1.7A、 3:3.2A、 4:与1口配置一致、 5:无需配置
 * @param offset	0:150mA、1:450mA、2:600mA、3:300mA、5:无需配置
 */
void SW3516P_Current_Limiting(uint8 port1, uint8 port2, uint8 offset)
{
	uint8 ram_data=0;
	if(port1 != 5)
	{
		IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, CURRENT_LIMITING_0, &ram_data);
		printf("READ:0xBD = %#x ",ram_data);
		ram_data &= 0xCF;		// 0B11001111 将待设置位置0
		ram_data |= port1 << 4;	// 写入对应配置
		printf("WRITE:0xBD = %#x ",ram_data);
		IIC_WRITE_SINGLE_DATA(DEVICE_ADDRESS, CURRENT_LIMITING_0, ram_data);
	}

	if(port2 == 5 && offset ==5)
	{
		return;
	}

	IIC_READ_SINGLE_DATA(DEVICE_ADDRESS, CURRENT_LIMITING_1, &ram_data);
	if(port2 != 5)
	{
		printf("READ:0xBD = %#x ",ram_data);
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
	printf("WRITE:0xBD = %#x ",ram_data);
	IIC_WRITE_SINGLE_DATA(DEVICE_ADDRESS, CURRENT_LIMITING_0, ram_data);
	printf("\r\n");
}