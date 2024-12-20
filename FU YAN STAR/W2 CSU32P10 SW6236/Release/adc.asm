.stabs "D:\\Work_Area\\项目\\客户项目\\赋研星辰\\芯片资料\\芯海(CHIPSEA)\\CSU32P10_C_example\\",0x64,0,0,0
.stabs "D:\\Work_Area\\项目\\客户项目\\赋研星辰\\芯片资料\\芯海(CHIPSEA)\\CSU32P10_C_example\\adc.c",0x64,0,0,0
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
	_adc_sec .section rom
	.global	_adc
_adc:
Lfunc_begin0:
.stabs "adc:f:16", 0x24,0,0,_adc
.stabn 0xc0, 0, 0, _adc
.stabn 0xe0, 0, 0, _adc
LM0:
.stabn	0x44, 0, 7, LM0-_adc
Lfunc_begin1:
LM1:
.stabn	0x44, 0, 8, LM1-_adc
	banksel 0
btfss _if_adc, _if_adc
goto LBB0_2
LBB0_1:
LM2:
.stabn	0x44, 0, 10, LM2-_adc
Ltmp1:
	movlw 0x80
	movwf _SRADCON1+0
LM3:
.stabn	0x44, 0, 11, LM3-_adc
	movlw 16
	movwf _SRADCON2+0
LM4:
.stabn	0x44, 0, 12, LM4-_adc
	movlw 2
	movwf _PT3CON+0
LM5:
.stabn	0x44, 0, 13, LM5-_adc
	call _ad_fun
LM6:
.stabn	0x44, 0, 14, LM6-_adc
	call _ad_deal
LM7:
.stabn	0x44, 0, 15, LM7-_adc
	goto LBB0_8
Ltmp2:
LBB0_2:
LM8:
.stabn	0x44, 0, 16, LM8-_adc
btfss _if_adc1, _if_adc1
goto LBB0_4
LBB0_3:
LM9:
.stabn	0x44, 0, 18, LM9-_adc
Ltmp4:
	movlw 0x81
	movwf _SRADCON1+0
LM10:
.stabn	0x44, 0, 19, LM10-_adc
	movlw 96
	movwf _SRADCON2+0
LM11:
.stabn	0x44, 0, 20, LM11-_adc
	movlw 1
	movwf _PT3CON+0
LM12:
.stabn	0x44, 0, 21, LM12-_adc
	movlw 84
	movwf _METCH2+0
LM13:
.stabn	0x44, 0, 22, LM13-_adc
	call _ad_fun
LM14:
.stabn	0x44, 0, 23, LM14-_adc
	goto LBB0_7
Ltmp5:
LBB0_4:
LM15:
.stabn	0x44, 0, 30, LM15-_adc
btfss _if_adc2, _if_adc2
goto LBB0_6
LBB0_5:
LM16:
.stabn	0x44, 0, 32, LM16-_adc
Ltmp7:
	movlw 0x83
	movwf _SRADCON1+0
LM17:
.stabn	0x44, 0, 33, LM17-_adc
	movlw 0xf0
	movwf _SRADCON2+0
LM18:
.stabn	0x44, 0, 34, LM18-_adc
	movlw 2
	movwf _PT3CON+0
LM19:
.stabn	0x44, 0, 35, LM19-_adc
	movlw 84
	movwf _METCH2+0
LM20:
.stabn	0x44, 0, 36, LM20-_adc
	call _ad_fun
Ltmp8:
LBB0_6:
LBB0_7:
LBB0_8:
LM21:
.stabn	0x44, 0, 39, LM21-_adc
	return
Ltmp9:
Lfunc_end0:
Lfunc_end1:
LM22:
.stabf LM22-_adc
	_ad_fun_sec .section rom
	.global	_ad_fun
_ad_fun:
Lfunc_begin2:
.stabs "ad_fun:f:16", 0x24,0,0,_ad_fun
.stabn 0xc0, 0, 0, _ad_fun
.stabn 0xe0, 0, 0, _ad_fun
LM23:
.stabn	0x44, 0, 43, LM23-_ad_fun
Lfunc_begin3:
LM24:
.stabn	0x44, 0, 46, LM24-_ad_fun
	banksel 0
	bcf _OFFEX, _OFFEX
LM25:
.stabn	0x44, 0, 47, LM25-_ad_fun
Ltmp10:
LM26:
.stabn	0x44, 0, 49, LM26-_ad_fun
btfsc _if_adc2, _if_adc2
LBB1_1:
Ltmp12:
bsf	  _OFFEX, _OFFEX
Ltmp13:
LBB1_2:
LM27:
.stabn	0x44, 0, 53, LM27-_ad_fun
	clrf _AdData+0
	clrf _AdData+1
