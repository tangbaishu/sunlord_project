.stabs "D:\\Work_Area\\项目\\客户项目\\赋研星辰\\芯片资料\\芯海(CHIPSEA)\\CSU32P10_C_example\\",0x64,0,0,0
.stabs "D:\\Work_Area\\项目\\客户项目\\赋研星辰\\芯片资料\\芯海(CHIPSEA)\\CSU32P10_C_example\\main.c",0x64,0,0,0
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
	_main_sec .section rom
	.global	_main
_main:
Lfunc_begin0:
.stabs "main:f:16", 0x24,0,0,_main
.stabn 0xc0, 0, 0, _main
.stabn 0xe0, 0, 0, _main
LM0:
.stabn	0x44, 0, 38, LM0-_main
Lfunc_begin1:
LM1:
.stabn	0x44, 0, 39, LM1-_main
	banksel 0
	;APP
	clrwdt
	;NO_APP
LM2:
.stabn	0x44, 0, 40, LM2-_main
	call _delay
LM3:
.stabn	0x44, 0, 41, LM3-_main
	call _MCU_Initialization
LM4:
.stabn	0x44, 0, 42, LM4-_main
	call _Bit_Initialization
LBB0_1:
LM5:
.stabn	0x44, 0, 46, LM5-_main
Ltmp0:
	;APP
	clrwdt
	;NO_APP
LM6:
.stabn	0x44, 0, 47, LM6-_main
	call _adc
LM7:
.stabn	0x44, 0, 48, LM7-_main
	call _open_pwm
LM8:
.stabn	0x44, 0, 49, LM8-_main
	call _sleep
Ltmp1:
LM9:
.stabn	0x44, 0, 44, LM9-_main
	goto LBB0_1
Ltmp2:
Lfunc_end0:
Lfunc_end1:
LM10:
.stabf LM10-_main
	_INT_FUNCTION_sec .section rom,interrupt
	.global	_INT_FUNCTION
_INT_FUNCTION:
Lfunc_begin2:
.stabs "INT_FUNCTION:f:16", 0x24,0,0,_INT_FUNCTION
.stabn 0xc0, 0, 0, _INT_FUNCTION
.stabn 0xe0, 0, 0, _INT_FUNCTION
LM11:
.stabn	0x44, 0, 60, LM11-_INT_FUNCTION
Lfunc_begin3:
	push
LM12:
.stabn	0x44, 0, 61, LM12-_INT_FUNCTION
Ltmp3:
	banksel 0
btfss _E0IF, _E0IF
goto LBB1_2
LBB1_1:
LM13:
.stabn	0x44, 0, 63, LM13-_INT_FUNCTION
Ltmp5:
	bcf _E0IF, _E0IF
LM14:
.stabn	0x44, 0, 64, LM14-_INT_FUNCTION
	goto LBB1_19
Ltmp6:
LBB1_2:
LM15:
.stabn	0x44, 0, 66, LM15-_INT_FUNCTION
btfss _E1IF, _E1IF
goto LBB1_4
LBB1_3:
LM16:
.stabn	0x44, 0, 68, LM16-_INT_FUNCTION
Ltmp8:
	bcf _E1IF, _E1IF
LM17:
.stabn	0x44, 0, 69, LM17-_INT_FUNCTION
	goto LBB1_18
Ltmp9:
LBB1_4:
LM18:
.stabn	0x44, 0, 71, LM18-_INT_FUNCTION
btfss _TM0IF, _TM0IF
goto LBB1_11
LBB1_5:
LM19:
.stabn	0x44, 0, 73, LM19-_INT_FUNCTION
Ltmp11:
	bcf _TM0IF, _TM0IF
LM20:
.stabn	0x44, 0, 74, LM20-_INT_FUNCTION
	movlw 1
	addwf _t1s_count+0, F
	movlw 0
	addwfc _t1s_count+1, F
LM21:
.stabn	0x44, 0, 75, LM21-_INT_FUNCTION
Ltmp12:
	CSCC_CMP_CONST _t1s_count,125,0,0,0,9,0,16,0,0
Ltmp13:
andlw 1
bne LBB1_10
LBB1_6:
LM22:
.stabn	0x44, 0, 77, LM22-_INT_FUNCTION
Ltmp14:
	clrf _t1s_count+0
	clrf _t1s_count+1
LM23:
.stabn	0x44, 0, 78, LM23-_INT_FUNCTION
btfsc _PT1_4, _PT1_4 
goto $+3
bsf _PT1_4, _PT1_4
goto $+2
bcf _PT1_4, _PT1_4
LM24:
.stabn	0x44, 0, 79, LM24-_INT_FUNCTION
Ltmp15:
	CSCC_CMP_CONST _t10s_count,60,0,0,0,9,0,16,0,0
Ltmp16:
andlw 1
bne LBB1_8
LBB1_7:
LM25:
.stabn	0x44, 0, 81, LM25-_INT_FUNCTION
Ltmp17:
	clrf _TM3CON+0
LM26:
.stabn	0x44, 0, 82, LM26-_INT_FUNCTION
	clrf _TM3CON2+0
LM27:
.stabn	0x44, 0, 83, LM27-_INT_FUNCTION
	bcf _B_Time500ms, _B_Time500ms
LM28:
.stabn	0x44, 0, 84, LM28-_INT_FUNCTION
	bsf _B_Sleep, _B_Sleep
LM29:
.stabn	0x44, 0, 85, LM29-_INT_FUNCTION
	goto LBB1_9
Ltmp18:
LBB1_8:
LM30:
.stabn	0x44, 0, 88, LM30-_INT_FUNCTION
	movlw 1
	addwf _t10s_count+0, F
	movlw 0
	addwfc _t10s_count+1, W
	movwf _t10s_count+1
LM31:
.stabn	0x44, 0, 89, LM31-_INT_FUNCTION
btfsc _B_Time500ms, _B_Time500ms 
goto $+3
bsf _B_Time500ms, _B_Time500ms
goto $+2
bcf _B_Time500ms, _B_Time500ms
Ltmp19:
LBB1_9:
LBB1_10:
LM32:
.stabn	0x44, 0, 92, LM32-_INT_FUNCTION
	goto LBB1_17
Ltmp20:
LBB1_11:
LM33:
.stabn	0x44, 0, 94, LM33-_INT_FUNCTION
btfss _TM2IF, _TM2IF
goto LBB1_13
LBB1_12:
LM34:
.stabn	0x44, 0, 96, LM34-_INT_FUNCTION
Ltmp22:
	bcf _TM2IF, _TM2IF
LM35:
.stabn	0x44, 0, 97, LM35-_INT_FUNCTION
	goto LBB1_16
Ltmp23:
LBB1_13:
LM36:
.stabn	0x44, 0, 99, LM36-_INT_FUNCTION
btfss _TM3IF, _TM3IF
goto LBB1_15
LBB1_14:
LM37:
.stabn	0x44, 0, 101, LM37-_INT_FUNCTION
Ltmp25:
	bcf _TM3IF, _TM3IF
LBB1_15:
LBB1_16:
LBB1_17:
LBB1_18:
LBB1_19:
LM38:
.stabn	0x44, 0, 102, LM38-_INT_FUNCTION
	pop
Ltmp26:
LM39:
.stabn	0x44, 0, 104, LM39-_INT_FUNCTION
	retfie
Ltmp27:
Lfunc_end2:
Lfunc_end3:
LM40:
.stabf LM40-_INT_FUNCTION