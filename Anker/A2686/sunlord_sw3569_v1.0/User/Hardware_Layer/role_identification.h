#ifndef _ROLE_IDENTIFICATION_H_
#define _ROLE_IDENTIFICATION_H_

#include "system.h"
#include "hardware_config.h"

/**
 * @brief 角色识别函数
 * 
 * @param device_type 1：主机、2：从机
 */
void Role_Identification(Device_Type_e *device_type);

#endif
