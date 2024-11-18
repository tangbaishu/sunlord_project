#include "IIC_Driver/software_iic_driver.h"
#include "stdio.h"


void software_iic_Init(void)
{
	rcu_periph_clock_enable(SOFTWARE_IIC_COLCK);
	IIC_SDA_PIN_HIGH();
	IIC_SCL_PIN_HIGH();
	gpio_init(SOFTWARE_IIC_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, SOFTWARE_IIC_SCL_PIN | SOFTWARE_IIC_SDA_PIN);

}

/**
 * @brief 实测9.4us
 * 
 * @param n 
 */
void delay_10us(unsigned char n)
{
	volatile unsigned char num=0,i=0;
	for(; i < n; i++)
	{
		for(num=0; num<80; num++)
		{
			__NOP();
		}
	}
}

void SDA_Test(void)
{
	software_iic_Init();
	while (1)
	{
		IIC_SDA_PIN_LOW();
		delay_10us(1);
		IIC_SDA_PIN_INPUT();
		delay_10us(1);
		IIC_SDA_PIN_OUTPUT();
	}
}

/**********************************************
//IIC Start
**********************************************/
void IIC_Start(void)
{
	IIC_SDA_PIN_HIGH();
	IIC_SDA_PIN_OUTPUT();
	IIC_SCL_PIN_HIGH();
	IIC_SCL_PIN_OUTPUT();

	delay_10us(1);
	IIC_SDA_PIN_LOW();
	delay_10us(1);
	IIC_SCL_PIN_LOW();
	delay_10us(1);
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop(void)
{
	IIC_SDA_PIN_LOW();
	IIC_SDA_PIN_OUTPUT();
	IIC_SCL_PIN_HIGH();
	delay_10us(1);
	IIC_SDA_PIN_HIGH();
	delay_10us(1);
	IIC_SDA_PIN_INPUT();
	IIC_SCL_PIN_INPUT();
}

void IIC_Wait_Ack(void)
{
	IIC_SDA_PIN_INPUT();
	while (IIC_SDA_PIN_READ());
	IIC_SCL_PIN_HIGH();
	delay_10us(1);
	IIC_SCL_PIN_LOW();
	delay_10us(1);
	IIC_SDA_PIN_LOW();
	IIC_SDA_PIN_OUTPUT();
}

void IIC_Send_Ack(void)
{
	IIC_SDA_PIN_LOW();
	IIC_SCL_PIN_HIGH();
	delay_10us(1);
	IIC_SCL_PIN_LOW();
	delay_10us(1);
}

void IIC_Send_NAck(void)
{
	IIC_SCL_PIN_HIGH();
	delay_10us(1);
	IIC_SCL_PIN_LOW();
	delay_10us(1);
}

/**********************************************
// IIC Write byte
**********************************************/
void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m, da;
	da = IIC_Byte;
	IIC_SCL_PIN_LOW();
	for (i = 0; i < 8; i++)
	{
		m = da;
		//	IIC_SCL_PIN_LOW();
		m = m & 0x80;
		if (m == 0x80)
		{
			IIC_SDA_PIN_HIGH();
		}
		else
			IIC_SDA_PIN_LOW();
		da = da << 1;
		delay_10us(1);
		IIC_SCL_PIN_HIGH();
		delay_10us(1);
		IIC_SCL_PIN_LOW();
	}
}

/**********************************************
// IIC Read byte
**********************************************/
void Read_IIC_Byte(unsigned char *IIC_Byte)
{
	unsigned char i;
	unsigned char m, da;
	*IIC_Byte = 0;
	IIC_SDA_PIN_INPUT();
	delay_10us(1);
	for (i = 0; i < 8; i++)
	{
		IIC_SCL_PIN_HIGH();
		delay_10us(1);
		if(IIC_SDA_PIN_READ())
		{
			*IIC_Byte |= 0x80>>i;
		}
		IIC_SCL_PIN_LOW();
		delay_10us(1);
	}
	IIC_SDA_PIN_HIGH();
	IIC_SDA_PIN_OUTPUT();
}

/**
 * @brief 
 * IIC单数据写入 
 * 返回值 1:PASS、0:ERROR
 */
unsigned char IIC_Driver_Write_Single_Data(unsigned char device_id, unsigned char reg_adr, unsigned char data)
{
	IIC_Start();
	Write_IIC_Byte(device_id << 1 & 0xFE); 		// 设备ID
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
unsigned char IIC_Driver_Write_Multi_Data(unsigned char device_id, unsigned char reg_adr, unsigned char *pdata, unsigned char data_len)
{
	IIC_Start();
	Write_IIC_Byte(device_id << 1 & 0xFE); 	// 设备ID bit0: 0写 1读
	IIC_Wait_Ack();
	Write_IIC_Byte(reg_adr); 			// 设备寄存器地址
	IIC_Wait_Ack();
	Write_IIC_Byte(*pdata);				// 寄存器写入数据
	IIC_Wait_Ack();
	IIC_Stop();
	return 0;
}
	
/**
 * @brief 
 * IIC单数据读取
 * 返回值 1:PASS、0:ERROR
 */
unsigned char IIC_Driver_Read_Single_Data(unsigned char device_id, unsigned char reg_adr, unsigned char *pdata)
{
	IIC_Start();
	Write_IIC_Byte(device_id << 1 & 0xFE);	// 设备ID bit0: 0写 1读
	IIC_Wait_Ack();
	Write_IIC_Byte(reg_adr); 			// 设备寄存器地址
	IIC_Wait_Ack();
	IIC_Start();
	Write_IIC_Byte(device_id << 1 | 0x01);	// 设备ID bit0: 0写 1读
	IIC_Wait_Ack();
	Read_IIC_Byte(pdata);				// 寄存器写入数据
	IIC_Send_Ack();
	IIC_Stop();
	return 0;
}

/**
 * @brief 
 * IIC多数据读取
 * 返回值 1:PASS、0:ERROR
 */
unsigned char IIC_Driver_Read_Multi_Data(unsigned char device_id, unsigned char reg_adr, unsigned char *data, unsigned char data_len)
{
	unsigned char i=0;
	IIC_Start();
	Write_IIC_Byte(device_id << 1 & 0xFE);	// 设备ID bit0: 0写 1读
	IIC_Wait_Ack();
	Write_IIC_Byte(reg_adr); 			// 设备寄存器地址
	IIC_Wait_Ack();
	IIC_Start();
	Write_IIC_Byte(device_id << 1 | 0x01);	// 设备ID bit0: 0写 1读
	IIC_Wait_Ack();
	for(; i < data_len; i++)
	{
		Read_IIC_Byte(data++);				// 寄存器写入数据
		if(i== data_len -1)
		{
			IIC_Send_NAck();
		}
		else
		{
			IIC_Send_Ack();
		}
	}
	IIC_Stop();
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