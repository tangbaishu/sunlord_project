.stabs "D:\\Work_Area\\项目\\客户项目\\赋研星辰\\芯片资料\\芯海(CHIPSEA)\\CSU32P10_C_example\\",0x64,0,0,0
.stabs "D:\\Work_Area\\项目\\客户项目\\赋研星辰\\芯片资料\\芯海(CHIPSEA)\\CSU32P10_C_example\\data.c",0x64,0,0,0
.stabs "int:t1=r1;-32768;32767;",0x80,0,0,0
.stabs "unsigned int:t2=r2;0;65535;",0x80,0,0,0
.stabs "long int:t3=r3;-2147483648;2147483647;",0x80,0,0,0
.stabs "unsigned long int:t4=r4;0;4294967295;",0x80,0,0,0
.stabs "long long unsigned int:t6=r6;0;4294967295;",0x80,0,0,0
.stabs "short int:t7=r7;-32768;32767;",0x80,0,0,0
.stabs "short unsigned int:t8=r8;0;65535;",0x80,0,0,0
.stabs "char:t9=r9;-128;127;",0x80,0,0,0
.stabs "unsigned char:t10=r10;0;255;",0x80,0,0,0
.stabs "_Bool:t11=r11;0;1;",0x80,0,0,0
.stabs "sbit:t12=r12;0;1;",0x80,0,0,0
.stabs "float:t13=r13;4;0;",0x80,0,0,0
.stabs "double:t14=r14;4;0;",0x80,0,0,0
.stabs "long double:t15=r15;4;0;",0x80,0,0,0
.stabs "void:t16=r10;0;255;",0x80,0,0,0
.stabs "B_Time500ms:G12",0x20,0,0,_B_Time500ms
.stabs "B_pwm2:G12",0x20,0,0,_B_pwm2
.stabs "B_pwm3:G12",0x20,0,0,_B_pwm3
.stabs "B_Sleep:G12",0x20,0,0,_B_Sleep
.stabs "led:G12",0x20,0,0,_led
.stabs "led1:G12",0x20,0,0,_led1
.stabs "led2:G12",0x20,0,0,_led2
.stabs "if_adc:G12",0x20,0,0,_if_adc
.stabs "if_adc1:G12",0x20,0,0,_if_adc1
.stabs "if_adc2:G12",0x20,0,0,_if_adc2
.stabs "if_pwm:G12",0x20,0,0,_if_pwm
.stabs "AdData:G2",0x20,0,0,_AdData
.stabs "CurTabData:G2",0x20,0,0,_CurTabData
.stabs "t1s_count:G2",0x20,0,0,_t1s_count
.stabs "t10s_count:G2",0x20,0,0,_t10s_count
.stabs "R_AdcStation1_Cnt:G2",0x20,0,0,_R_AdcStation1_Cnt
.stabs "R_AdcStation2_Cnt:G2",0x20,0,0,_R_AdcStation2_Cnt
.stabs "R_AdcStation3_Cnt:G2",0x20,0,0,_R_AdcStation3_Cnt
.include "csccCond.inc"
.include "csccCmp.inc"
.include "csccShift.inc"
	_Bit_Initialization_sec .section rom
	.global	_Bit_Initialization
_Bit_Initialization:
Lfunc_begin0:
.stabs "Bit_Initialization:f:16", 0x24,0,0,_Bit_Initialization
.stabn 0xc0, 0, 0, _Bit_Initialization
.stabn 0xe0, 0, 0, _Bit_Initialization
LM0:
.stabn	0x44, 0, 20, LM0-_Bit_Initialization
Lfunc_begin1:
LM1:
.stabn	0x44, 0, 21, LM1-_Bit_Initialization
	banksel 0
	bsf _if_adc, _if_adc
LM2:
.stabn	0x44, 0, 22, LM2-_Bit_Initialization
	bcf _if_adc1, _if_adc1
LM3:
.stabn	0x44, 0, 23, LM3-_Bit_Initialization
	bcf _if_adc2, _if_adc2
LM4:
.stabn	0x44, 0, 24, LM4-_Bit_Initialization
	bcf _if_pwm, _if_pwm
LM5:
.stabn	0x44, 0, 25, LM5-_Bit_Initialization
	return
Ltmp0:
Lfunc_end0:
Lfunc_end1:
LM6:
.stabf LM6-_Bit_Initialization
	_if_adc_data.c .section bank0,bitfield=0xFF,uninit
	_if_adc .ds 1
	.ends
	_if_adc1_data.c .section bank0,bitfield=0xFF,uninit
	_if_adc1 .ds 1
	.ends
	_if_adc2_data.c .section bank0,bitfield=0xFF,uninit
	_if_adc2 .ds 1
	.ends
	_if_pwm_data.c .section bank0,bitfield=0xFF,uninit
	_if_pwm .ds 1
	.ends
	_B_Time500ms_data.c .section bank0,bitfield=0xFF,uninit
	_B_Time500ms .ds 1
	.ends
	_B_pwm2_data.c .section bank0,bitfield=0xFF,uninit
	_B_pwm2 .ds 1
	.ends
	_B_pwm3_data.c .section bank0,bitfield=0xFF,uninit
	_B_pwm3 .ds 1
	.ends
	_B_Sleep_data.c .section bank0,bitfield=0xFF,uninit
	_B_Sleep .ds 1
	.ends
	_led_data.c .section bank0,addr=32,bitfield=4,uninit
	_led .ds 1
	.ends
	_led1_data.c .section bank0,addr=32,bitfield=5,uninit
	_led1 .ds 1
	.ends
	_led2_data.c .section bank0,addr=40,bitfield=2,uninit
	_led2 .ds 1
	.ends
	_AdData_data.c .section bank0,uninit
	_AdData .ds 2
	.ends
	_CurTabData_data.c .section bank0,uninit
	_CurTabData .ds 2
	.ends
	_t1s_count_data.c .section bank0,uninit
	_t1s_count .ds 2
	.ends
	_t10s_count_data.c .section bank0,uninit
	_t10s_count .ds 2
	.ends
	_R_AdcStation1_Cnt_data.c .section bank0,uninit
	_R_AdcStation1_Cnt .ds 2
	.ends
	_R_AdcStation2_Cnt_data.c .section bank0,uninit
	_R_AdcStation2_Cnt .ds 2
	.ends
	_R_AdcStation3_Cnt_data.c .section bank0,uninit
	_R_AdcStation3_Cnt .ds 2
	.ends
	.global	_if_adc
	.global	_if_adc1
	.global	_if_adc2
	.global	_if_pwm
	.global	_B_Time500ms
	.global	_B_pwm2
	.global	_B_pwm3
	.global	_B_Sleep
	.global	_led
	.global	_led1
	.global	_led2
	.global	_AdData
	.global	_CurTabData
	.global	_t1s_count
	.global	_t10s_count
	.global	_R_AdcStation1_Cnt
	.global	_R_AdcStation2_Cnt
	.global	_R_AdcStation3_Cnt