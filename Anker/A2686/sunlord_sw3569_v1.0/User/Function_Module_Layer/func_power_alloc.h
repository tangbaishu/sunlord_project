/**
 * @file busi_fast_charge_config.h	快充配置业务
 * @author tong.libo@sunlord.com.cn
 * @brief 
 * 将客户提供的快充应用需求，转换成软件配置参数
 * @version 0.1
 * @date 2024-12-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _FAST_POWER_ALLOC_H_
#define _FAST_POWER_ALLOC_H_

#include "api.h"
#include "port_data_library.h"

void Func_Power_Alloc(uint8_t fast_charge_en, uint16_t power);

void Power_PDO_Mode_Select(config_data_t *config_data, POWER_PDO_Mode_e mode_select);

void Right_Away_Power_PDO_Mode(void);

#endif
