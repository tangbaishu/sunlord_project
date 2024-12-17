#include "log_function.h"
#ifdef LOG_PRINTF
	#include "stdio.h"
	#include "string.h"
#endif // DEBUG

#ifdef LOG_ABNORMAL
	const char Abnormal_Occurred_Event_Name[19][15] = {
		"LPS", "DM OVP", "DP OVP", "CC2 OVP", "CC1 OVP", "VCONN2 OCP",
		"VCONN1 OCP", "DIE OTP", "VOUT SCP", "VOUT Slow OVP", "VOUT Fast OVP", "VIN UVLO",
		"VIN OVP", "VOUT UVP", "DIE Warn", "DM2 Weak Short", "DP2 Weak Short", "DM1 Weak Short", "DP1 Weak Short"
		};

	const char Abnormal_Release_Event_Name[14][23] = {
		"DM OVP Release", "DP OVP Release", "CC2 OVP Release", "CC1 OVP Release", "DIE OTP Release", 
		"VOUT SLOW OVP Release", "VOUT FAST OVP  Release", "VIN UVLO Release", "VIN OVP Release", "DIE WARN Release", 
		"DM2 Weak Short Release", "DP2 Weak Short Release", "DM1 Weak Short Release", "DP1 Weak Short Release"
	};
#endif // DEBUG

void LOG_Abnormal_Occurred_Event(u32 pending_flag)
{
	#ifdef LOG_ABNORMAL
		if(pending_flag != 0)
		{
			printf("Abnormal_Occurred_Event:\r\n");
		}
		for(uint8_t num=0; num < 19; num++)
		{
			if(pending_flag & (0x01 << num))
			{
				printf("%s", Abnormal_Occurred_Event_Name[num]);
				printf("\r\n");
			}
		}
	#endif
}

void LOG_Abnormal_Release_Event(u32 pending_flag)
{
		#ifdef LOG_ABNORMAL
		if(pending_flag != 0)
		{
			printf("Abnormal_Release_Event:\r\n");
		}
		for(uint8_t num=0; num < 14; num++)
		{
			if(pending_flag & (0x01 << num))
			{
				printf("%s", Abnormal_Release_Event_Name[num]);
				printf("\r\n");
			}
		}
	#endif
}
