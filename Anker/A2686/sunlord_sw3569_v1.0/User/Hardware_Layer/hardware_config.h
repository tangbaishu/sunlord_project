#ifndef _HARDWARE_CONFIG_H_
#define _HARDWARE_CONFIG_H_

#include "zr_gpio.h"
#include "zr_adc.h"

/******		I2C模块 	*****/
// 硬件接口	主机I2C
#define I2C_M_SCK_GPIO_PIN			GPIO1			//	GPIO SCK IO口
#define I2C_M_SCK_AF_MODE			FUNCTION_SEL7	//	GPIO SCK复用模式

#define I2C_M_SDA_GPIO_PIN			GPIO2			//	GPIO SDA IO口
#define I2C_M_SDA_AF_MODE			FUNCTION_SEL1	//	GPIO SDA复用模式

// 硬件接口	从机I2C
#define I2C_S_SCK_GPIO_PIN			GPIO1			//	GPIO SCK IO口
#define I2C_S_SCK_AF_MODE			FUNCTION_SEL7	//	GPIO SCK复用模式

#define I2C_S_SDA_GPIO_PIN			GPIO2			//	GPIO SDA IO口
#define I2C_S_SDA_AF_MODE			FUNCTION_SEL1	//	GPIO SDA复用模式

#define IIC_MAX_WDATA_LEN			20
#define IIC_MAX_RDATA_LEN			20
#define IIC_BUFFER_AREA_LEN			20
#define IIC_OVERTIME_THRESHOLD		200				// I2C超时检测阈值

typedef enum
{
	INIT,							// 初始化值
	HOST_ROLE,						// 主机
	SLAVE_ROLE_1,					// 从机
}Device_Type_e;

typedef enum
{
	SELF_NUMBER 	= 0,			// 默认自身编号为 0，对应I2C数据块的第0块区域
	SLAVE_NUMBER_1 	= 1,			// 1号从机
}I2C_Device_Num_e;					// I2C设备编号
#define SLAVE_SUM					1				//	从机芯片总数
#define SLAVE_ROLE_1_CHIP			SW3566			//	1号从机芯片
typedef enum
{
	SLAVE_1_DEV_ADDR = 0x3C,
}Slave_Device_Addr_e;

/******		NTC模块 	*****/
#define NTC_ADC_CHANNEL				ADC_CHL_2

/******		GPIO口	 	*****/
#define GPIO_INT_PIN				GPIO2			// 	主机中断唤醒引脚
#define GPIO_ONLINE					GPIO2			//	从机端口状态跟随引脚

/******		角色识别接口 *****/
#define LEVEL_IDENTIFY				0				//	高低电平识别
#define RES_IDENTIFY				1				//	外挂阻值识别
#define IDENTIFY_WAY				LEVEL_IDENTIFY	//	角色识别方式

#if IDENTIFY_WAY == LEVEL_IDENTIFY
	#define IDENTIFY_GPIO_PIN		GPIO3			//	电平识别IO口
	#define HOST_ROLE_LEVEL			true			//	高电平
	#define SLAVE_ROLE_LEVEL		false			//	低电平
#else
	#define IDENTIFY_ADC_CHANNEL	ADC3_SIG_SRC_VD	//	ADC识别通道
	#define HOST_ROLE_RES			10				//	主机识别电阻值，单位：KΩ
	#define SLAVE_ROLE_LEVEL		0				//	从机识别电阻值，单位：KΩ
#endif

#endif

