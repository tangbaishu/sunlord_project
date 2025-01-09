#include "busi_i2c_policy.h"
#include "func_hardware_api.h"
#include "device_driver.h"
#include "busi_share_memory.h"
#include "func_i2c_api.h"
#include "api.h"

#if defined(DEBUG_PRINTF_OPEN)
    #include "stdio.h"
    #define LOG "busi_i_b: "
#endif


typedef enum
{
    NONE=0,                     //  空操作，用于变量初始化
    PROCESS_READ,               //  读操作
    PROCESS_WRITE,              //  写操作
    // MULTI_READ,                 //  连读（I2C总线单次通讯，对多个地址进行数据读取）
    // SINGLE_READ,                //  单读（I2C总线单次通讯，对单个地址进行数据读取）
    // MULTI_WRITE,                //  连写（I2C总线单次通讯，对多个地址进行数据写入）
    // SINGLE_WRITE,               //  单写（I2C总线单次通讯，对单个地址进行数据写入）
}Process_Set_e;                 //  操作指令集

typedef struct
{
    Process_Set_e       Process;        //  具体操作
    I2C_Device_Num_e    Slave_Num;      //  操作的从机编号
    uint8_t             Reg_Addr;       //  操作的寄存器地址（连读/连写的首地址、单读/单写的地址）
    uint8_t             Reg_Data_Len;   // 涉及的寄存器数据长度
}Prev_Process_Info_t;                   //  Previous_Process_Info_t 上一次操作/当前已完成的操作

typedef struct
{
    Prev_Process_Info_t     Prev_Process_Info;
    I2C_SW3566_Reg_Data_t   *Slave_1_Reg_Data;
}Busi_I2C_Data_t;
static Busi_I2C_Data_t  Busi_I2C_Data={
    .Slave_1_Reg_Data = &I2C_SW3566_Reg_Data[SLAVE_NUMBER_1],
};

static I2C_Driver_API_t   *I2C_Driver_API = &Hardware_Driver_API.I2C_Driver_API;

Busi_I2C_Policy_Data_t	Busi_I2C_Policy_Data;

static void Get_Buffer_Data(uint8_t *p_data, volatile uint8_t copy_len);
static void I2C_Read_Once_Data(uint8_t reg);
static void I2C_Read_Multi_Data(uint8_t reg, uint8_t rdata_len);
static void I2C_Write_Once_Data(uint8_t reg, uint8_t wdata);
static void I2C_Write_Multi_Data(uint8_t reg, uint8_t *p_wdata, uint8_t wdata_len);

Busi_I2C_Process_e Busi_I2C_Policy(Busi_I2C_Policy_Data_t *policy_data)
{
    if(Busi_I2C_Traverse_Reg() == PROCESS_READ_RGE_FINSIH) // 主机遍历从机运行数据
    {
        MY_PRINTF("SW3566 Reg Data:\r\n");
        for(uint8_t num=0; num < sizeof(I2C_SW3566_Reg_Data->Memory_Block); num++)
        {
            printf("[%d] = %#x\r\n", num, Busi_I2C_Data.Slave_1_Reg_Data->Memory_Block[num]);
        }
        return PROCESS_READ_RGE_FINSIH;
    }
    if(policy_data->Rebroadcast_PDO_En)
    {
        I2C_Driver_API->P_Func_Write_Data(I2C_Driver_API, REBROADCAST_PDO, &policy_data->Rebroadcast_PDO_En, 1);
        policy_data->Rebroadcast_PDO_En = 0;
    }
    if(policy_data->Slave_Output_Power)
    {
        I2C_Driver_API->P_Func_Write_Data(I2C_Driver_API, SET_OUTPUT_POWER, &policy_data->Slave_Output_Power, 1);
        policy_data->Slave_Output_Power = 0;
    }
}


/**
 * @brief 遍历从机寄存器
 * 
 * @return Busi_I2C_Process_e 
 */
Busi_I2C_Process_e Busi_I2C_Traverse_Reg(void)
{
    static uint8_t  read_reg_add = PORT_STATE;
    static uint32_t busi_i2c_time=0;
    I2C_Driver_API->P_Func_Async_Thread(&I2C_Driver_API->I2C_Driver_Data);
    if(My_GetSystemTimePass(busi_i2c_time) < 5)
    {
        return PROCESS_READ_REG;
    }
    busi_i2c_time = My_GetSystemTimeMark();
    if( (Busi_I2C_Data.Prev_Process_Info.Process == PROCESS_READ) &&\
        (I2C_Driver_API->I2C_Driver_Data.Driver_State.Reg_Read_Finish == true) )  // 若 I2C总线刚刚完成读操作
    {
        Get_Buffer_Data(&I2C_SW3566_Reg_Data[Busi_I2C_Data.Prev_Process_Info.Slave_Num].Memory_Block[Busi_I2C_Data.Prev_Process_Info.Reg_Addr],\
                Busi_I2C_Data.Prev_Process_Info.Reg_Data_Len);
        read_reg_add++;
        if(read_reg_add > CMD_BYTE)
        {
            read_reg_add = PORT_STATE;
            return PROCESS_READ_RGE_FINSIH;
        }
    }
    I2C_Read_Once_Data(read_reg_add);
    return PROCESS_READ_REG;
}


/**
 * @brief I2C读单个数据
 * @param reg
 */
