/**
 * @file app_abnormal_protection.h		异常保护应用
 * @author tong.libo@sunlord.com.cn
 * @brief 
 * @version 0.1
 * @date 2024-12-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "system.h"

typedef struct
{

	void (*P_Temperature_Detection) (void);			// 温度检测与处理
}App_Abnormal_Protection_Func;
