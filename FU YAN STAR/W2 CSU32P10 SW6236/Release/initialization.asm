.stabs "D:\\Work_Area\\项目\\客户项目\\赋研星辰\\芯片资料\\芯海(CHIPSEA)\\CSU32P10_C_example\\",0x64,0,0,0
.stabs "D:\\Work_Area\\项目\\客户项目\\赋研星辰\\芯片资料\\芯海(CHIPSEA)\\CSU32P10_C_example\\initialization.c",0x64,0,0,0
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
	_MCU_Initialization_sec .section rom
	.global	_MCU_Initialization
_MCU_Initialization:
Lfunc_begin0:
.stabs "MCU_Initialization:f:16", 0x24,0,0,_MCU_Initialization
.stabn 0xc0, 0, 0, _MCU_Initialization
.stabn 0xe0, 0, 0, _MCU_Initialization
LM0:
.stabn	0x44, 0, 13, LM0-_MCU_Initialization
Lfunc_begin1:
LM1:
.stabn	0x44, 0, 19, LM1-_MCU_Initialization
	banksel 0
	;APP
	clrf _STATUS
	;NO_APP
LM2:
.stabn	0x44, 0, 24, LM2-_MCU_Initialization
	clrf _MCK+0
LM3:
.stabn	0x44, 0, 35, LM3-_MCU_Initialization
	movlw 0x81
	movwf _WDTCON+0
LM4:
.stabn	0x44, 0, 50, LM4-_MCU_Initialization
	movlw 0xff
	movwf _WDTIN+0
LM5:
.stabn	0x44, 0, 64, LM5-_MCU_Initialization
	clrf _PT1+0
LM6:
.stabn	0x44, 0, 66, LM6-_MCU_Initialization
	movlw 48
	movwf _PT1EN+0
LM7:
.stabn	0x44, 0, 69, LM7-_MCU_Initialization
	clrf _PT1PU+0
LM8:
.stabn	0x44, 0, 72, LM8-_MCU_Initialization
	movlw 1
	movwf _PT1CON+0
LM9:
.stabn	0x44, 0, 107, LM9-_MCU_Initialization
	clrf _PT3+0
LM10:
.stabn	0x44, 0, 109, LM10-_MCU_Initialization
	movlw 4
	movwf _PT3EN+0
LM11:
.stabn	0x44, 0, 131, LM11-_MCU_Initialization
	clrf _PT3PU+0
LM12:
.stabn	0x44, 0, 139, LM12-_MCU_Initialization
	clrf _PT3CON+0
LM13:
.stabn	0x44, 0, 150, LM13-_MCU_Initialization
	clrf _PT5+0
LM14:
.stabn	0x44, 0, 152, LM14-_MCU_Initialization
	movlw 3
	movwf _PT5EN+0
LM15:
.stabn	0x44, 0, 156, LM15-_MCU_Initialization
	clrf _PT5PU+0
LM16:
.stabn	0x44, 0, 161, LM16-_MCU_Initialization
	clrf _PT5CON+0
LM17:
.stabn	0x44, 0, 72, LM17-_MCU_Initialization
	movlw 1
LM18:
.stabn	0x44, 0, 169, LM18-_MCU_Initialization
	movwf _SRADCON0+0
LM19:
.stabn	0x44, 0, 185, LM19-_MCU_Initialization
	clrf _SRADCON1+0
LM20:
.stabn	0x44, 0, 219, LM20-_MCU_Initialization
	clrf _SRADCON2+0
LM21:
.stabn	0x44, 0, 244, LM21-_MCU_Initialization
	clrf _SRADL+0
LM22:
.stabn	0x44, 0, 245, LM22-_MCU_Initialization
	clrf _SRADH+0
LM23:
.stabn	0x44, 0, 249, LM23-_MCU_Initialization
	movlw 0xf0
	movwf _TM0CON+0
LM24:
.stabn	0x44, 0, 276, LM24-_MCU_Initialization
	movlw 124
	movwf _TM0IN+0
LM25:
.stabn	0x44, 0, 280, LM25-_MCU_Initialization
	clrf _TM0CNT+0
LM26:
.stabn	0x44, 0, 285, LM26-_MCU_Initialization
	clrf _TM2CON+0
LM27:
.stabn	0x44, 0, 324, LM27-_MCU_Initialization
	clrf _TM2IN+0
LM28:
.stabn	0x44, 0, 325, LM28-_MCU_Initialization
	clrf _TM2INH+0
LM29:
.stabn	0x44, 0, 328, LM29-_MCU_Initialization
	clrf _TM2R+0
LM30:
.stabn	0x44, 0, 329, LM30-_MCU_Initialization
	clrf _TM2RH+0
LM31:
.stabn	0x44, 0, 331, LM31-_MCU_Initialization
	clrf _TM2CNT+0
LM32:
.stabn	0x44, 0, 332, LM32-_MCU_Initialization
	clrf _TM2CNTH+0
LM33:
.stabn	0x44, 0, 349, LM33-_MCU_Initialization
	clrf _TM3CON+0
LM34:
.stabn	0x44, 0, 388, LM34-_MCU_Initialization
	clrf _TM3IN+0
LM35:
.stabn	0x44, 0, 389, LM35-_MCU_Initialization
	clrf _TM3INH+0
LM36:
.stabn	0x44, 0, 393, LM36-_MCU_Initialization
	clrf _TM3R+0
LM37:
.stabn	0x44, 0, 394, LM37-_MCU_Initialization
	clrf _TM3RH+0
LM38:
.stabn	0x44, 0, 396, LM38-_MCU_Initialization
	clrf _TM3CNT+0
LM39:
.stabn	0x44, 0, 397, LM39-_MCU_Initialization
	clrf _TM3CNTH+0
LM40:
.stabn	0x44, 0, 414, LM40-_MCU_Initialization
	clrf _TM3CON2+0
LM41:
.stabn	0x44, 0, 441, LM41-_MCU_Initialization
	movlw 0x90
	movwf _INTE+0
LM42:
.stabn	0x44, 0, 467, LM42-_MCU_Initialization
	clrf _INTE2+0
LM43:
.stabn	0x44, 0, 473, LM43-_MCU_Initialization
	clrf _INTF+0
LM44:
.stabn	0x44, 0, 495, LM44-_MCU_Initialization
	clrf _INTF2+0
LM45:
.stabn	0x44, 0, 501, LM45-_MCU_Initialization
	return
Ltmp0:
Lfunc_end0:
Lfunc_end1:
LM46:
.stabf LM46-_MCU_Initialization
