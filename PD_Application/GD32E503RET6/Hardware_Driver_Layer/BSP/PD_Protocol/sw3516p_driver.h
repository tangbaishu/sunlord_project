#ifndef _SW3516P_DRIVER_H_
#define _SW3516P_DRIVER_H_

#include "IIC_Driver/iic_api.h"

#ifndef uint8
#ifndef _DECA_UINT8_
#define _DECA_UINT8_
typedef unsigned char uint8;
#endif
#endif

#ifndef uint16
#ifndef _DECA_UINT16_
#define _DECA_UINT16_
typedef unsigned short uint16;
#endif
#endif

#define READ_DEVICE_VERSION


//状态指示寄存器（只读）
#define FAST_CHARGE_INDICATION		0x06	// 快充指示
#define SYSTEM_STATE_0				0x07	// 系统状态 0
#define SYSTEM_STATE_1				0x08	// 系统状态 1
#define PORT_1_CURRENT_LIMITING		0x0C	// 端口1限流值指示 	    ctrl_icc1[6:0] unit:50mA Icc1 = 1000mA + ctrl_icc1 *unit;
#define PORT_2_CURRENT_LIMITING		0x0D	// 端口2限流值指示 	    ctrl_icc2[6:0] unit:50mA Icc2 = 1000mA + ctrl_icc2 *unit;

//可读写状态寄存器
#define IIC_ENABLE_CONTROL			0x12	// IIC使能控制		    如果要操作寄存器 REG0xA0~D1,需按顺序写入：0x20、0x40、0x80
#define ADC_VIN_ENABLE				0x13	/* ADC Vin	 使能	    bit6: PPS 和 SCP 协议是否上报 NTC 温度 { 0: 上报 NTC 温度 1: 上报 45° }
															        bit1: 输入 Vin 的 ADC 工作使能,只有在使能时,Vin 的数据才能读出 { 0：不使能 1：使能}*/
#define PWR_COMPEL_OPERATION_ENABLE	0x15	// PWR强制操作使能      如果要操作寄存器 REG0x16,需按顺序写入：0x20、0x40、0x80
#define PWR_COMPEL_OPERATION_1		0x16	/* PWR强制操作0         bit5：强制打开2口通路管 { 0：无影响 1：强制打开2口通路管}
																    bit4：强制关闭2口通路管 { 0：无影响 1：强制关闭2口通路管 对于AC模式来说,2口为A口;对于AA模式来说,2口为A2口}
																    bit3：强制打开1口通路管 { 0：无影响 1：强制打开1口通路管}
																    bit2：强制关闭1口通路管	{ 0：无影响 1：强制关闭1口通路管 对于AC或C模式来说,1口为C口;对于AA模式来说,1口为A1口}
																    bit1：强制打开Buck操作	{ 0：无影响 1：强制打开Buck}
																    bit0：强制关闭Buck操作	{ 0：无影响 1：强制关闭Buck}
                                                                    */
#define ADC_VIN_DATA				0x30	// ADC Vin	 数据	    Vin电压的高8bit 160mV/bit;(若取12bit时分辨率为 10mV/bit,参见 REG0x3A) 注意默认Vin的数据不能读出, 需要写REG0x13[1]为1.
#define ADC_VOUT_DATA				0x31	// ADC Vout	 数据	    输出电压的高8bit 96mV/bit;(若取12bit时分辨率为 6mV/bit,参见 REG0x3A)
#define ADC_IOUT_1_DATA				0x33	// ADC Iout1 数据	    1口输出电流的高8bit 40mA/bit;(若取12bit时分辨率为 2.5mA/bit,参见 REG0x3A)
#define ADC_IOUT_2_DATA				0x34	// ADC Iout2 数据	    2口输出电流的高8bit 40mA/bit;(若取12bit时分辨率为 2.5mA/bit,参见 REG0x3A)
#define ADC_NTC_VOLTAGE_DATA		0x37	// ADC NTC 电压数据	    NTC电阻上电压的高8bit 8mV/bit;(若取12bit时分辨率为 0.5mV/bit,参见 REG0x3A) NTC电阻的计算公式:RNTC=REG0x37*8mV/100uA-2kohm;
#define ADC_CONFIG					0x3A	/* ADC 配置			    ADC数据选择写此寄存器之后,将对应的ADC数据锁存到 REG0x3B和REG0x3C,防止读到的数据高低位不对应
																    对应关系如下: 
																    1: adc_vin[11:0], 10mV/bit
                                                                    2: adc_vout[11:0], 6mV/ bit
                                                                    3: adc_iout1[11:0], 2.5mA/ bit
                                                                    4: adc_iout2[11:0], 2.5mA/ bit
                                                                    6: adc_ntc[11:0], 0.5mV/ bit, RNTC= adc_ntc[11:0] *0.5mV / 100uA – 2kohm;
                                                                    Other: reserved
                                                                    */
#define ADC_DATA_HIGH_8BIT			0x3B	// ADC 数据高8位	    ADC高 8bit 数据锁存 adc_data[11:04]
#define ADC_DTAT_LOW_4BIT			0x3C	// ADC 数据低4位	    ADC低 4bit 数据锁存 adc_data[03:00]

