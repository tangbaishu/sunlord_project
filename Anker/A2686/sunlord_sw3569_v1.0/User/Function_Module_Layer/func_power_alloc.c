#include "func_power_alloc.h"
#include "zr_gpio.h"
#include "zr_systick.h"
#include "api.h"
#include "config.h"
#include "ufcs_caps.h"
#include "pdo.h"
#if defined(DEBUG_PRINTF_OPEN)
    #include "stdio.h"
    #define LOG "busi_f_c_c: "
#endif

static const u32 PD_Supply_70W_Cap[]={
    PDO_FIXED_VSAFE5V(3000, PDO_FIXED_UNCONSTRAINED_POWER, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_FIXED_OTHERS(9000, 3000, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_FIXED_OTHERS(15000, 3000, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_FIXED_OTHERS(20000, 3500, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_PPS(5000, 5000, 11000,PDO_SPR_PPS_POWER_LIMITED)
};

static const u32 PD_Supply_65W_Cap[]={
    PDO_FIXED_VSAFE5V(3000, PDO_FIXED_UNCONSTRAINED_POWER, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_FIXED_OTHERS(9000, 3000, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_FIXED_OTHERS(15000, 3000, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_FIXED_OTHERS(20000, 3250, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_PPS(5000, 5000, 11000,PDO_SPR_PPS_POWER_LIMITED)
};

static const u32 PD_Supply_45W_Cap[]={
    PDO_FIXED_VSAFE5V(3000, PDO_FIXED_UNCONSTRAINED_POWER, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_FIXED_OTHERS(9000, 3000, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_FIXED_OTHERS(15000, 3000, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_FIXED_OTHERS(20000, 2250, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_PPS(4000, 5000, 11000,PDO_SPR_PPS_POWER_LIMITED)
};

static const u32 PD_Supply_35W_Cap[]={
    PDO_FIXED_VSAFE5V(3000, PDO_FIXED_UNCONSTRAINED_POWER, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_FIXED_OTHERS(9000, 3000, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_FIXED_OTHERS(15000, 2330, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_FIXED_OTHERS(20000, 1750, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_PPS(3000, 5000, 11000,PDO_SPR_PPS_POWER_LIMITED)
};

static const u32 PD_Supply_25W_Cap[]={
    PDO_FIXED_VSAFE5V(3000, PDO_FIXED_UNCONSTRAINED_POWER, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_FIXED_OTHERS(9000, 2770, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_FIXED_OTHERS(15000, 1660, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_FIXED_OTHERS(20000, 1250, PDO_FIXED_NO_PEAK_CURRENT),
    PDO_PPS(2750, 5000, 11000,PDO_SPR_PPS_POWER_LIMITED)
};

static const ufcs_output_cap_t UFCS_Supply_33W_Cap[] = {
    {
        .fields.head = UFCS_CAP_SET_HEAD(UFCS_CAP_ID1, UFCS_CURR_STEP_50MA, UFCS_VOL_MIN_STEP_10MV),
        .fields.hiMaxVolage = UFCS_CAP_SET_HI_BYTE(1100),        // unit = 10mV
        .fields.loMaxVolage = UFCS_CAP_SET_LO_BYTE(1100),        // unit = 10mV
        .fields.hiMinVolage = UFCS_CAP_SET_HI_BYTE(500),         // unit = 10mV
        .fields.loMinVolage = UFCS_CAP_SET_LO_BYTE(500),         // unit = 10mV
        .fields.hiMaxCurrent = UFCS_CAP_SET_HI_BYTE(300),        // unit = 10mA
        .fields.loMaxCurrent = UFCS_CAP_SET_LO_BYTE(300),        // unit = 10mA
        .fields.minCurrent = 100,                                // unit = 10mV
    },
};

static const ufcs_output_cap_t UFCS_Supply_22W_Cap[] = {
    {
        .fields.head = UFCS_CAP_SET_HEAD(UFCS_CAP_ID1, UFCS_CURR_STEP_50MA, UFCS_VOL_MIN_STEP_10MV),
        .fields.hiMaxVolage = UFCS_CAP_SET_HI_BYTE(1100),        // unit = 10mV
        .fields.loMaxVolage = UFCS_CAP_SET_LO_BYTE(1100),        // unit = 10mV
        .fields.hiMinVolage = UFCS_CAP_SET_HI_BYTE(500),         // unit = 10mV
        .fields.loMinVolage = UFCS_CAP_SET_LO_BYTE(500),         // unit = 10mV
        .fields.hiMaxCurrent = UFCS_CAP_SET_HI_BYTE(200),        // unit = 10mA
        .fields.loMaxCurrent = UFCS_CAP_SET_LO_BYTE(200),        // unit = 10mA
        .fields.minCurrent = 100,                                // unit = 10mV
    },
};

void Fast_Charge_Enable(config_data_t *config_data, bool protocol_enable);

/**
 * @brief 
 * 
 * @param fast_charge_en 
 * @param power 
 */
void Func_Power_Alloc(uint8_t fast_charge_en, uint16_t power)
{
    config_data_t *config_data;
    config_data = Config_Get();
    if(fast_charge_en == false)
    {
        Fast_Charge_Enable(config_data, false);
    }
    else
    {
        Fast_Charge_Enable(config_data, true);
    }
    
    switch(power)
    {
        case 5:     Power_PDO_Mode_Select(config_data, POWER_PDO_5W); break;
        case 25:    Power_PDO_Mode_Select(config_data, POWER_PDO_25W); break;
        case 35:    Power_PDO_Mode_Select(config_data, POWER_PDO_35W); break;
        case 45:    Power_PDO_Mode_Select(config_data, POWER_PDO_45W); break;
        case 65:    Power_PDO_Mode_Select(config_data, POWER_PDO_65W); break;
        case 70:    Power_PDO_Mode_Select(config_data, POWER_PDO_70W); break;
        default: break;
    }
}

/**
 * @brief 快充开关
 * 关闭所有快充协议{BC1.2协议 5V 1.5A (恒流点1A，过流点1.5A) 除外}、同时控制Type-c 最高功率5V/0.9A(通过修改CC的Rp电阻去实现)
 * @param config_data       快充配置数据
 * @param protocol_enable   1：使能快充协议 0：关闭快充协议
 */
void Fast_Charge_Enable(config_data_t *config_data, bool protocol_enable)
{
	if(protocol_enable)
	{
		// config_data->isTfcpEnabled = 1;                                     //bit0 !< whether to enable tfcp
		// config_data->isVooc1p0Enabled = 1;                                  //bit1 !< whether to enable vooc 1.0
		// config_data->isVooc4p0Enabled = 1;                                  //bit2 !< whether to enable vooc 4.0
		// config_data->isSuperVooc2p0Enabled = 1;                             //bit3 !< whether to enable vooc supper 2.0
		config_data->isQc2p0Enabled = 1;                                    //bit5 !< whether to enable qc2.0
		config_data->isQc3p0Enabled = 1;                                    //bit6 !< whether to enable qc3.0
		config_data->isQc3plusEnabled = 1;                                  //bit7 !< whether to enable qc3+
		config_data->isAfcEnabled = 1;                                      //bit8 !< whether to enable afc
		config_data->isFcpEnabled = 1;                                      //bit9 !< whether to enable FCP
		config_data->isHvScpEnabled = 1;                                    //bit10 !< whether to enable high voltage scp
		config_data->isLvScpEnabled = 1;                                    //bit11 !< whether to enable low voltage scp
		config_data->isAppleEnabled = 1;                                    //bit13 !< whether to enable apple 2.7v mode
		config_data->isUfcsEnabled = 1;                                     //bit15 !< whether to enable ufcs
		config_data->isEmarkEnabled = 1;                                    //bit16 !< whether to read emark
		config_data->isPdEnabled = 1;                                      	//bit17 !< whether to enable PD
        config_data->typecCcTerm = RP_3A;
	}
	else
	{
		// config_data->isTfcpEnabled = 0;                                     //bit0 !< whether to enable tfcp
		// config_data->isVooc1p0Enabled = 0;                                  //bit1 !< whether to enable vooc 1.0
		// config_data->isVooc4p0Enabled = 0;                                  //bit2 !< whether to enable vooc 4.0
		// config_data->isSuperVooc2p0Enabled = 0;                             //bit3 !< whether to enable vooc supper 2.0
		config_data->isQc2p0Enabled = 0;                                    //bit5 !< whether to enable qc2.0
		config_data->isQc3p0Enabled = 0;                                    //bit6 !< whether to enable qc3.0
		config_data->isQc3plusEnabled = 0;                                  //bit7 !< whether to enable qc3+
		config_data->isAfcEnabled = 0;                                      //bit8 !< whether to enable afc
		config_data->isFcpEnabled = 0;                                      //bit9 !< whether to enable FCP
		config_data->isHvScpEnabled = 0;                                    //bit10 !< whether to enable high voltage scp
		config_data->isLvScpEnabled = 0;                                    //bit11 !< whether to enable low voltage scp
		config_data->isAppleEnabled = 0;                                    //bit13 !< whether to enable apple 2.7v mode
		config_data->isUfcsEnabled = 0;                                     //bit15 !< whether to enable ufcs
		config_data->isEmarkEnabled = 0;                                    //bit16 !< whether to read emark
		config_data->isPdEnabled = 0;                                      	//bit17 !< whether to enable PD	
        config_data->typecCcTerm = RP_DEFAULT_USB;                          //
	}
}

/**
 * @brief 快充PDO挡位输出模式选择
 * 输入 POWER_PDO_Mode_e 内预留的PDO模式参数，并基于 right_away 参数
 * 控制协议块 输出的PDO挡位
 * @param config_data 快充配置数据
 * @param mode_select POWER_PDO_Mode_e内预留的各项参数
 * @param right_away  1：立即执行 0：仅修改 config_data 变量数据，不在此生效
 */
void Power_PDO_Mode_Select(config_data_t *config_data, POWER_PDO_Mode_e mode_select)
{
    switch(mode_select)
    {
        case POWER_PDO_70W: goto mode_a; break;
        case POWER_PDO_65W: goto mode_b; break;
        case POWER_PDO_45W: goto mode_c; break;
        case POWER_PDO_35W: goto mode_d; break;
        case POWER_PDO_25W: goto mode_e; break;
        case POWER_PDO_5W: goto mode_f; break;
    }
mode_a:
    #if defined(DEBUG_PRINTF_OPEN)
        printf(LOG"mode_a\r\n");
    #endif

    /*** dpdm ***/
    config_data->dpdmFixPower = 24;

/*** 三星 ***/  
    /*** AFC ***/
    config_data->afcMaxVoltage = AFC_MAX_VOL_12V;

/*** 华为 ***/    
    /*** SCP ***/
    config_data->scpMaxCurrent = 23;                 // uint: 100mA
    config_data->scpMaxPower = 23;                   // unit: W
    config_data->scpMaxVoltage = 100;                // unit: 100mV

    /*** FCP ***/
    config_data->fcpMaxVoltage = FCP_MAX_VOL_12V;

/*** 公有部分 ***/ 
    /*** QC ***/
    config_data->qc2p0MaxVoltage = QC2P0_MAX_VOL_12V;
    config_data->qc3p0MaxVoltage = QC3P0_MAX_VOL_12V;
    config_data->qc3MaxPower = QC3_MAX_POWER_18W;

    /*** ufcs ***/
    config_data->ufcsSrcCap = (uint8_t*)UFCS_Supply_33W_Cap;
    config_data->ufcsSrcCapNum = 1;
    config_data->isUfcsCertEnabled = false;
    config_data->ufcsDevTemp = 25;
    config_data->ufcsUsbTemp = 25;

    /*** PD ***/
    config_data->pdSrcCap = PD_Supply_70W_Cap;
    config_data->pdMaxPower = 70;
    config_data->sprPdoNumber = 5;
    config_data->sprFixPdoNumber = 4;
    config_data->eprPdoNumber = 0;
    config_data->eprFixPdoNumber = 0;
    goto Enable_Protocol;

mode_b:
    #if defined(DEBUG_PRINTF_OPEN)
        printf(LOG"mode_b\r\n");
    #endif
     /*** dpdm ***/
    config_data->dpdmFixPower = 24;

/*** 三星 ***/  
    /*** AFC ***/
    config_data->afcMaxVoltage = AFC_MAX_VOL_12V;

/*** 华为 ***/    
    /*** SCP ***/
    config_data->scpMaxCurrent = 23;                 // uint: 100mA
    config_data->scpMaxPower = 23;                   // unit: W
    config_data->scpMaxVoltage = 100;                // unit: 100mV

    /*** FCP ***/
    config_data->fcpMaxVoltage = FCP_MAX_VOL_12V;

/*** 公有部分 ***/ 
    /*** QC ***/
    config_data->qc2p0MaxVoltage = QC2P0_MAX_VOL_12V;
    config_data->qc3p0MaxVoltage = QC3P0_MAX_VOL_12V;
    config_data->qc3MaxPower = QC3_MAX_POWER_18W;

    /*** ufcs ***/
    config_data->ufcsSrcCap = (uint8_t*)UFCS_Supply_33W_Cap;
    config_data->ufcsSrcCapNum = 1;
    config_data->isUfcsCertEnabled = false;
    config_data->ufcsDevTemp = 25;
    config_data->ufcsUsbTemp = 25;

    /*** PD ***/
    config_data->pdSrcCap = PD_Supply_65W_Cap;
    config_data->pdMaxPower = 65;
    config_data->sprPdoNumber = 5;
    config_data->sprFixPdoNumber = 4;
    config_data->eprPdoNumber = 0;
    config_data->eprFixPdoNumber = 0;
    goto Enable_Protocol;

mode_c:
    #if defined(DEBUG_PRINTF_OPEN)
        printf(LOG"mode_c\r\n");
    #endif

    /*** dpdm ***/
    config_data->dpdmFixPower = 24;

/*** 三星 ***/  
    /*** AFC ***/
    config_data->afcMaxVoltage = AFC_MAX_VOL_12V;

/*** 华为 ***/    
    /*** SCP ***/
    config_data->scpMaxCurrent = 23;                 // uint: 100mA
    config_data->scpMaxPower = 23;                   // unit: W
    config_data->scpMaxVoltage = 100;                // unit: 100mV

    /*** FCP ***/
    config_data->fcpMaxVoltage = FCP_MAX_VOL_12V;

/*** 公有部分 ***/ 
    /*** QC ***/
    config_data->qc2p0MaxVoltage = QC2P0_MAX_VOL_12V;
    config_data->qc3p0MaxVoltage = QC3P0_MAX_VOL_12V;
    config_data->qc3MaxPower = QC3_MAX_POWER_18W;

    /*** ufcs ***/
    config_data->ufcsSrcCap = (uint8_t*)UFCS_Supply_33W_Cap;
    config_data->ufcsSrcCapNum = 1;
    config_data->isUfcsCertEnabled = false;
    config_data->ufcsDevTemp = 25;
    config_data->ufcsUsbTemp = 25;

    /*** PD ***/
    config_data->pdSrcCap = PD_Supply_45W_Cap;
    config_data->pdMaxPower = 45;
    config_data->sprPdoNumber = 5;
    config_data->sprFixPdoNumber = 4;
    config_data->eprPdoNumber = 0;
    config_data->eprFixPdoNumber = 0;
    goto Enable_Protocol;

mode_d:
    #if defined(DEBUG_PRINTF_OPEN)
        printf(LOG"mode_d\r\n");
    #endif

    /*** dpdm ***/
    config_data->dpdmFixPower = 24;

/*** 三星 ***/  
    /*** AFC ***/
    config_data->afcMaxVoltage = AFC_MAX_VOL_12V;

/*** 华为 ***/    
    /*** SCP ***/
    config_data->scpMaxCurrent = 23;                 // uint: 100mA
    config_data->scpMaxPower = 23;                   // unit: W
    config_data->scpMaxVoltage = 100;                // unit: 100mV

    /*** FCP ***/
    config_data->fcpMaxVoltage = FCP_MAX_VOL_12V;

/*** 公有部分 ***/ 
    /*** QC ***/
    config_data->qc2p0MaxVoltage = QC2P0_MAX_VOL_12V;
    config_data->qc3p0MaxVoltage = QC3P0_MAX_VOL_12V;
    config_data->qc3MaxPower = QC3_MAX_POWER_18W;

    /*** ufcs ***/
    config_data->ufcsSrcCap = (uint8_t*)UFCS_Supply_33W_Cap;
    config_data->ufcsSrcCapNum = 1;
    config_data->isUfcsCertEnabled = false;
    config_data->ufcsDevTemp = 25;
    config_data->ufcsUsbTemp = 25;

    /*** PD ***/
    config_data->pdSrcCap = PD_Supply_35W_Cap;
    config_data->pdMaxPower = 35;
    config_data->sprPdoNumber = 5;
    config_data->sprFixPdoNumber = 4;
    config_data->eprPdoNumber = 0;
    config_data->eprFixPdoNumber = 0;
    goto Enable_Protocol;

mode_e:
    #if defined(DEBUG_PRINTF_OPEN)
        printf(LOG"mode_e\r\n");
    #endif

         /*** dpdm ***/
    config_data->dpdmFixPower = 24;

/*** 三星 ***/  
    /*** AFC ***/
    config_data->afcMaxVoltage = AFC_MAX_VOL_12V;

/*** 华为 ***/    
    /*** SCP ***/
    config_data->scpMaxCurrent = 23;                 // uint: 100mA
    config_data->scpMaxPower = 23;                   // unit: W
    config_data->scpMaxVoltage = 100;                // unit: 100mV

    /*** FCP ***/
    config_data->fcpMaxVoltage = FCP_MAX_VOL_12V;

/*** 公有部分 ***/ 
    /*** QC ***/
    config_data->qc2p0MaxVoltage = QC2P0_MAX_VOL_12V;
    config_data->qc3p0MaxVoltage = QC3P0_MAX_VOL_12V;
    config_data->qc3MaxPower = QC3_MAX_POWER_18W;

    /*** ufcs ***/
    config_data->ufcsSrcCap = (uint8_t*)UFCS_Supply_22W_Cap;
    config_data->ufcsSrcCapNum = 1;
    config_data->isUfcsCertEnabled = false;
    config_data->ufcsDevTemp = 25;
    config_data->ufcsUsbTemp = 25;

    /*** PD ***/
    config_data->pdSrcCap = PD_Supply_25W_Cap;
    config_data->pdMaxPower = 25;
    config_data->sprPdoNumber = 5;
    config_data->sprFixPdoNumber = 4;
    config_data->eprPdoNumber = 0;
    config_data->eprFixPdoNumber = 0;
    goto Enable_Protocol;

mode_f:
    #if defined(DEBUG_PRINTF_OPEN)
        printf(LOG"mode_f\r\n");
    #endif
    
    goto Disable_Protocol;


Enable_Protocol:
    Fast_Charge_Enable(config_data, ENABLE);
    return;

Disable_Protocol:
    Fast_Charge_Enable(config_data, DISABLE);
    return;
}

void Right_Away_Power_PDO_Mode(void)
{
        Config_Apply();             // 立即应用 配置表
	#if 0
        if(Busi_Port_Detection_Data.Port1_Out_Protocol_Data.Current_Power_Grade == POWER_PDO_5W)
        {
            // Systick_Delay_Ms(400);Systick_Delay_Ms(400);Systick_Delay_Ms(400);Systick_Delay_Ms(400);Systick_Delay_Ms(400);
            // Systick_Delay_Ms(400);Systick_Delay_Ms(400);Systick_Delay_Ms(400);Systick_Delay_Ms(400);Systick_Delay_Ms(400);
            // Power_Gate_Ctrl(1, false);  // true on; fasle off
            // Power_Gate_Ctrl(2, false);  // true on; fasle off
            // Systick_Delay_Ms(400);Systick_Delay_Ms(400);Systick_Delay_Ms(400);Systick_Delay_Ms(400);Systick_Delay_Ms(400);
            // Systick_Delay_Ms(400);Systick_Delay_Ms(400);Systick_Delay_Ms(400);Systick_Delay_Ms(400);Systick_Delay_Ms(400);
            // Power_Gate_Ctrl(1, true);  // true on; fasle off
            // Power_Gate_Ctrl(2, true);  // true on; fasle off
            return;
        }
	#endif
        Ufcs_Set_Power_Change();    // ufcs 
        Pd_Rebroadcast_Src_Cap();   // PD重新广播
}

