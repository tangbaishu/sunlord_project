#include "busi_port_dispose.h"
#include "sunlord_api.h"
#include "port_data_library.h"
#include "func_hardware_api.h"

Port_Out_Info_t Port_Out_Info[SYS_PORT_MAX_NUMBER];
extern struct SW_API_Func_t SW3566_Func;

static uint8_t Port_Insert_Check(void);

void busi_Port_Dispose(void)
{
	static uint8_t process=0;
	if(process == 0) // 上电启动，由主机下达所有端口的输出功率
	{

	}
	switch(process)
	{
		case 0: if( Port_Insert_Check() == 0 )	
				{
					process++;
				}
				break;
		case 1: 
		defaule: break;
	}
}

uint8_t Port_Insert_Check(void)
{
	SW3566_Func.SW_Get_Port_C_State(CHIP_TYPE_C_PORT_NUM, (uint8_t *)&(Port_Out_Info[0].Port_Link_State));
	if(HOST_ROLE == Hardware_Driver_Data.Device_Type)	// 主机设备逐个获取从机端口摘要信息
	{

	}
}