#define PD_CMD_REQUEST				0x70	/* PD命令请求		    PD 命令发送使能 MCU 通过写此 bit 为 1, 芯片将发送 REG0x70[3:0]中所定义的PD 命令。 此 bit 自动清零
																    bit[3:0]   1:Hardreset命令 Other: reserved*/
#define PD_MESAGE_SET				0x71	/* PD消息设置		    bit4: PD Get Source Extend 使能
                                                                    bit3: PD Get Status 使能
                                                                    bit2: PD Vconn Swap 使能
                                                                    bit1: PD Dr Swap 使能
                                                                    */
#define PD_SRC_CAP_CMD_SEND			0x73	// PD SRC CAP 命令发送  PD Source Cap 命令发送 { 0:无影响、1:发送Source Capability命令 此bit自动清零}
#define HARDRESET_COUNT_SET			0x75	/* 硬复位 次数设置		寄存器写 Hardreset 命令时的次数设置 { 0: 3 次、1: 1 次 }
																    写寄存器发送 Hardreset 的流程为: 先写 REG0x75[0], 设置Hardreset 的次数, 然后写 REG0x70, 发送 Hardreset。*/
#define CONNECT_SET					0x76	/* 连接设置			    bit7: 禁止端口2空载检测控制 { 0: 不禁止端口2空载检测 1:禁止端口2空载检测}
                                                                    bit6: 禁止端口1空载检测控制	{ 0: 不禁止端口1空载检测 1:禁止端口1空载检测}
                                                                    bit2: 单A口或单C口模式时,5V非快充时的限流档位配置 { 0: 默认做法,根据功率和在线端口数自动设置 
																												     1: 配置流程为: 关闭端口快充,设置此bit为1,再通过REG0xBD[5:4]来设置限流}
																    bit1: DPDM 短接使能
																    bit0: 强制 CC 不驱动使能
                                                                    */
#define IN_OUT_EVENT_CTL_SET		0x77	/* 插拔事件控制设置	    bit3: 端口2拔出事件触发
																    bit2: 端口1拔出事件触发
																    bit1: 端口2插入事件触发
																    bit0: 端口1插入事件触发*/
#define VOLTAGE_OFFSET_SET			0xA6	/* 电压偏移设置		    bit3: 输出电压固定 80mV Offset 设置	{ 0: 不使能80mV offset 1:使能80mV offset}
																    bit2: 输出电压固定 40mV Offset 设置
                                                                    */
#define FAST_CHARGE_CONFIG_0		0xAA	// 快充配置0		    bit6: QC3.0使能 { 0：不使能 1：使能}
#define PORT_MODE_CONFIG			0xAB	/* 端口模式配置		    bit7: 线补使能
																    bit[3:2]：0 单A口 1：双A口 2：单C口 3：C+A口
                                                                    */
#define FAST_CHARGE_CONFIG_1		0xAD	// 快充配置1		    bit2: 三星1.2V 模式使能 { 0：不使能 1:使能}
#define VID_CONFIG_0				0xAF	// VID配置0			    bit[7:0]: PD 认证里面的 Vendor ID 配置 VID[15:8]
#define PD_CONFIG_0					0xB0	/* PD配置0			    bit7: Fixed 5V PDO 电流设置使能
																    bit[6:0]: Fixed 5V PDO 电流		50mA/bit
                                                                    */
#define PD_CONFIG_1					0xB1	/* PD配置1			    bit7: Fixed 9V PDO 电流设置使能
																    bit[6:0]: Fixed 9V PDO 电流		50mA/bit
                                                                    */
#define PD_CONFIG_2					0xB2	/* PD配置2			    bit7: Fixed 12V PDO 电流设置使能
																    bit[6:0]: Fixed 12V PDO 电流		50mA/bit
                                                                    */
#define PD_CONFIG_3					0xB3	/* PD配置3			    bit7: Fixed 15V PDO 电流设置使能
																    bit[6:0]: Fixed 15V PDO 电流		50mA/bit
                                                                    */
#define PD_CONFIG_4					0xB4	/* PD配置4			    bit7: Fixed 20V PDO 电流设置使能
																    bit[6:0]: Fixed 20V PDO 电流		50mA/bit
                                                                    */
#define PD_CONFIG_5					0xB5	/* PD配置5			    bit7: PPS0 电流设置使能
																    bit[6:0]: PPS0电流					50mA/bit
                                                                    */
#define PD_CONFIG_6					0xB6	/* PD配置6			    bit7: PPS1 电流设置使能
																    bit[6:0]: PPS1电流					50mA/bit
                                                                    */
#define PD_CONFIG_7					0xB7	/* PD配置7              bit7: PPS1 使能
                                                                    bit6: PPS0 使能
                                                                    bit5: PD 20V PDO使能
                                                                    bit4: PD 15V PDO使能
                                                                    bit3: PD 12V  PDO使能
                                                                    bit2: PD 9V  PDO使能
                                                                    bit1: PD 读 Emarker 使能
                                                                    bit0: PD3.0 使能
                                                                    */