LM28:
.stabn	0x44, 0, 54, LM28-_ad_fun
	call _delay_10us
LM29:
.stabn	0x44, 0, 55, LM29-_ad_fun
Ltmp14:
	movlw 1
	movwf _ad_fun.temp.+0
	clrf _ad_fun.temp.+1
LBB1_3:
	movlw 1
	movwf _ad_fun.temp.+2
Ltmp15:
	movfw _ad_fun.temp.+0
Ltmp16:
	sublw 8
	blt LBB1_18
LBB1_17:
	movlw 0
	movwf _ad_fun.temp.+2
LBB1_18:
	movlw 1
	movwf _ad_fun.temp.+5
Ltmp17:
	movfw _ad_fun.temp.+1
Ltmp18:
	movwf _ad_fun.temp.+6
	xorlw 0x80
	sublw 0x80
	blt LBB1_20
LBB1_19:
	movlw 0
	movwf _ad_fun.temp.+5
LBB1_20:
	movfw _ad_fun.temp.+2
	movwf _ad_fun.temp.+7
	movfw _ad_fun.temp.+6
	sublw 0
	beq LBB1_22
LBB1_21:
	movfw _ad_fun.temp.+5
	movwf _ad_fun.temp.+7
LBB1_22:
	movfw _ad_fun.temp.+7
andlw 1
bne LBB1_11
LBB1_4:
LM30:
.stabn	0x44, 0, 57, LM30-_ad_fun
Ltmp19:
	bsf _SRADS, _SRADS
LM31:
.stabn	0x44, 0, 58, LM31-_ad_fun
	;APP
	nop
	;NO_APP
LBB1_5:
LM32:
.stabn	0x44, 0, 59, LM32-_ad_fun
btfss _SRADS, _SRADS
goto LBB1_7
LBB1_6:
	goto LBB1_5
LBB1_7:
LM33:
.stabn	0x44, 0, 60, LM33-_ad_fun
Ltmp20:
btfsc _if_adc2, _if_adc2
goto LBB1_9
LBB1_8:
LM34:
.stabn	0x44, 0, 62, LM34-_ad_fun
Ltmp22:
btfsc _OFFEX, _OFFEX 
goto $+3
bsf _OFFEX, _OFFEX
goto $+2
bcf _OFFEX, _OFFEX
Ltmp23:
LBB1_9:
LM35:
.stabn	0x44, 0, 64, LM35-_ad_fun
	movfw _SRADH+0
	movwf _ad_fun.temp.+3
	clrf _ad_fun.temp.+4
LM36:
.stabn	0x44, 0, 65, LM36-_ad_fun
	movfw _SRADL+0
	addwf _AdData+0, F
LM37:
.stabn	0x44, 0, 64, LM37-_ad_fun
	movfw _SRADH+0
LM38:
.stabn	0x44, 0, 65, LM38-_ad_fun
	addwfc _AdData+1, W
	movwf _AdData+1
Ltmp24:
LM39:
.stabn	0x44, 0, 55, LM39-_ad_fun
	movlw 1
	addwf _ad_fun.temp.+0, F
	movlw 0
	addwfc _ad_fun.temp.+1, W
	movwf _ad_fun.temp.+1
	goto LBB1_3
Ltmp25:
LBB1_11:
LM40:
.stabn	0x44, 0, 67, LM40-_ad_fun
	bcf _C, _C
LM41:
.stabn	0x44, 0, 68, LM41-_ad_fun
	movfw _AdData+0
	SrlThree
	movwf _ad_fun.temp.+2
	movfw _AdData+1
	ShlFive
	iorwf _ad_fun.temp.+2, W
	movwf _AdData+0
	movfw _AdData+1
	SrlThree
	movwf _AdData+1
LM42:
.stabn	0x44, 0, 71, LM42-_ad_fun
Ltmp26:
btfss _if_adc2, _if_adc2
goto LBB1_16
LBB1_12:
LM43:
.stabn	0x44, 0, 73, LM43-_ad_fun
Ltmp28:
	movlw 8
andwf _AdData+1,W
beq LBB1_14
LBB1_13:
LM44:
.stabn	0x44, 0, 75, LM44-_ad_fun
Ltmp29:
	movlw 0xf7
	andwf _AdData+1, W
	movwf _AdData+1
LM45:
.stabn	0x44, 0, 76, LM45-_ad_fun
	goto LBB1_15
Ltmp30:
LBB1_14:
LM46:
.stabn	0x44, 0, 79, LM46-_ad_fun
	movfw _AdData+0
	sublw 0
	movwf _AdData+0
	movfw _AdData+1
	sublwc 8
	movwf _AdData+1
