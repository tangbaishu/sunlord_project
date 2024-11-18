#include "oled_driver.h"
#include "debug_driver.h"
#include "oled.h"
#include "delay.h"
#include "gpio_ctl.h"

IIC_Interface_t IIC_Interface = \
{
	IIC_Driver_Init,	\
	IIC_Driver_Write,	\
	IIC_Driver_Read		\
};
IIC_Transmit_t IIC_Transmit={OLED_DEVICE_ADDR,};

void Pixel_Display(void);

#ifdef FREERTOS_EN
	TaskHandle_t OLED_Driver_Handle;
#endif

void OLED_Driver_Task(void *param)
{
	u8 t;
	uint8_t x_offset=0,y_offset=0;
	uint8_t x_axis=0,y_axis=0;
	uint32_t hex_value=0x00;
	OLED_Init();
	OLED_Clear(); 

	// Pixel_Display();

	while(1)
	{
		// OLED_Set_Pos(x_axis,y_axis);
		// OLED_WR_Byte(hex_value,OLED_DATA);
		// DMA_Sprintf("x_axis=%d\ty_axis=%d\thex_value=0x%lx\r\n",x_axis, y_axis, hex_value);
		// x_axis++;
		// hex_value = hex_value<<1 | 0x01;
		// if(hex_value == 0x7F)
		// {
		// 	hex_value = 0X00;
		// }
		// if(x_axis >= 128)
		// {
		// 	x_axis = 0;
		// 	y_axis ++;
		// 	hex_value = 0x01;
		// }
		// #ifdef FREERTOS_EN
		// TickType_t current_tick_value = xTaskGetTickCount();
		// 	vTaskDelayUntil(&current_tick_value,pdMS_TO_TICKS(2000));
		// #endif //FREETROS_EN
		// LED0_OVERTURN();
		
		OLED_ShowCHinese(0+x_offset,y_offset,0);//
		OLED_ShowCHinese(18+x_offset,y_offset,1);//
		OLED_ShowCHinese(36+x_offset,y_offset,2);//
		OLED_ShowCHinese(54+x_offset,y_offset,3);//
		OLED_ShowCHinese(72+x_offset,y_offset,4);//
		OLED_ShowCHinese(90+x_offset,y_offset,5);//


		OLED_ShowCHinese(0+x_offset,y_offset+3,7);//中
		OLED_ShowCHinese(18+x_offset,y_offset+3,8);//景
		OLED_ShowCHinese(36+x_offset,y_offset+3,9);//园
		OLED_ShowCHinese(54+x_offset,y_offset+3,10);//电
		OLED_ShowCHinese(72+x_offset,y_offset+3,11);//子
		OLED_ShowCHinese(90+x_offset,y_offset+3,12);//科
		x_offset++;
		if(x_offset >= 4)
		{
			x_offset = 2;
		}
		y_offset++;
		if(y_offset >= 3)
		{
			y_offset = 1;
		}
		#ifdef FREERTOS_EN
		TickType_t current_tick_value = xTaskGetTickCount();
			vTaskDelayUntil(&current_tick_value,pdMS_TO_TICKS(2000));
		#endif //FREETROS_EN
		LED0_OVERTURN();
		OLED_Clear(); 

	}
}

//初始化SSD1306					    
void OLED_Init(void)
{ 	
	#ifdef FREERTOS_EN
		IIC_Interface.IIC_Semaphore_Binary = NULL;
		vSemaphoreCreateBinary(IIC_Interface.IIC_Semaphore_Binary);
		if(IIC_Interface.IIC_Semaphore_Binary == NULL)
		{
			DMA_Sprintf("Error!!!\t OLED_Init Fail Return\r\n");
			return;
		}
	#endif //FREETROS_EN

	IIC_Interface.IIC_Init();
	delay_ms(800);

	OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED_WR_Byte(0x02,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128   
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
} 


void OLED_IIC_Byte(uint8_t cmd, uint8_t data)
{
	#ifdef FREERTOS_EN
		if(IIC_Interface.IIC_Semaphore_Binary != NULL)
		{
			xSemaphoreTake(IIC_Interface.IIC_Semaphore_Binary, portMAX_DELAY);
		}
	#endif //FREETROS_EN

	IIC_Transmit.Dev_Addr = OLED_DEVICE_ADDR;
	if(cmd == OLED_DATA)
	{
		IIC_Transmit.Reg_Addr = OLED_WRITE_DATA_REG;
	}
	else if(cmd == OLED_CMD)
	{
		IIC_Transmit.Reg_Addr = OLED_WRITE_CMD_REG;
	}
	IIC_Transmit.P_Data[0] = data;
	IIC_Transmit.Data_Len = 1;
	IIC_Interface.IIC_Write(IIC_Transmit);

	#ifdef FREERTOS_EN
		if(IIC_Interface.IIC_Semaphore_Binary != NULL)
		{
			xSemaphoreGive(IIC_Interface.IIC_Semaphore_Binary);
		}
	#endif //FREETROS_EN
}

/**
 * @brief 
 * 
 */
void Pixel_Display(void)
{
	uint8_t column_value=0;
	uint32_t x_offset=0,y_offset=0;
	while(1)
	{
		OLED_Set_Pos(x_offset,y_offset);
		column_value = column_value << 1 | 0x01;
		x_offset++;
		OLED_WR_Byte(column_value,OLED_DATA);
		if(column_value == 0xff)
		{
			column_value = 0;
			DMA_Sprintf("Pixel_Size = %ld * %ld\r\n", x_offset, y_offset*8);
			HAL_Delay(500);
			y_offset++;
		}
		DMA_Sprintf("x_offset = %ld, y_offset = %ld, column_value = 0x%x\r\n", x_offset, y_offset, column_value);
		#ifdef FREERTOS_EN
		TickType_t current_tick_value = xTaskGetTickCount();
			vTaskDelayUntil(&current_tick_value,pdMS_TO_TICKS(200));
		#endif //FREETROS_EN
		LED0_OVERTURN();
	}

}