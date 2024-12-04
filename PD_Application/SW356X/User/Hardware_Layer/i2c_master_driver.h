#ifndef _I2C_MASTER_DRIVER_H_
#define _I2C_MASTER_DRIVER_H_

#ifndef uint8_t
#ifndef _DECA_UINT8_T_
#define _DECA_UINT8_T_
typedef unsigned char uint8_t;
#endif // !_DECA_UINT8_T_
#endif // ! uint8_t

#define IIC_MAX_WDATA_LEN			10
#define IIC_MAX_RDATA_LEN			10

#define I2C_M_SCK_GPIO_PIN		GPIO1			//	GPIO SCK IO口
#define I2C_M_SCK_AF_MODE		FUNCTION_SEL7	//	GPIO SCK复用模式

#define I2C_M_SDA_GPIO_PIN		GPIO2			//	GPIO SDA IO口
#define I2C_M_SDA_AF_MODE		FUNCTION_SEL1	//	GPIO SDA复用模式

typedef enum
{
	IDLE,						// 空闲 	发送I2C Stop Bit 时置位 / 初始化
	BUSY,						// 忙碌 	发送I2C Statrt Bit 时置位
	FINISH,						// 完成 	发送/接收 I2C Data 完成时置位
	WAIT_FINISH_OVER_TIME,		// 等待应答超时
	BUS_LOST,					// 总线丢失	检测到总线丢失，触发中断后置位
	SCL_TIMEOUT,				// 时钟超时
	BUSY_TIMEOUT,				// 忙碌超时 超时时间：BUSY_TIMEOUT_TIME
	DATA_OVERFLOW				// 读取/写入数据溢出
}I2C_State_e;

typedef struct
{
	uint8_t Reg_Addr;
	uint8_t P_Wdata[IIC_MAX_WDATA_LEN];
	uint8_t Wdata_Len;
	uint8_t P_Rdata[IIC_MAX_RDATA_LEN];
	uint8_t Rdata_Len;
}Transfer_Info_t;

typedef struct
{
	Transfer_Info_t 		Transfer_Data;
	volatile I2C_State_e	Runing_State;
}I2C_M_Driver_Info_t;
extern I2C_M_Driver_Info_t I2C_M_Driver;

// void I2C_Master_Driver_Init(uint8_t device_id);

// /**
//  * @brief  * @brief I2C主机写入 N数据
//  * 
//  * @param wirte_data
// 	struct Transfer_Info_t
// 	{
// 		uint8_t Reg_Addr;
// 		uint8_t *P_Wdata[IIC_MAX_WDATA_LEN];
// 		uint8_t Wdata_Len;
// 	}wirte_data;
//  *
//  * @return I2C_State_e 
//  */
// I2C_State_e I2C_Master_Write_NByte(Transfer_Info_t *wirte_data);


// I2C_State_e I2C_Master_Read_NByte(Transfer_Info_t *read_data);

#endif // !_I2C_MASTER_DRIVER_H_
