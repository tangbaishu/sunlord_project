/****************************************************************************
 * @copyright Copyright(C) 2020-2023 Ismartware Limited. All rights reserved.
 * @file ufcs_caps.h
 * @brief  Provide macros to assist ufcs output capabilities generation & decoded
 * @author SW team
 ****************************************************************************/
#ifndef UFCS_CAPS_H
#define UFCS_CAPS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// clang-format off

/** 
 * ufcs capability ID definition
 * 作用:
 * 1、技术规范中的标识作用：标识特定的供电能力和充电特性，ID帮助设备之间识别和匹配各自的充电能力
 * 2、UFCS协议标准中定义了多达20种消息类型，包括Output_Capabilities等
 * 3、UFCS支持广泛的输出电压和功率输出，capability ID在这里可能用于标识不同设备或应用场景下的具体功率需求和电压要求
 * 4、安全性和性能 可能用于标识和激活特定的安全保护措施，如过压保护、欠压保护、过流保护等
 * 5、兼容性 可能用于确保与PD和QC等其他协议的兼容性，避免协议冲突
 */ 
typedef enum 
{
    UFCS_CAP_ID1 = 1,           //!< ID 1
    UFCS_CAP_ID2 = 2,           //!< ID 2
    UFCS_CAP_ID3 = 3,           //!< ID 3
    UFCS_CAP_ID4 = 4,           //!< ID 4
    UFCS_CAP_ID5 = 5,           //!< ID 5
    UFCS_CAP_ID6 = 6,           //!< ID 6
    UFCS_CAP_ID7 = 7,           //!< ID 7
} ufcs_cap_id_e;

/** 
 * ufcs capability current step
 */ 
typedef enum
{
    UFCS_CURR_STEP_10MA = 0,         //!< current step is 10mA
    UFCS_CURR_STEP_20MA,             //!< current step is 20mA
    UFCS_CURR_STEP_30MA,             //!< current step is 30mA
    UFCS_CURR_STEP_40MA,             //!< current step is 40mA
    UFCS_CURR_STEP_50MA,             //!< current step is 50mA
    UFCS_CURR_STEP_RESERVED,         //!< current step reserved
} ufcs_cap_curr_min_step_e;

/** 
 * ufcs capability voltage min step
 */ 
typedef enum
{
    UFCS_VOL_MIN_STEP_10MV = 0,         //!< voltage min step is 10mV
    UFCS_VOL_MIN_STEP_20MV = 1,         //!< voltage min step is 20mV
} ufcs_cap_vol_min_step_e;

/**
 * ufcs cap head
 */
typedef union 
{
    uint8_t data;
    struct 
    {
        ufcs_cap_vol_min_step_e volMinStep : 1;         //!< bit 0, voltage minimal step, see ufcs_cap_vol_min_step_e
        ufcs_cap_curr_min_step_e currMinStep : 3;       //!< bit 1-3, current step, see ufcs_cap_curr_min_step_e
        ufcs_cap_id_e outId : 4;                        //!< bit 4-7, cap id, see ufcs_cap_id_e
    } fields;   
} ufcs_cap_head_t;

#define UFCS_CAP_NUM (8)

/**
 * ufcs out capability
 */
typedef union
{
    uint8_t data[UFCS_CAP_NUM];
    struct
    {
        uint8_t head : 8;               //!< head, see ufcs_cap_head_t
        uint8_t hiMaxVolage : 8;        //!< max output voltage, hi 8bits unit:10mv
        uint8_t loMaxVolage : 8;        //!< max output voltage, lo 8bits unit:10mv
        uint8_t hiMinVolage : 8;        //!< min output voltage, hi 8bits unit:10mv
        uint8_t loMinVolage : 8;        //!< min output voltage, lo 8bits unit:10mv
        uint8_t hiMaxCurrent: 8;        //!< max output current, hi 8bits unit:10ma
        uint8_t loMaxCurrent: 8;        //!< max output current, lo 8bits unit:10ma
        uint8_t minCurrent: 8;          //!< min output current, unit:10ma
    } fields;
} ufcs_output_cap_t;

/// set cap head
#define UFCS_CAP_SET_HEAD(id, curr_step, vol_step) (id << 4 | curr_step << 1 | vol_step) 
/// set max voltage, min voltage and max current HI or LOW byte
#define UFCS_CAP_SET_HI_BYTE(val) ((val >> 8) & 0xFF)
#define UFCS_CAP_SET_LO_BYTE(val) (val & 0xFF)


#define UFCS_CAP_GET_BYTE(hi, lo) (hi << 8 | lo)

/// get max voltage, unit mV
#define UFCS_CAP_GET_MAX_VOL(x) (10 * UFCS_CAP_GET_BYTE(x->fields.hiMaxVolage, x->fields.loMaxVolage))
/// get min voltage, unit mV
#define UFCS_CAP_GET_MIN_VOL(x) (10 * UFCS_CAP_GET_BYTE(x->fields.hiMinVolage, x->fields.loMinVolage))
/// get max current, unit mV
#define UFCS_CAP_GET_MAX_CURR(x) (10 * UFCS_CAP_GET_BYTE(x->fields.hiMaxCurrent, x->fields.loMaxCurrent))
/// get min current, unit mV
#define UFCS_CAP_GET_MIN_CURR(x) (10 * x->fields.minCurrent)

// clang-format on

#ifdef __cplusplus
}
#endif

#endif        // UFCS_CAPS_H
