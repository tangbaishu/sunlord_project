.stabs "D:\\Work_Area\\Software\\PD_Application\\CSU32M10\\source\\sample\\src\\",0x64,0,0,0
.stabs "D:\\Work_Area\\Software\\PD_Application\\CSU32M10\\source\\sample\\src\\csu32m10_timer0.c",0x64,0,0,0
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
.include "csccCond.inc"
.include "csccCmp.inc"
.include "csccShift.inc"
	_csu32m10_timer0_init_sec .section rom
	.global	_csu32m10_timer0_init
_csu32m10_timer0_init:
Lfunc_begin0:
.stabs "csu32m10_timer0_init:f:16", 0x24,0,0,_csu32m10_timer0_init
.stabn 0xc0, 0, 0, _csu32m10_timer0_init
.stabn 0xe0, 0, 0, _csu32m10_timer0_init
LM0:
.stabn	0x44, 0, 32, LM0-_csu32m10_timer0_init
Lfunc_begin1:
LM1:
.stabn	0x44, 0, 33, LM1-_csu32m10_timer0_init
	banksel 0
	movlw 0xfc
	andwf _TM0CON+0, F
Ltmp0:
LM2:
.stabn	0x44, 0, 34, LM2-_csu32m10_timer0_init
	bsf _T0RATE_0, _T0RATE_0
	bcf _T0RATE_1, _T0RATE_1
	bsf _T0RATE_2, _T0RATE_2
Ltmp1:
LM3:
.stabn	0x44, 0, 35, LM3-_csu32m10_timer0_init
	movlw 124
	movwf _TM0IN+0
Ltmp2:
LM4:
.stabn	0x44, 0, 36, LM4-_csu32m10_timer0_init
	bcf _TM0IF, _TM0IF
Ltmp3:
LM5:
.stabn	0x44, 0, 37, LM5-_csu32m10_timer0_init
	bsf _TM0IE, _TM0IE
	bsf _GIE, _GIE
Ltmp4:
LM6:
.stabn	0x44, 0, 38, LM6-_csu32m10_timer0_init
	bcf _T0RSTB, _T0RSTB
Ltmp5:
LM7:
.stabn	0x44, 0, 39, LM7-_csu32m10_timer0_init
	bsf _T0EN, _T0EN
Ltmp6:
LM8:
.stabn	0x44, 0, 40, LM8-_csu32m10_timer0_init
	return
Ltmp7:
Lfunc_end0:
Lfunc_end1:
LM9:
.stabf LM9-_csu32m10_timer0_init