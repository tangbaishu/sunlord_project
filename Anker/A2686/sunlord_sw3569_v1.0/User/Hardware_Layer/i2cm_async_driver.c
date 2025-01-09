#include "i2cm_async_driver.h"
#include "hardware_config.h"
#include "base_timer_driver.h"
#include "device_driver.h"
#include "i2cm_async.h"
#include "zr_gpio.h"
#include "zr_rcc.h"
#include "zr_systick.h"
#include "zr_nvic.h"
#if defined(DEBUG_PRINTF_OPEN)
    #include "stdio.h"
    #define LOG "I2C_Async_Driver: "
#endif

static void I2CM_Async_GPIO_Init(void);
static void On_Begin_Event_Func(i2cs_device_t* device);
static void On_End_Event_Func(i2cs_device_t* device);

void I2CM_Async_Driver_Example(void)
{
	static i2cs_device_t I2CM_Deivce_Data;
	static uint8_t I2CM_Device_Buff[20]={0};
	static uint8_t buffer[]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09};
	static bool i2c_write_running_flag=true;
	uint8_t return_value;

	I2CM_Async_Hardware_Init(50);
	I2cm_Async_Attach(&I2CM_Deivce_Data, On_Begin_Event_Func, On_End_Event_Func, 0x3c, I2CM_Device_Buff, sizeof(I2CM_Device_Buff));
	Systick_Delay_Ms(200);	// 初始化后需要短暂延时后使用
	while(1)
	{
		return_value = false;
		if(i2c_write_running_flag)
		{
			if(I2cm_Async_Write(&I2CM_Deivce_Data, 0x01, buffer, sizeof(buffer)))
			{	
				
				i2c_write_running_flag = false;
			}
		}
		else
		{
			return_value = 	I2cm_Async_Get_And_Clear_Finished(&I2CM_Deivce_Data);
			if( return_value == I2CM_RESULT_SUCCESS || return_value == I2CM_RESULT_FAILURE )
			{
				i2c_write_running_flag = true;
			}
		}
		I2cm_Async_Run();
	}
	
}

/**
 * @brief init i2cm async
 * @param[in] i2c_clock the frequency of I2c master, unit : 1KHz  e.g. 100 -> 100KHz
 */
void I2CM_Async_Hardware_Init(uint8_t i2c_clock)
{
	Nvic_Irq_Enable_All();
	I2CM_Async_GPIO_Init();
	Rcc_Module_Clk_Enable(GPIO_ID | I2C_ID);
	I2cm_Async_Init(i2c_clock);
}

/**
 * @brief 异步I2C变量数据初始化
 * 在硬件初始化之后，再进一步对异步I2C变量进行初始化
 * @param ram_data 		
 * @param device_addr 			设备寄存器地址
 * @param buffer_area 			写入/读取 数据缓存区 映射地址
 * @param buffer_len 			写入/读取 数据缓存区 长度
 * @param overtime_threshold 	超时阈值
 */
void I2CM_Async_Driver_Attach_Slave(Func_I2CM_Async_Data_t *ram_data, uint8_t device_addr, uint8_t *buffer_area, uint8_t buffer_len, uint32_t overtime_threshold)
{
	ram_data->I2CM_Async_RAM.OverTime_Threshold = overtime_threshold;
	I2cm_Async_Attach(&ram_data->I2CS_Device, ram_data->P_On_Begin_Event_Fun, ram_data->P_On_End_Event_Fun, device_addr, buffer_area, buffer_len);
}

/**
 * @brief 主机通过 异步I2c 写从机寄存器数据
 * 
 * @param ram_data 
 * @param reg 
 * @param p_wdata 
 * @param wdata_len 注数据写入长度不可超过 ram_data->.buffer缓存区长度
 * @return true 	正常
 * @return false 	传输超时
 */