Ltmp31:
LBB1_15:
LM47:
.stabn	0x44, 0, 81, LM47-_ad_fun
	bcf _C, _C
LM48:
.stabn	0x44, 0, 82, LM48-_ad_fun
	CSCC_SHIFT _AdData,0,_AdData,0,2,0,1,1
Ltmp32:
LBB1_16:
LM49:
.stabn	0x44, 0, 85, LM49-_ad_fun
	return
Ltmp33:
Lfunc_end2:
Lfunc_end3:
LM50:
.stabn 0xc0,0,0, LM24-_ad_fun
.stabs "temp:1",0x80,0,0,_ad_fun.temp.+3
.stabn 0xe0,0,0, LM49-_ad_fun
.stabn 0xc0,0,0, LM24-_ad_fun
.stabs "i:1",0x80,0,0,_ad_fun.temp.+0
.stabn 0xe0,0,0, LM49-_ad_fun
.stabf LM50-_ad_fun
	_ad_deal_sec .section rom
	.global	_ad_deal
_ad_deal:
Lfunc_begin4:
.stabs "ad_deal:f:16", 0x24,0,0,_ad_deal
.stabn 0xc0, 0, 0, _ad_deal
.stabn 0xe0, 0, 0, _ad_deal
LM51:
.stabn	0x44, 0, 88, LM51-_ad_deal
Lfunc_begin5:
LM52:
.stabn	0x44, 0, 89, LM52-_ad_deal
	banksel 0
	;APP
	movlw	TabStartAddr
	;NO_APP
LM53:
.stabn	0x44, 0, 90, LM53-_ad_deal
	;APP
	movwf	_EADRL
	;NO_APP
LM54:
.stabn	0x44, 0, 91, LM54-_ad_deal
	;APP
	movlw	.high(TabStartAddr)
	;NO_APP
LM55:
.stabn	0x44, 0, 92, LM55-_ad_deal
	;APP
	movwf	_EADRH
	;NO_APP
LM56:
.stabn	0x44, 0, 93, LM56-_ad_deal
	;APP
	movp
	;NO_APP
LM57:
.stabn	0x44, 0, 94, LM57-_ad_deal
	;APP
	nop
	;NO_APP
LM58:
.stabn	0x44, 0, 95, LM58-_ad_deal
	;APP
	movwf	_CurTabData
	;NO_APP
LM59:
.stabn	0x44, 0, 96, LM59-_ad_deal
	;APP
	movfw	_EDATH
	;NO_APP
LM60:
.stabn	0x44, 0, 97, LM60-_ad_deal
	;APP
	movwf	_CurTabData+1
	;NO_APP
LM61:
.stabn	0x44, 0, 99, LM61-_ad_deal
Ltmp34:
	CSCC_CMP_VAR _AdData,0,0,_CurTabData,0,0,6,0,16
Ltmp35:
andlw 1
bne LBB2_4
LBB2_1:
LM62:
.stabn	0x44, 0, 101, LM62-_ad_deal
Ltmp36:
	clrf _R_AdcStation2_Cnt+0
	clrf _R_AdcStation2_Cnt+1
LM63:
.stabn	0x44, 0, 102, LM63-_ad_deal
	clrf _R_AdcStation3_Cnt+0
	clrf _R_AdcStation3_Cnt+1
LM64:
.stabn	0x44, 0, 103, LM64-_ad_deal
	movlw 1
	addwf _R_AdcStation1_Cnt+0, F
	movlw 0
	addwfc _R_AdcStation1_Cnt+1, F
LM65:
.stabn	0x44, 0, 104, LM65-_ad_deal
Ltmp37:
	CSCC_CMP_CONST _R_AdcStation1_Cnt,20,0,0,0,9,0,16,0,0
Ltmp38:
andlw 1
bne LBB2_3
LBB2_2:
LM66:
.stabn	0x44, 0, 106, LM66-_ad_deal
Ltmp39:
	clrf _R_AdcStation1_Cnt+0
	clrf _R_AdcStation1_Cnt+1
LM67:
.stabn	0x44, 0, 107, LM67-_ad_deal
	bcf _PT1_5, _PT1_5
LM68:
.stabn	0x44, 0, 108, LM68-_ad_deal
	bcf _PT3_2, _PT3_2
Ltmp40:
LBB2_3:
LM69:
.stabn	0x44, 0, 110, LM69-_ad_deal
	goto LBB2_12
Ltmp41:
LBB2_4:
LM70:
.stabn	0x44, 0, 113, LM70-_ad_deal
	;APP
	movlw	1
	;NO_APP
