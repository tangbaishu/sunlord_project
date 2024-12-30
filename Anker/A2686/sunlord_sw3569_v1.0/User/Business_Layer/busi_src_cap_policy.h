/**
 * @file busi_src_cap_policy.h	电源供应策略业务
 * @author  tong.libo@sunlord.com.cn
 * @brief 
 * 1、基于快充配置参数，调整电源供应功率
 * 2、基于NTC温度数据，调整电源供应功率（NTC温度过温逻辑）
 * 3、基于端口插入信息，调整电源供应功率
 * 4、基于sink请求的功率信息，调整电源供应功率（按需功率分配）
 * @version 0.1
 * @date 2024-12-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _POWER_POLICY_H_
#define _POWER_POLICY_H_

void Busi_SRC_CAP_Policy_Param();


#endif