static void I2C_Read_Once_Data(uint8_t reg)
{
    I2C_Driver_API->P_Func_Read_Data(&I2C_Driver_API->I2C_Driver_Data, reg, 1);
    Busi_I2C_Data.Prev_Process_Info.Process = PROCESS_READ;
    Busi_I2C_Data.Prev_Process_Info.Reg_Addr = reg;
    Busi_I2C_Data.Prev_Process_Info.Reg_Data_Len = 1;
    Busi_I2C_Data.Prev_Process_Info.Slave_Num = SLAVE_NUMBER_1;
}

/**
 * @brief I2C读多个数据
 * @param reg
 * @param rdata_len 
 */
static void I2C_Read_Multi_Data(uint8_t reg, uint8_t rdata_len)
{
    I2C_Driver_API->P_Func_Read_Data(&I2C_Driver_API->I2C_Driver_Data, reg, rdata_len);
    Busi_I2C_Data.Prev_Process_Info.Process = PROCESS_READ;
    Busi_I2C_Data.Prev_Process_Info.Reg_Addr = reg;
    Busi_I2C_Data.Prev_Process_Info.Reg_Data_Len = rdata_len;
    Busi_I2C_Data.Prev_Process_Info.Slave_Num = SLAVE_NUMBER_1;
}

/**
 * @brief I2C写单个数据
 * @param reg 
 * @param wdata
 */
static void I2C_Write_Once_Data(uint8_t reg, uint8_t wdata)
{
    I2C_Driver_API->P_Func_Write_Data(&I2C_Driver_API->I2C_Driver_Data, reg, &wdata, 1);
    Busi_I2C_Data.Prev_Process_Info.Process = PROCESS_WRITE;
    Busi_I2C_Data.Prev_Process_Info.Reg_Addr = reg;
    Busi_I2C_Data.Prev_Process_Info.Reg_Data_Len = 1;
    Busi_I2C_Data.Prev_Process_Info.Slave_Num = SLAVE_NUMBER_1;
}

/**
 * @brief I2C写多个数据
 * 
 * @param reg 
 * @param p_wdata 
 * @param wdata_len 
 */
static void I2C_Write_Multi_Data(uint8_t reg, uint8_t *p_wdata, uint8_t wdata_len)
{
    I2C_Driver_API->P_Func_Write_Data(&I2C_Driver_API->I2C_Driver_Data, reg, p_wdata, wdata_len);
    Busi_I2C_Data.Prev_Process_Info.Process = PROCESS_WRITE;
    Busi_I2C_Data.Prev_Process_Info.Reg_Addr = reg;
    Busi_I2C_Data.Prev_Process_Info.Reg_Data_Len = wdata_len;
    Busi_I2C_Data.Prev_Process_Info.Slave_Num = SLAVE_NUMBER_1;
}

static void Get_Buffer_Data(uint8_t *p_data, volatile uint8_t copy_len)
{
    while(copy_len--)
    {
        p_data[copy_len] = I2C_Driver_API->I2C_Driver_Data.I2C_Read_Buffer.Read_Buffer[copy_len];
    }
    I2C_Driver_API->I2C_Driver_Data.Driver_State.Allow_Read_Write = true;
    I2C_Driver_API->I2C_Driver_Data.Driver_State.Reg_Read_Finish = false;
}

void Func_I2C_Slave_Int_Dispose(void)
{
	volatile uint8_t tmpRegAddr = 0;
	volatile uint8_t tmpBuf[8];
	volatile uint8_t tmpLen;
	static uint8_t finishEnFlag = 0;
	
	Low_Power_Not_Sleep();
	
	if (I2c_Slave_Get_Pending(I2C_S_TX_DATA_PENGING))   // 主机读数据
	{
        tmpRegAddr = I2c_Slave_Get_Register_Address();
        if(CMD_BYTE == tmpRegAddr)
        {
            for(tmpLen=1; tmpLen < 7; tmpLen++)
            {
                ZR_I2C->S_TX_DATA = I2C_SW3566_Reg_Data[0].Memory_Block[tmpLen];
            }
        }
        else
        {
            ZR_I2C->S_TX_DATA = I2C_SW3566_Reg_Data[0].Memory_Block[tmpRegAddr];
        }
        I2c_Slave_RX_FIFO_Reset();
        I2c_Slave_TX_FIFO_Reset();
		I2c_Slave_Clear_Pending(I2C_S_TX_DATA_PENGING);
	}
	
	if(I2c_Slave_Get_Pending(I2C_S_RX_DATA_PENGING))    // 主机写数据
	{
		finishEnFlag = 1;
		I2c_Slave_Clear_Pending(I2C_S_FINISH_PENGING);
		I2c_Slave_Int_Enable(I2C_S_FINISH_INT_EN);
		I2c_Slave_Clear_Pending(I2C_S_RX_DATA_PENGING);
	}	

	if(I2c_Slave_Get_Pending(I2C_S_FINISH_PENGING))
	{
        if(finishEnFlag)
        {
            tmpRegAddr = I2c_Slave_Get_Register_Address();
            tmpLen = I2c_Slave_Get_RX_Byte_Length();
            if(1 == tmpLen)
            {
                if(SET_OUTPUT_POWER ==  tmpRegAddr)
                {
                    I2c_Slave_Get_RX_Data(&I2C_SW3566_Reg_Data[0].Memory_Block[SET_OUTPUT_POWER], 1);
                }
                else if(REBROADCAST_PDO == tmpRegAddr)
                {
                    I2c_Slave_Get_RX_Data(&I2C_SW3566_Reg_Data[0].Memory_Block[REBROADCAST_PDO], 1);
                }
            }
            finishEnFlag = 0;
        }
		I2c_Slave_Int_Disable(I2C_S_FINISH_INT_EN);
		I2c_Slave_Clear_Pending(I2C_S_FINISH_PENGING);
	}
}
