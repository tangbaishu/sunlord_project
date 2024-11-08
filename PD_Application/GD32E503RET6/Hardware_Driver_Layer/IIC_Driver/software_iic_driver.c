#include "IIC_Driver/software_iic_driver.h"


void software_iic_Init(void)
{
	rcu_periph_clock_enable(RCU_GPIOB);
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10 | GPIO_PIN_11);
}

/**********************************************
//IIC Start
**********************************************/
void IIC_Start(void)
{

	OLED_SCLK_Set();
	OLED_SDIN_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop(void)
{
	OLED_SCLK_Set();
	//	OLED_SCLK_Clr();
	OLED_SDIN_Clr();
	OLED_SDIN_Set();
}

void IIC_Wait_Ack(void)
{

	// GPIOB->CRH &= 0XFFF0FFFF;	//设置PB12为上拉输入模式
	// GPIOB->CRH |= 0x00080000;
	//	OLED_SDA = 1;
	//	delay_us(1);
	// OLED_SCL = 1;
	// delay_us(50000);
	/*	while(1)
		{
			if(!OLED_SDA)				//判断是否接收到OLED 应答信号
			{
				//GPIOB->CRH &= 0XFFF0FFFF;	//设置PB12为通用推免输出模式
				//GPIOB->CRH |= 0x00030000;
				return;
			}
		}
	*/
	OLED_SCLK_Set();
	OLED_SCLK_Clr();
	delay_1ms(1);
}

/**********************************************
// IIC Write byte
**********************************************/
void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m, da;
	da = IIC_Byte;
	OLED_SCLK_Clr();
	for (i = 0; i < 8; i++)
	{
		m = da;
		//	OLED_SCLK_Clr();
		m = m & 0x80;
		if (m == 0x80)
		{
			OLED_SDIN_Set();
		}
		else
			OLED_SDIN_Clr();
		da = da << 1;
		OLED_SCLK_Set();
		OLED_SCLK_Clr();
	}
}

/**
 * @brief 
 * IIC单数据写入 
 * 返回值 1:PASS、0:ERROR
 */
unsigned char IIC_Write_Single_Data(unsigned char device_id, unsigned char reg_adr, unsigned char data)
{
	IIC_Start();
	Write_IIC_Byte(device_id); 		// 设备ID
	IIC_Wait_Ack();
	Write_IIC_Byte(reg_adr); 		// 设备寄存器地址
	IIC_Wait_Ack();
	Write_IIC_Byte(data);			// 寄存器写入数据
	IIC_Wait_Ack();
	IIC_Stop();
	return 0;
}	

/**
 * @brief 
 * IIC多数据写入 
 * 返回值 1:PASS、0:ERROR
 */
unsigned char IIC_Write_Multi_Data(unsigned char device_id, unsigned char reg_adr, unsigned char *data, unsigned char data_len)
{
	IIC_Start();
	Write_IIC_Byte(device_id); 		// 设备ID
	IIC_Wait_Ack();
	Write_IIC_Byte(reg_adr); 		// 设备寄存器地址
	IIC_Wait_Ack();
	Write_IIC_Byte(*data);			// 寄存器写入数据
	IIC_Wait_Ack();
	IIC_Stop();
	return 0;
}
	
/**
 * @brief 
 * IIC单数据读取
 * 返回值 1:PASS、0:ERROR
 */
unsigned char IIC_Read_Single_Data(unsigned char device_id, unsigned char reg_adr, unsigned char *data)
{
	return 0;
}

/**
 * @brief 
 * IIC多数据读取
 * 返回值 1:PASS、0:ERROR
 */
unsigned char IIC_Read_Multi_Data(unsigned char device_id, unsigned char reg_adr, unsigned char *data, unsigned char data_len)
{
	return 0;
}

/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
	IIC_Start();
	Write_IIC_Byte(0x78); // Slave address,SA0=0
	IIC_Wait_Ack();
	Write_IIC_Byte(0x00); // write command
	IIC_Wait_Ack();
	Write_IIC_Byte(IIC_Command);
	IIC_Wait_Ack();
	IIC_Stop();
}

/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
	IIC_Start();
	Write_IIC_Byte(0x78); // D/C#=0; R/W#=0
	IIC_Wait_Ack();
	Write_IIC_Byte(0x40); // write data
	IIC_Wait_Ack();
	Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();
	IIC_Stop();
}