.stabs "D:\\Work_Area\\项目\\客户项目\\赋研星辰\\芯片资料\\芯海(CHIPSEA)\\CSU32P10_C_example\\",0x64,0,0,0
.stabs "D:\\Work_Area\\项目\\客户项目\\赋研星辰\\芯片资料\\芯海(CHIPSEA)\\CSU32P10_C_example\\delay.c",0x64,0,0,0
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
	_delay_sec .section rom
	.global	_delay
_delay:
Lfunc_begin0:
.stabs "delay:f:16", 0x24,0,0,_delay
.stabn 0xc0, 0, 0, _delay
.stabn 0xe0, 0, 0, _delay
LM0:
.stabn	0x44, 0, 5, LM0-_delay
Lfunc_begin1:
LM1:
.stabn	0x44, 0, 7, LM1-_delay
	banksel 0
	clrf _delay.temp.+0
	clrf _delay.temp.+1
LBB0_1:
	movlw 1
	movwf _delay.temp.+2
Ltmp0:
	movfw _delay.temp.+0
Ltmp1:
	sublw 0x87
	blt LBB0_6
LBB0_5:
	movlw 0
	movwf _delay.temp.+2
LBB0_6:
	movlw 1
	movwf _delay.temp.+3
Ltmp2:
	movfw _delay.temp.+1
Ltmp3:
	movwf _delay.temp.+4
	xorlw 0x80
	sublw 0x93
	blt LBB0_8
LBB0_7:
	movlw 0
	movwf _delay.temp.+3
LBB0_8:
	movfw _delay.temp.+2
	movwf _delay.temp.+5
	movfw _delay.temp.+4
	sublw 19
	beq LBB0_10
LBB0_9:
	movfw _delay.temp.+3
	movwf _delay.temp.+5
LBB0_10:
	movfw _delay.temp.+5
andlw 1
bne LBB0_4
LBB0_2:
LM2:
.stabn	0x44, 0, 9, LM2-_delay
Ltmp4:
	;APP
	nop
	;NO_APP
Ltmp5:
LM3:
.stabn	0x44, 0, 7, LM3-_delay
	movlw 1
	addwf _delay.temp.+0, F
	movlw 0
	addwfc _delay.temp.+1, W
	movwf _delay.temp.+1
	goto LBB0_1
Ltmp6:
LBB0_4:
LM4:
.stabn	0x44, 0, 11, LM4-_delay
	return
Ltmp7:
Lfunc_end0:
Lfunc_end1:
LM5:
.stabn 0xc0,0,0, LM1-_delay
.stabs "i:1",0x80,0,0,_delay.temp.+0
.stabn 0xe0,0,0, LM4-_delay
.stabf LM5-_delay
	_delay_10us_sec .section rom
	.global	_delay_10us
_delay_10us:
Lfunc_begin2:
.stabs "delay_10us:f:16", 0x24,0,0,_delay_10us
.stabn 0xc0, 0, 0, _delay_10us
.stabn 0xe0, 0, 0, _delay_10us
LM6:
.stabn	0x44, 0, 13, LM6-_delay_10us
Lfunc_begin3:
LM7:
.stabn	0x44, 0, 15, LM7-_delay_10us
	banksel 0
	clrf _delay_10us.temp.+0
	clrf _delay_10us.temp.+1
LBB1_1:
	movlw 1
	movwf _delay_10us.temp.+2
Ltmp8:
	movfw _delay_10us.temp.+0
Ltmp9:
	sublw 0
	bne LBB1_6
LBB1_5:
	movlw 0
	movwf _delay_10us.temp.+2
LBB1_6:
	movlw 1
	movwf _delay_10us.temp.+3
Ltmp10:
	movfw _delay_10us.temp.+1
Ltmp11:
	movwf _delay_10us.temp.+4
	xorlw 0x80
	sublw 0x80
	blt LBB1_8
LBB1_7:
	movlw 0
	movwf _delay_10us.temp.+3
LBB1_8:
	movfw _delay_10us.temp.+2
	movwf _delay_10us.temp.+5
	movfw _delay_10us.temp.+4
	sublw 0
	beq LBB1_10
LBB1_9:
	movfw _delay_10us.temp.+3
	movwf _delay_10us.temp.+5
LBB1_10:
	movfw _delay_10us.temp.+5
andlw 1
bne LBB1_4
LBB1_2:
LM8:
.stabn	0x44, 0, 17, LM8-_delay_10us
Ltmp12:
	;APP
	nop
	;NO_APP
Ltmp13:
LM9:
.stabn	0x44, 0, 15, LM9-_delay_10us
	movlw 1
	addwf _delay_10us.temp.+0, F
	movlw 0
	addwfc _delay_10us.temp.+1, W
	movwf _delay_10us.temp.+1
	goto LBB1_1
Ltmp14:
LBB1_4:
LM10:
.stabn	0x44, 0, 19, LM10-_delay_10us
	return
Ltmp15:
Lfunc_end2:
Lfunc_end3:
LM11:
.stabn 0xc0,0,0, LM7-_delay_10us
.stabs "j:1",0x80,0,0,_delay_10us.temp.+0
.stabn 0xe0,0,0, LM10-_delay_10us
.stabf LM11-_delay_10us
	.@_delay_sec@_delay.temp. .section bank0,uninit
	_delay.temp. .ds 6
	.ends
	.@_delay_10us_sec@_delay_10us.temp. .section bank0,uninit
	_delay_10us.temp. .ds 6
	.ends