#define PD_CONFIG_8					0xB8	/* PD配置8              bit5: PD 5V/2A PDO广播使能
                                                                    bit4: PD 65W~70W 是否需检测emarker
                                                                    bit3: PPS 后出现Hardreset,是否自动禁止 PPS
                                                                    bit2: PD Discovery Identity 响应使能
                                                                    */
#define FAST_CHARGE_CONFIG_2		0xB9	/* 快充配置2            bit7: 1口快充使能
                                                                    bit6: 2口快充使能
                                                                    bit5: PD协议使能
                                                                    bit4: QC协议使能
                                                                    bit3: FCP协议使能
                                                                    bit2: SCP协议使能
                                                                    bit0: PE协议使能
                                                                    */
#define FAST_CHARGE_CONFIG_3		0xBA	/* 快充配置3            bit6: AFC协议使能
                                                                    bit[3:2]: 最高输出电压（除PD以外的协议）
                                                                    */
#define FAST_CHARGE_CONFIG_4		0xBC	// 快充配置4            bit3: C口空载检测使能
#define CURRENT_LIMITING_0			0xBD	/* 限流配置0            bit6: 单口转双口时，DPDM是否有效
                                                                    bit[5:4]: 双口同时打开时的1口限流值
                                                                    */
#define PD_CONFIG_9					0xBE	/* PD配置9              bit7: PPS1 的最高电压设置使能， 参见 REG0xBE[5:4]
                                                                    bit6: PPS1 功率限制使能
                                                                    bit[5:4]: PPS1最高电压设置
                                                                    bit3: PPS0 的最高电压设置使能， 参见 REG0xBE[1:0]
                                                                    bit2: PPS0 功率限制使能
                                                                    bit[1:0]: PPS0 最高电压设置
                                                                    */
#define VID_CONFIG_1				0xBF	// VID配置1             bit[7:0]: PD 认证里面的 Vendor ID 配置 VID[7:0]
#define CURRENT_LIMITING_1			0xC4	/* 限流配置1            bit4: 双口在线时 2 口独立限流使能
                                                                    bit[3:2]: 双口同时打开时的 2 口限流值
                                                                    bit[1:0]: 限流值的固定 Offset 设置
                                                                    */
#define FAST_CHARGE_CONFIG_5		0xC5	/* 快充配置5            bit4: Type-C 的 Power Level 设置
                                                                    bit2: SCP 高压协议使能
                                                                    */
#define FAST_CHARGE_CONFIG_6		0xCF	// 快充配置6            bit0: PDO 电压与 Vin 电压联动
#define FAST_CHARGE_CONFIG_7        0xD2    /* 快充配置7            bit6: 进入 PD 协议后是否响应 SCP/FCP/AFC 协议
                                                                    bit2: 非 PD 协议单独设置功率使能
                                                                    bit[1:0]: 非 PD 协议功率设置
                                                                    */
#define FAST_CHARGE_CONFIG_8        0xD3    /* 快充配置8            bit7: QC3.0 20V 使能
                                                                    bit6: 低压 SCP 协议使能
                                                                    bit[5:4]: SCP 协议最大限流设置
                                                                    */

typedef enum
{
	NORAML = 0,								// 正常状态（无保护事件）
	INPUT_OVER_VOLTAGE,						// 输入过压保护
	INPUT_UNDER_VOLTAGE,					// 输入欠压保护
	OUTPUT_OVER_CURRENT,					// 输出过流保护
	OUTPUT_SHORT_CIRCUIT,					// 输出短路保护
	OVER_TEMPERATURE						// 过温保护
}Protect_Event_e;							// 保护事件

typedef struct
{
	uint8 	Fast_Charge_State;				// 快充指示状态
	uint8 	SYS_State0;						// 系统状态0
	uint8 	SYS_State1;						// 系统状态1
	uint16	Port1_Limiting_Value;			// 端口1限流值
	uint16	Port2_Limiting_Value;			// 端口1限流值
}IC_Work_State_t;							// 芯片工作状态

typedef struct
{
	uint16	Vin_Voltage;					// 输入电压
	uint16	Vout_Voltage;					// 输出电压
	uint16	Iout1_Current;					// 输出电流1
	uint16	Iout2_Current;					// 输出电流2
	uint16	Temp_Onboard;					// 板载温度
}IC_Work_Data_t;							// 芯片工作数据

typedef struct 
{
	Protect_Event_e		Protect_Event;		// 保护事件
	IC_Work_State_t		IC_Work_State;		// 芯片工作状态
	IC_Work_Data_t		IC_Work_Data;		// 芯片工作数据
}SW3516P_Driver_t;
extern SW3516P_Driver_t	SW3516P_Driver;


#ifdef READ_DEVICE_VERSION
	void Read_Device_Version(void);
#endif // !READ_DEVICE_VERSION

void SW3516P_Get_State(void);
void SW3516P_Init(void);
/**
 * @brief SW3516P电源端口设置
 * 
 * @param param 
 * 0：单A口
 * 1：双A口
 * 2: 单C口
 * 3: C+A口
 */
void SW3516P_Power_Port_Config(uint8 param);
#endif
