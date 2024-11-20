.stabs "D:\\CSU32P20\\CSU32P20_C_Demo\\csu32p20_gpio\\source\\csu32p20_gpio\\sample\\src\\",0x64,0,0,0
.stabs "D:\\CSU32P20\\CSU32P20_C_Demo\\csu32p20_gpio\\source\\csu32p20_gpio\\sample\\src\\main.c",0x64,0,0,0
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
	_INT_FUNCTION_sec .section rom,interrupt
	.global	_INT_FUNCTION
_INT_FUNCTION:
Lfunc_begin0:
.stabs "INT_FUNCTION:f:16", 0x24,0,0,_INT_FUNCTION
.stabn 0xc0, 0, 0, _INT_FUNCTION
.stabn 0xe0, 0, 0, _INT_FUNCTION
LM0:
.stabn	0x44, 0, 30, LM0-_INT_FUNCTION
Lfunc_begin1:
	push
	banksel 0
LM1:
.stabn	0x44, 0, 32, LM1-_INT_FUNCTION
Ltmp0:
	pop
	retfie
Ltmp1:
Lfunc_end0:
Lfunc_end1:
LM2:
.stabf LM2-_INT_FUNCTION
	_main_sec .section rom
	.global	_main
_main:
Lfunc_begin2:
.stabs "main:f:16", 0x24,0,0,_main
.stabn 0xc0, 0, 0, _main
.stabn 0xe0, 0, 0, _main
LM3:
.stabn	0x44, 0, 43, LM3-_main
Lfunc_begin3:
LM4:
.stabn	0x44, 0, 44, LM4-_main
	banksel 0
	bcf _PT1EN_1, _PT1EN_1
LM5:
.stabn	0x44, 0, 45, LM5-_main
	bsf _PT1EN_0, _PT1EN_0
LBB1_1:
LM6:
.stabn	0x44, 0, 48, LM6-_main
Ltmp2:
	call _csu32p20_gpio_demo
Ltmp3:
LM7:
.stabn	0x44, 0, 46, LM7-_main
	goto LBB1_1
Ltmp4:
Lfunc_end2:
Lfunc_end3:
LM8:
.stabf LM8-_main
