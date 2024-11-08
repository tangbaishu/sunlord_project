#ifndef _IIC_API_H_
#define _IIC_API_H_

typedef unsigned char uint8;

typedef struct
{
	void (*IIC_Driver_init) (void);																	// IIC驱动初始化
	uint8 (*IIC_Write_Single_Data) (uint8 device_id, uint8 reg_adr, uint8 data);					// IIC单数据写入 返回值 1:PASS、0:ERROR
	uint8 (*IIC_Write_Multi_Data) (uint8 device_id, uint8 reg_adr, uint8 *data, uint8 data_len);	// IIC多数据写入 返回值 1:PASS、0:ERROR
	
	uint8 (*IIC_Read_Single_Data) (uint8 device_id, uint8 reg_adr, uint8 *data);					// IIC单数据读取 返回值 1:PASS、0:ERROR
	uint8 (*IIC_Read_Multi_Data) (uint8 device_id, uint8 reg_adr, uint8 *data, uint8 data_len);		// IIC单数据读取 返回值 1:PASS、0:ERROR
}IIC_API_t;



#endif
