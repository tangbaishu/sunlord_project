.stabs "D:\\Work_Area\\项目\\客户项目\\赋研星辰\\芯片资料\\芯海(CHIPSEA)\\CSU32P10_C_example\\",0x64,0,0,0
.stabs "D:\\Work_Area\\项目\\客户项目\\赋研星辰\\芯片资料\\芯海(CHIPSEA)\\CSU32P10_C_example\\lowpower.c",0x64,0,0,0
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
	_sleep_sec .section rom
	.global	_sleep
_sleep:
Lfunc_begin0:
.stabs "sleep:f:16", 0x24,0,0,_sleep
.stabn 0xc0, 0, 0, _sleep
.stabn 0xe0, 0, 0, _sleep
LM0:
.stabn	0x44, 0, 8, LM0-_sleep
Lfunc_begin1:
LM1:
.stabn	0x44, 0, 9, LM1-_sleep
	banksel 0
btfss _B_Sleep, _B_Sleep
goto LBB0_2
LBB0_1:
LM2:
.stabn	0x44, 0, 11, LM2-_sleep
Ltmp1:
	clrf _INTE+0
LM3:
.stabn	0x44, 0, 13, LM3-_sleep
	clrf _SRADCON1+0
LM4:
.stabn	0x44, 0, 14, LM4-_sleep
	clrf _SRADCON2+0
LM5:
.stabn	0x44, 0, 16, LM5-_sleep
	clrf _PT1+0
LM6:
.stabn	0x44, 0, 17, LM6-_sleep
	movlw 0xf7
	movwf _PT1EN+0
LM7:
.stabn	0x44, 0, 18, LM7-_sleep
	movlw 8
	movwf _PT1PU+0
LM8:
.stabn	0x44, 0, 19, LM8-_sleep
	movlw 17
	movwf _PT1CON+0
LM9:
.stabn	0x44, 0, 21, LM9-_sleep
	clrf _PT3+0
LM10:
.stabn	0x44, 0, 22, LM10-_sleep
	movlw 0xff
	movwf _PT3EN+0
LM11:
.stabn	0x44, 0, 23, LM11-_sleep
	clrf _PT3PU+0
LM12:
.stabn	0x44, 0, 24, LM12-_sleep
	clrf _PT3CON+0
LM13:
.stabn	0x44, 0, 26, LM13-_sleep
	clrf _PT5+0
LM14:
.stabn	0x44, 0, 22, LM14-_sleep
	movlw 0xff
LM15:
.stabn	0x44, 0, 27, LM15-_sleep
	movwf _PT5EN+0
LM16:
.stabn	0x44, 0, 28, LM16-_sleep
	clrf _PT5PU+0
LM17:
.stabn	0x44, 0, 29, LM17-_sleep
	clrf _PT5CON+0
LM18:
.stabn	0x44, 0, 31, LM18-_sleep
	clrf _TM0CON+0
LM19:
.stabn	0x44, 0, 32, LM19-_sleep
	clrf _TM2CON+0
LM20:
.stabn	0x44, 0, 33, LM20-_sleep
	clrf _TM3CON+0
LM21:
.stabn	0x44, 0, 35, LM21-_sleep
	clrf _WDTCON+0
LM22:
.stabn	0x44, 0, 36, LM22-_sleep
	bsf _CST_WDT, _CST_WDT
LM23:
.stabn	0x44, 0, 38, LM23-_sleep
	clrf _INTF+0
LM24:
.stabn	0x44, 0, 39, LM24-_sleep
	clrf _INTF2+0
LM25:
.stabn	0x44, 0, 40, LM25-_sleep
	clrf _INTE2+0
LM26:
.stabn	0x44, 0, 41, LM26-_sleep
	movlw 0x82
	movwf _INTE+0
LM27:
.stabn	0x44, 0, 43, LM27-_sleep
	;APP
	sleep
	;NO_APP
LM28:
.stabn	0x44, 0, 44, LM28-_sleep
	;APP
	nop
	;NO_APP
LM29:
.stabn	0x44, 0, 45, LM29-_sleep
	;APP
	nop
	;NO_APP
LM30:
.stabn	0x44, 0, 46, LM30-_sleep
	;APP
	nop
	;NO_APP
Ltmp2:
LBB0_2:
LM31:
.stabn	0x44, 0, 51, LM31-_sleep
	return
Ltmp3:
Lfunc_end0:
Lfunc_end1:
LM32:
.stabf LM32-_sleep