bool I2CM_Async_Driver_Write(Func_I2CM_Async_Data_t *ram_data, uint8_t reg, uint8_t *p_wdata, uint8_t wdata_len)
{
	// 若缓冲区数据锁存解锁(已处理上次传输的数据) 且 当前数据传输完成/未开始传输
	I2cm_Async_Write(&ram_data->I2CS_Device, reg, p_wdata, wdata_len);
	ram_data->I2CM_Async_RAM.Transfer_Time_Slice = My_GetSystemTimeMark();
	return true;
}

/**
 * @brief 主机通过 异步I2c 读从机寄存器数据
 * 读取数据存放置 ram_data.I2CS_Device.buffer
 * @param ram_data 
 * @param reg 
 * @param rdata_len 
 */
bool Func_I2CM_Async_Driver_Read(Func_I2CM_Async_Data_t *ram_data, uint8_t reg, uint8_t rdata_len)
{
	I2cm_Async_Read(&ram_data->I2CS_Device, reg, rdata_len);
	ram_data->I2CM_Async_RAM.Transfer_Time_Slice = My_GetSystemTimeMark();
	return true;
}

static void I2CM_Async_GPIO_Init(void)
{
	gpio_config_t gpio_config;
	Rcc_Module_Clk_Enable(GPIO_ID);
	// gpio_config.direction = GPIO_DIRECTION_OUTPUT;
	Gpio_Function_Select(I2C_M_SCK_GPIO_PIN,I2C_M_SCK_AF_MODE);
	Gpio_Function_Select(I2C_M_SDA_GPIO_PIN,I2C_M_SDA_AF_MODE);
	Gpio_Mos_Opendrain_Cfg(I2C_M_SCK_GPIO_PIN, PMOS_OPDRAIN_DIS, NMOS_OPDRAIN_EN);
    Gpio_Mos_Opendrain_Cfg(I2C_M_SDA_GPIO_PIN, PMOS_OPDRAIN_DIS, NMOS_OPDRAIN_EN);
    Gpio_Pulldown_Pullup_Cfg(I2C_M_SCK_GPIO_PIN, PULLDOWN_DIS, PULLUP_EN);
    Gpio_Pulldown_Pullup_Cfg(I2C_M_SDA_GPIO_PIN, PULLDOWN_DIS, PULLUP_EN);
}

static void On_Begin_Event_Func(i2cs_device_t* device)
{
	(void)device;
	Gpio_Function_Select(I2C_M_SCK_GPIO_PIN,I2C_M_SCK_AF_MODE);
	Gpio_Function_Select(I2C_M_SDA_GPIO_PIN,I2C_M_SDA_AF_MODE);
	Gpio_Mos_Opendrain_Cfg(I2C_M_SCK_GPIO_PIN, PMOS_OPDRAIN_DIS, NMOS_OPDRAIN_EN);
    Gpio_Mos_Opendrain_Cfg(I2C_M_SDA_GPIO_PIN, PMOS_OPDRAIN_DIS, NMOS_OPDRAIN_EN);
    Gpio_Pulldown_Pullup_Cfg(I2C_M_SCK_GPIO_PIN, PULLDOWN_DIS, PULLUP_EN);
    Gpio_Pulldown_Pullup_Cfg(I2C_M_SDA_GPIO_PIN, PULLDOWN_DIS, PULLUP_EN);
	// MY_PRINTF(LOG"On_Begin_Event_Func\r\n");
	// printf(LOG"On_Begin_Event_Func\r\n");
}

static void On_End_Event_Func(i2cs_device_t* device)
{
	// MY_PRINTF(LOG"On_End_Event_Func\r\n");
}

/**
 * @brief 异步I2C数据传输超时检测
 * 
 * @param ram_data 
 * @param over_threshold 超时阈值	单位：@SystemTimeCounter对应的时间单位
 * @return true 	超时
 * @return false 	未超时
 */
bool I2CM_Async_Driver_Overtime(I2CM_Async_RAM_t ram_data, uint32_t over_threshold)
{
	if(My_GetSystemTimePass(ram_data.Transfer_Time_Slice) > over_threshold)
	{
		return true;
	}
	return false;
}