LM71:
.stabn	0x44, 0, 114, LM71-_ad_deal
	;APP
	addwf	_EADRL,1
	;NO_APP
LM72:
.stabn	0x44, 0, 115, LM72-_ad_deal
	;APP
	clrf	_WORK
	;NO_APP
LM73:
.stabn	0x44, 0, 116, LM73-_ad_deal
	;APP
	addwfc _EADRH,1
	;NO_APP
LM74:
.stabn	0x44, 0, 117, LM74-_ad_deal
	;APP
	movp
	;NO_APP
LM75:
.stabn	0x44, 0, 118, LM75-_ad_deal
	;APP
	nop
	;NO_APP
LM76:
.stabn	0x44, 0, 119, LM76-_ad_deal
	;APP
	movwf	_CurTabData
	;NO_APP
LM77:
.stabn	0x44, 0, 120, LM77-_ad_deal
	;APP
	movfw	_EDATH
	;NO_APP
LM78:
.stabn	0x44, 0, 121, LM78-_ad_deal
	;APP
	movwf	_CurTabData+1
	;NO_APP
LM79:
.stabn	0x44, 0, 123, LM79-_ad_deal
Ltmp42:
	CSCC_CMP_VAR _AdData,0,0,_CurTabData,0,0,6,0,16
Ltmp43:
andlw 1
bne LBB2_8
LBB2_5:
LM80:
.stabn	0x44, 0, 125, LM80-_ad_deal
Ltmp44:
	clrf _R_AdcStation1_Cnt+0
	clrf _R_AdcStation1_Cnt+1
LM81:
.stabn	0x44, 0, 126, LM81-_ad_deal
	clrf _R_AdcStation3_Cnt+0
	clrf _R_AdcStation3_Cnt+1
LM82:
.stabn	0x44, 0, 127, LM82-_ad_deal
	movlw 1
	addwf _R_AdcStation2_Cnt+0, F
	movlw 0
	addwfc _R_AdcStation2_Cnt+1, F
LM83:
.stabn	0x44, 0, 128, LM83-_ad_deal
Ltmp45:
	CSCC_CMP_CONST _R_AdcStation2_Cnt,10,0,0,0,9,0,16,0,0
Ltmp46:
andlw 1
bne LBB2_7
LBB2_6:
LM84:
.stabn	0x44, 0, 130, LM84-_ad_deal
Ltmp47:
	clrf _R_AdcStation2_Cnt+0
	clrf _R_AdcStation2_Cnt+1
LM85:
.stabn	0x44, 0, 131, LM85-_ad_deal
	bsf _PT1_5, _PT1_5
LM86:
.stabn	0x44, 0, 132, LM86-_ad_deal
	bcf _PT3_2, _PT3_2
Ltmp48:
LBB2_7:
LM87:
.stabn	0x44, 0, 134, LM87-_ad_deal
	goto LBB2_11
Ltmp49:
LBB2_8:
LM88:
.stabn	0x44, 0, 137, LM88-_ad_deal
	clrf _R_AdcStation1_Cnt+0
	clrf _R_AdcStation1_Cnt+1
LM89:
.stabn	0x44, 0, 138, LM89-_ad_deal
	clrf _R_AdcStation2_Cnt+0
	clrf _R_AdcStation2_Cnt+1
LM90:
.stabn	0x44, 0, 139, LM90-_ad_deal
	movlw 1
	addwf _R_AdcStation3_Cnt+0, F
	movlw 0
	addwfc _R_AdcStation3_Cnt+1, F
LM91:
.stabn	0x44, 0, 140, LM91-_ad_deal
Ltmp50:
	CSCC_CMP_CONST _R_AdcStation3_Cnt,10,0,0,0,9,0,16,0,0
Ltmp51:
andlw 1
bne LBB2_10
LBB2_9:
LM92:
.stabn	0x44, 0, 142, LM92-_ad_deal
Ltmp52:
	clrf _R_AdcStation3_Cnt+0
	clrf _R_AdcStation3_Cnt+1
LM93:
.stabn	0x44, 0, 143, LM93-_ad_deal
	bcf _PT1_5, _PT1_5
LM94:
.stabn	0x44, 0, 144, LM94-_ad_deal
	bsf _PT3_2, _PT3_2
Ltmp53:
LBB2_10:
LBB2_11:
LBB2_12:
LM95:
.stabn	0x44, 0, 149, LM95-_ad_deal
	return
Ltmp54:
Lfunc_end4:
Lfunc_end5:
LM96:
.stabf LM96-_ad_deal
	.@_ad_fun_sec@_ad_fun.temp. .section bank0,uninit
	_ad_fun.temp. .ds 8
	.ends
