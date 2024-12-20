.stabs "D:\\Work_Area\\项目\\客户项目\\赋研星辰\\芯片资料\\芯海(CHIPSEA)\\CSU32P10_C_example\\",0x64,0,0,0
.stabs "D:\\Work_Area\\项目\\客户项目\\赋研星辰\\芯片资料\\芯海(CHIPSEA)\\CSU32P10_C_example\\pwm.c",0x64,0,0,0
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
	_open_pwm_sec .section rom
	.global	_open_pwm
_open_pwm:
Lfunc_begin0:
.stabs "open_pwm:f:16", 0x24,0,0,_open_pwm
.stabn 0xc0, 0, 0, _open_pwm
.stabn 0xe0, 0, 0, _open_pwm
LM0:
.stabn	0x44, 0, 7, LM0-_open_pwm
Lfunc_begin1:
LM1:
.stabn	0x44, 0, 8, LM1-_open_pwm
	banksel 0
btfss _if_pwm, _if_pwm
goto LBB0_7
LBB0_1:
LM2:
.stabn	0x44, 0, 10, LM2-_open_pwm
Ltmp1:
btfss _B_Time500ms, _B_Time500ms
goto LBB0_5
LBB0_2:
LM3:
.stabn	0x44, 0, 12, LM3-_open_pwm
Ltmp3:
btfsc _B_pwm2, _B_pwm2
goto LBB0_4
LBB0_3:
LM4:
.stabn	0x44, 0, 14, LM4-_open_pwm
Ltmp5:
	bsf _B_pwm2, _B_pwm2
LM5:
.stabn	0x44, 0, 15, LM5-_open_pwm
	clrf _TM2CON+0
LM6:
.stabn	0x44, 0, 16, LM6-_open_pwm
	clrf _TM2RH+0
LM7:
.stabn	0x44, 0, 17, LM7-_open_pwm
	clrf _TM2INH+0
LM8:
.stabn	0x44, 0, 18, LM8-_open_pwm
	movlw 125
	movwf _TM2R+0
LM9:
.stabn	0x44, 0, 19, LM9-_open_pwm
	movlw 0xf9
	movwf _TM2IN+0
LM10:
.stabn	0x44, 0, 20, LM10-_open_pwm
	movlw 0x92
	movwf _TM2CON+0
Ltmp6:
LBB0_4:
LM11:
.stabn	0x44, 0, 22, LM11-_open_pwm
	goto LBB0_6
Ltmp7:
LBB0_5:
LM12:
.stabn	0x44, 0, 25, LM12-_open_pwm
	clrf _TM2CON+0
LM13:
.stabn	0x44, 0, 26, LM13-_open_pwm
	clrf _TM2R+0
LM14:
.stabn	0x44, 0, 27, LM14-_open_pwm
	clrf _TM2IN+0
LM15:
.stabn	0x44, 0, 28, LM15-_open_pwm
	bcf _B_pwm2, _B_pwm2
Ltmp8:
LBB0_6:
LM16:
.stabn	0x44, 0, 30, LM16-_open_pwm
	goto LBB0_10
Ltmp9:
LBB0_7:
LM17:
.stabn	0x44, 0, 33, LM17-_open_pwm
btfsc _B_pwm3, _B_pwm3
goto LBB0_9
LBB0_8:
LM18:
.stabn	0x44, 0, 35, LM18-_open_pwm
Ltmp11:
	bsf _B_pwm3, _B_pwm3
LM19:
.stabn	0x44, 0, 36, LM19-_open_pwm
	clrf _TM3CON+0
LM20:
.stabn	0x44, 0, 37, LM20-_open_pwm
	clrf _TM3RH+0
LM21:
.stabn	0x44, 0, 38, LM21-_open_pwm
	clrf _TM3INH+0
LM22:
.stabn	0x44, 0, 39, LM22-_open_pwm
	movlw 20
	movwf _TM3R+0
LM23:
.stabn	0x44, 0, 40, LM23-_open_pwm
	movlw 79
	movwf _TM3IN+0
LM24:
.stabn	0x44, 0, 41, LM24-_open_pwm
	movlw 4
	movwf _METCH2+0
LM25:
.stabn	0x44, 0, 42, LM25-_open_pwm
	movlw 39
	movwf _TM3CON2+0
LM26:
.stabn	0x44, 0, 43, LM26-_open_pwm
	movlw 0x80
	movwf _TM3CON+0
LM27:
.stabn	0x44, 0, 44, LM27-_open_pwm
	clrf _t10s_count+0
	clrf _t10s_count+1
Ltmp12:
LBB0_9:
LBB0_10:
LM28:
.stabn	0x44, 0, 48, LM28-_open_pwm
	return
Ltmp13:
Lfunc_end0:
Lfunc_end1:
LM29:
.stabf LM29-_open_pwm
