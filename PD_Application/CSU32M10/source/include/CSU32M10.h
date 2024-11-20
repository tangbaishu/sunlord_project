//==============================================
// V5 Special Register Address Definition
// Date:2019-04-18
// Version: V1.0.0 
//==============================================

#ifndef  _CSU32M10_H
#define  _CSU32M10_H 1

//Data Memory Organization 00h-08h
extern volatile unsigned char IND0             @ 0x00;     //{  }
extern volatile unsigned char FSR0             @ 0x02;     //{  }
extern volatile unsigned char STATUS           @ 0x04;     //{ LVD24, 0, 0, PD, TO, DC, C, Z }
extern volatile unsigned char WORK             @ 0x05;     //{  }
extern volatile unsigned char INTF             @ 0x06;     //{ 0, TM2IF, 0, TM0IF, SRADIF, 0, E1IF, E0IF }
extern volatile unsigned char INTE             @ 0x07;     //{ GIE, TM2IE, 0, TM0IE, SRADIE, 0, E1IE, E0IE }

//Peripheral special registers 09h-79h
extern volatile unsigned char RSTSR            @ 0x09;     //{ 0, 0, 0, 0, 0, EMCF, ILOPF, WWDTF }
extern volatile unsigned char EADRH            @ 0x0A;     //{ EADR[15:8] }
extern volatile unsigned char EADRL            @ 0x0B;     //{ EADR[7:0] }
extern volatile unsigned char EDATH            @ 0x0C;     //{ EDATH[7:0] }
extern volatile unsigned char WDTCON           @ 0x0D;     //{ WDTEN, 0, 0, 0, 0, WTS[2:0] }
extern volatile unsigned char WDTIN            @ 0x0E;     //{ WDTIN[7:0] }
extern volatile unsigned char TM0CON           @ 0x0F;     //{ T0EN, T0RATE[2:0], 0, T0RSTB, T0SEL[1:0] }
extern volatile unsigned char TM0IN            @ 0x10;     //{ TM0IN[7:0] }
extern volatile unsigned char TM0CNT           @ 0x11;     //{ TM0CNT[7:0] }
extern volatile unsigned char MCK              @ 0x16;     //{ 0, 0, CST_WDT, 0, 0, 0, 0, 0 }
extern volatile unsigned char TM2CON           @ 0x17;     //{ T2EN, T2RATE[2:0], T2CKS, T2RSTB, T2OUT, PWM2OUT }
extern volatile unsigned char TM2IN            @ 0x18;     //{ TM2IN[7:0] }
extern volatile unsigned char TM2CNT           @ 0x19;     //{ TM2CNT[7:0] }
extern volatile unsigned char TM2R             @ 0x1A;     //{ TM2R[7:0] }
extern volatile unsigned char TM3CON           @ 0x1B;     //{ T3EN, T3RATE[2:0], T3CKS, T3RSTB, T3OUT, PWM3OUT }
extern volatile unsigned char TM3IN            @ 0x1C;     //{ TM3IN[7:0] }
extern volatile unsigned char TM3CNT           @ 0x1D;     //{ TM3CNT[7:0] }
extern volatile unsigned char TM3R             @ 0x1E;     //{ TM3R[7:0] }
extern volatile unsigned char TM3INH           @ 0x1F;     //{ 0, 0, 0, 0, TM3INH[11:8] }
extern volatile unsigned char PT1              @ 0x20;     //{ 0, PT1[6:3], 0, PT1[1:0] }
extern volatile unsigned char PT1EN            @ 0x21;     //{ 0, PT1EN[6:3], 0, PT1EN[1:0] }
extern volatile unsigned char PT1PU            @ 0x22;     //{ 0, PT1PU[6:3], 0, PT1PU[1:0] }
extern volatile unsigned char PT1CON           @ 0x23;     //{ PT11OD, PT1W[3:0], E1M, E0M[1:0] }
extern volatile unsigned char TM2INH           @ 0x24;     //{ 0, 0, 0, 0, TM2IN[11:8] }
extern volatile unsigned char TM2CNTH          @ 0x25;     //{ 0, 0, 0, 0, TM2CNT[11:8] }
extern volatile unsigned char TM2RH            @ 0x26;     //{ 0, 0, 0, 0, TM2R[11:8] }
extern volatile unsigned char TM3CNTH          @ 0x27;     //{ 0, 0, 0, 0, TM3CNT[11:8] }
extern volatile unsigned char PT3              @ 0x28;     //{ 0, 0, PT3[5:0] }
extern volatile unsigned char PT3EN            @ 0x29;     //{ 0, 0, PT3EN[5:0] }
extern volatile unsigned char PT3PU            @ 0x2A;     //{ 0, 0, PT3PU[5:0] }
extern volatile unsigned char PT3CON           @ 0x2B;     //{ 0, 0, PT3CON[5:0] }
extern volatile unsigned char TM3RH            @ 0x2C;     //{ 0, 0, 0, 0, TM3R[11:8] }
extern volatile unsigned char TM3CON2          @ 0x2D;     //{ DT3CK[1:0], DT3CNT[2:0], DT3_EN, P3H_OEN, P3L_OEN }
extern volatile unsigned char METCH1           @ 0x2E;     //{ P3HINV, P3LINV, PT1W[6:4], PWM2PO, PWM2PO1, 0 }
extern volatile unsigned char METCH            @ 0x2F;     //{ VTHSEL, REF_SEL[2:0], PWMIS, T3RATE, T2RATE, P14_CUR }
extern volatile unsigned char PT5              @ 0x30;     //{ 0, 0, 0, 0, 0, 0, PT5[1:0] }
extern volatile unsigned char PT5EN            @ 0x31;     //{ 0, 0, 0, 0, 0, 0, PT5EN[1:0] }
extern volatile unsigned char PT5PU            @ 0x32;     //{ 0, 0, 0, 0, 0, 0, PT5PU[1:0] }
extern volatile unsigned char PT5CON           @ 0x33;     //{ 0, 0, 0, 0, 0, 0, PT51OD, PT50OD }
extern volatile unsigned char SRADCON0         @ 0x34;     //{ 0, 0, SRADACKS[1:0], 0, 0, SRADCKS[1:0] }
extern volatile unsigned char SRADCON1         @ 0x35;     //{ SRADEN, SRADS, OFTEN, CALIF, ENOV, 0, VREFS[1:0] }
extern volatile unsigned char SRADCON2         @ 0x36;     //{ CHS[3:0], 0, 0, 0, 0 }
extern volatile unsigned char SRADL            @ 0x37;     //{ SRAD[7:0] }
extern volatile unsigned char SRADH            @ 0x38;     //{ 0, 0, 0, 0, SRAD[11:8] }
extern volatile unsigned char SROFTL           @ 0x39;     //{ SROFT[7:0] }
extern volatile unsigned char SROFTH           @ 0x3A;     //{ 0, 0, 0, 0, SROFT[11:8] }
extern volatile unsigned char INTF2            @ 0x3C;     //{ 0, 0, 0, TM3IF, 0, 0, 0, 0 }
extern volatile unsigned char INTE2            @ 0x3D;     //{ 0, 0, 0, TM3IE, 0, 0, 0, 0 }
extern volatile unsigned char INTCFG           @ 0x40;     //{ LVD36, INTCFG_LVD24, 0, 0, 0, INTCFG[2:0] }
extern volatile unsigned char PT1CON1          @ 0x41;     //{ 0, 0, 0, 0, 0, 0, 0, PT1CON0 }
extern volatile unsigned char PT1PD            @ 0x42;     //{ 0, PT1PD[6:3], 0, PT1PD[1:0] }
extern volatile unsigned char PT3PD            @ 0x43;     //{ 0, 0, PT3PD[5:0] }
extern volatile unsigned char PT5PD            @ 0x44;     //{ 0, 0, 0, 0, 0, 0, PT5PD[1:0] }
extern volatile unsigned char TM3CON3          @ 0x45;     //{ PWM3PO, 0, 0, 0, P3H2INV, P3L2INV, P3H2OEN, P3L2OEN }
extern volatile unsigned char LCDCOM           @ 0x46;     //{ 0, 0, 0, LCDCOM[4:0] }
extern volatile unsigned char WWDTCR           @ 0x47;     //{ WWDTEN, TR[6:0] }
extern volatile unsigned char WWDTWR           @ 0x48;     //{ 0, WD[6:0] }
extern volatile unsigned char ISPCON1          @ 0x60;     //{ CHKRSLT, 0, 0, 0, 0, 0, 0, 0 }
extern volatile unsigned char WRPRT            @ 0x63;     //{ 0, 0, 0, 0, 0, 0, 0, WRPRTF }
extern volatile unsigned char CMPCON0          @ 0x6A;     //{ CMPEN, INSHTEN, HYSEN, CMP_PSEL, CMP_NSEL[2:0], CMPOUT0 }
extern volatile unsigned char CMPCON1          @ 0x6B;     //{ CMPOEN, CMPVTHEN, CMPVTHS[1:0], VTHOEN, CMP_FLT[2:0] }
extern volatile unsigned char CMPCON2          @ 0x6C;     //{ CMPENS[1:0], CMPINTS[1:0], 0, 0, TM3STP, TM2STP }
extern volatile unsigned char CCSCON           @ 0x6D;     //{ 0, 0, 0, 0, 0, 0, 0, CCSOEN }
extern volatile unsigned char METCH2           @ 0x79;     //{ 0, 0, 0, 0, METCH2[3:0] }

//-------------------------------------------------------
//  STATUS register bit map
//-------------------------------------------------------
extern volatile sbit Z                         @ 0x04*8+0;
extern volatile sbit C                         @ 0x04*8+1;
extern volatile sbit DC                        @ 0x04*8+2;
extern volatile sbit TO                        @ 0x04*8+3;
extern volatile sbit PD                        @ 0x04*8+4;
extern volatile sbit LVD24                     @ 0x04*8+7;

//-------------------------------------------------------
//  INTF register bit map
//-------------------------------------------------------
extern volatile sbit E0IF                      @ 0x06*8+0;
extern volatile sbit E1IF                      @ 0x06*8+1;
extern volatile sbit SRADIF                    @ 0x06*8+3;
extern volatile sbit TM0IF                     @ 0x06*8+4;
extern volatile sbit TM2IF                     @ 0x06*8+6;

//-------------------------------------------------------
//  INTE register bit map
//-------------------------------------------------------
extern volatile sbit E0IE                      @ 0x07*8+0;
extern volatile sbit E1IE                      @ 0x07*8+1;
extern volatile sbit SRADIE                    @ 0x07*8+3;
extern volatile sbit TM0IE                     @ 0x07*8+4;
extern volatile sbit TM2IE                     @ 0x07*8+6;
extern volatile sbit GIE                       @ 0x07*8+7;

//-------------------------------------------------------
//  RSTSR register bit map
//-------------------------------------------------------
extern volatile sbit WWDTF                     @ 0x09*8+0;
extern volatile sbit ILOPF                     @ 0x09*8+1;
extern volatile sbit EMCF                      @ 0x09*8+2;

//-------------------------------------------------------
//  EADRH register bit map
//-------------------------------------------------------
extern volatile sbit EADR_8                    @ 0x0A*8+0;
extern volatile sbit EADR_9                    @ 0x0A*8+1;
extern volatile sbit EADR_10                   @ 0x0A*8+2;
extern volatile sbit EADR_11                   @ 0x0A*8+3;
extern volatile sbit EADR_12                   @ 0x0A*8+4;
extern volatile sbit EADR_13                   @ 0x0A*8+5;
extern volatile sbit EADR_14                   @ 0x0A*8+6;
extern volatile sbit EADR_15                   @ 0x0A*8+7;

//-------------------------------------------------------
//  EADRL register bit map
//-------------------------------------------------------
extern volatile sbit EADR_0                    @ 0x0B*8+0;
extern volatile sbit EADR_1                    @ 0x0B*8+1;
extern volatile sbit EADR_2                    @ 0x0B*8+2;
extern volatile sbit EADR_3                    @ 0x0B*8+3;
extern volatile sbit EADR_4                    @ 0x0B*8+4;
extern volatile sbit EADR_5                    @ 0x0B*8+5;
extern volatile sbit EADR_6                    @ 0x0B*8+6;
extern volatile sbit EADR_7                    @ 0x0B*8+7;

//-------------------------------------------------------
//  EDATH register bit map
//-------------------------------------------------------
extern volatile sbit EDATH_0                   @ 0x0C*8+0;
extern volatile sbit EDATH_1                   @ 0x0C*8+1;
extern volatile sbit EDATH_2                   @ 0x0C*8+2;
extern volatile sbit EDATH_3                   @ 0x0C*8+3;
extern volatile sbit EDATH_4                   @ 0x0C*8+4;
extern volatile sbit EDATH_5                   @ 0x0C*8+5;
extern volatile sbit EDATH_6                   @ 0x0C*8+6;
extern volatile sbit EDATH_7                   @ 0x0C*8+7;

//-------------------------------------------------------
//  WDTCON register bit map
//-------------------------------------------------------
extern volatile sbit WTS_0                     @ 0x0D*8+0;
extern volatile sbit WTS_1                     @ 0x0D*8+1;
extern volatile sbit WTS_2                     @ 0x0D*8+2;
extern volatile sbit WDTEN                     @ 0x0D*8+7;

//-------------------------------------------------------
//  WDTIN register bit map
//-------------------------------------------------------
extern volatile sbit WDTIN_0                   @ 0x0E*8+0;
extern volatile sbit WDTIN_1                   @ 0x0E*8+1;
extern volatile sbit WDTIN_2                   @ 0x0E*8+2;
extern volatile sbit WDTIN_3                   @ 0x0E*8+3;
extern volatile sbit WDTIN_4                   @ 0x0E*8+4;
extern volatile sbit WDTIN_5                   @ 0x0E*8+5;
extern volatile sbit WDTIN_6                   @ 0x0E*8+6;
extern volatile sbit WDTIN_7                   @ 0x0E*8+7;

//-------------------------------------------------------
//  TM0CON register bit map
//-------------------------------------------------------
extern volatile sbit T0SEL_0                   @ 0x0F*8+0;
extern volatile sbit T0SEL_1                   @ 0x0F*8+1;
extern volatile sbit T0RSTB                    @ 0x0F*8+2;
extern volatile sbit T0RATE_0                  @ 0x0F*8+4;
extern volatile sbit T0RATE_1                  @ 0x0F*8+5;
extern volatile sbit T0RATE_2                  @ 0x0F*8+6;
extern volatile sbit T0EN                      @ 0x0F*8+7;

//-------------------------------------------------------
//  TM0IN register bit map
//-------------------------------------------------------
extern volatile sbit TM0IN_0                   @ 0x10*8+0;
extern volatile sbit TM0IN_1                   @ 0x10*8+1;
extern volatile sbit TM0IN_2                   @ 0x10*8+2;
extern volatile sbit TM0IN_3                   @ 0x10*8+3;
extern volatile sbit TM0IN_4                   @ 0x10*8+4;
extern volatile sbit TM0IN_5                   @ 0x10*8+5;
extern volatile sbit TM0IN_6                   @ 0x10*8+6;
extern volatile sbit TM0IN_7                   @ 0x10*8+7;

//-------------------------------------------------------
//  TM0CNT register bit map
//-------------------------------------------------------
extern volatile sbit TM0CNT_0                  @ 0x11*8+0;
extern volatile sbit TM0CNT_1                  @ 0x11*8+1;
extern volatile sbit TM0CNT_2                  @ 0x11*8+2;
extern volatile sbit TM0CNT_3                  @ 0x11*8+3;
extern volatile sbit TM0CNT_4                  @ 0x11*8+4;
extern volatile sbit TM0CNT_5                  @ 0x11*8+5;
extern volatile sbit TM0CNT_6                  @ 0x11*8+6;
extern volatile sbit TM0CNT_7                  @ 0x11*8+7;

//-------------------------------------------------------
//  MCK register bit map
//-------------------------------------------------------
extern volatile sbit CST_WDT                   @ 0x16*8+5;

//-------------------------------------------------------
//  TM2CON register bit map
//-------------------------------------------------------
extern volatile sbit PWM2OUT                   @ 0x17*8+0;
extern volatile sbit T2OUT                     @ 0x17*8+1;
extern volatile sbit T2RSTB                    @ 0x17*8+2;
extern volatile sbit T2CKS                     @ 0x17*8+3;
extern volatile sbit T2RATE_0                  @ 0x17*8+4;
extern volatile sbit T2RATE_1                  @ 0x17*8+5;
extern volatile sbit T2RATE_2                  @ 0x17*8+6;
extern volatile sbit T2EN                      @ 0x17*8+7;

//-------------------------------------------------------
//  TM2IN register bit map
//-------------------------------------------------------
extern volatile sbit TM2IN_0                   @ 0x18*8+0;
extern volatile sbit TM2IN_1                   @ 0x18*8+1;
extern volatile sbit TM2IN_2                   @ 0x18*8+2;
extern volatile sbit TM2IN_3                   @ 0x18*8+3;
extern volatile sbit TM2IN_4                   @ 0x18*8+4;
extern volatile sbit TM2IN_5                   @ 0x18*8+5;
extern volatile sbit TM2IN_6                   @ 0x18*8+6;
extern volatile sbit TM2IN_7                   @ 0x18*8+7;

//-------------------------------------------------------
//  TM2CNT register bit map
//-------------------------------------------------------
extern volatile sbit TM2CNT_0                  @ 0x19*8+0;
extern volatile sbit TM2CNT_1                  @ 0x19*8+1;
extern volatile sbit TM2CNT_2                  @ 0x19*8+2;
extern volatile sbit TM2CNT_3                  @ 0x19*8+3;
extern volatile sbit TM2CNT_4                  @ 0x19*8+4;
extern volatile sbit TM2CNT_5                  @ 0x19*8+5;
extern volatile sbit TM2CNT_6                  @ 0x19*8+6;
extern volatile sbit TM2CNT_7                  @ 0x19*8+7;

//-------------------------------------------------------
//  TM2R register bit map
//-------------------------------------------------------
extern volatile sbit TM2R_0                    @ 0x1A*8+0;
extern volatile sbit TM2R_1                    @ 0x1A*8+1;
extern volatile sbit TM2R_2                    @ 0x1A*8+2;
extern volatile sbit TM2R_3                    @ 0x1A*8+3;
extern volatile sbit TM2R_4                    @ 0x1A*8+4;
extern volatile sbit TM2R_5                    @ 0x1A*8+5;
extern volatile sbit TM2R_6                    @ 0x1A*8+6;
extern volatile sbit TM2R_7                    @ 0x1A*8+7;

//-------------------------------------------------------
//  TM3CON register bit map
//-------------------------------------------------------
extern volatile sbit PWM3OUT                   @ 0x1B*8+0;
extern volatile sbit T3OUT                     @ 0x1B*8+1;
extern volatile sbit T3RSTB                    @ 0x1B*8+2;
extern volatile sbit T3CKS                     @ 0x1B*8+3;
extern volatile sbit T3RATE_0                  @ 0x1B*8+4;
extern volatile sbit T3RATE_1                  @ 0x1B*8+5;
extern volatile sbit T3RATE_2                  @ 0x1B*8+6;
extern volatile sbit T3EN                      @ 0x1B*8+7;

//-------------------------------------------------------
//  TM3IN register bit map
//-------------------------------------------------------
extern volatile sbit TM3IN_0                   @ 0x1C*8+0;
extern volatile sbit TM3IN_1                   @ 0x1C*8+1;
extern volatile sbit TM3IN_2                   @ 0x1C*8+2;
extern volatile sbit TM3IN_3                   @ 0x1C*8+3;
extern volatile sbit TM3IN_4                   @ 0x1C*8+4;
extern volatile sbit TM3IN_5                   @ 0x1C*8+5;
extern volatile sbit TM3IN_6                   @ 0x1C*8+6;
extern volatile sbit TM3IN_7                   @ 0x1C*8+7;

//-------------------------------------------------------
//  TM3CNT register bit map
//-------------------------------------------------------
extern volatile sbit TM3CNT_0                  @ 0x1D*8+0;
extern volatile sbit TM3CNT_1                  @ 0x1D*8+1;
extern volatile sbit TM3CNT_2                  @ 0x1D*8+2;
extern volatile sbit TM3CNT_3                  @ 0x1D*8+3;
extern volatile sbit TM3CNT_4                  @ 0x1D*8+4;
extern volatile sbit TM3CNT_5                  @ 0x1D*8+5;
extern volatile sbit TM3CNT_6                  @ 0x1D*8+6;
extern volatile sbit TM3CNT_7                  @ 0x1D*8+7;

//-------------------------------------------------------
//  TM3R register bit map
//-------------------------------------------------------
extern volatile sbit TM3R_0                    @ 0x1E*8+0;
extern volatile sbit TM3R_1                    @ 0x1E*8+1;
extern volatile sbit TM3R_2                    @ 0x1E*8+2;
extern volatile sbit TM3R_3                    @ 0x1E*8+3;
extern volatile sbit TM3R_4                    @ 0x1E*8+4;
extern volatile sbit TM3R_5                    @ 0x1E*8+5;
extern volatile sbit TM3R_6                    @ 0x1E*8+6;
extern volatile sbit TM3R_7                    @ 0x1E*8+7;

//-------------------------------------------------------
//  TM3INH register bit map
//-------------------------------------------------------
extern volatile sbit TM3INH_8                  @ 0x1F*8+0;
extern volatile sbit TM3INH_9                  @ 0x1F*8+1;
extern volatile sbit TM3INH_10                 @ 0x1F*8+2;
extern volatile sbit TM3INH_11                 @ 0x1F*8+3;

//-------------------------------------------------------
//  PT1 register bit map
//-------------------------------------------------------
extern volatile sbit PT1_0                     @ 0x20*8+0;
extern volatile sbit PT1_1                     @ 0x20*8+1;
extern volatile sbit PT1_3                     @ 0x20*8+3;
extern volatile sbit PT1_4                     @ 0x20*8+4;
extern volatile sbit PT1_5                     @ 0x20*8+5;
extern volatile sbit PT1_6                     @ 0x20*8+6;

//-------------------------------------------------------
//  PT1EN register bit map
//-------------------------------------------------------
extern volatile sbit PT1EN_0                   @ 0x21*8+0;
extern volatile sbit PT1EN_1                   @ 0x21*8+1;
extern volatile sbit PT1EN_3                   @ 0x21*8+3;
extern volatile sbit PT1EN_4                   @ 0x21*8+4;
extern volatile sbit PT1EN_5                   @ 0x21*8+5;
extern volatile sbit PT1EN_6                   @ 0x21*8+6;

//-------------------------------------------------------
//  PT1PU register bit map
//-------------------------------------------------------
extern volatile sbit PT1PU_0                   @ 0x22*8+0;
extern volatile sbit PT1PU_1                   @ 0x22*8+1;
extern volatile sbit PT1PU_3                   @ 0x22*8+3;
extern volatile sbit PT1PU_4                   @ 0x22*8+4;
extern volatile sbit PT1PU_5                   @ 0x22*8+5;
extern volatile sbit PT1PU_6                   @ 0x22*8+6;

//-------------------------------------------------------
//  PT1CON register bit map
//-------------------------------------------------------
extern volatile sbit E0M_0                     @ 0x23*8+0;
extern volatile sbit E0M_1                     @ 0x23*8+1;
extern volatile sbit E1M                       @ 0x23*8+2;
extern volatile sbit PT1W_0                    @ 0x23*8+3;
extern volatile sbit PT1W_1                    @ 0x23*8+4;
extern volatile sbit PT1W_2                    @ 0x23*8+5;
extern volatile sbit PT1W_3                    @ 0x23*8+6;
extern volatile sbit PT11OD                    @ 0x23*8+7;

//-------------------------------------------------------
//  TM2INH register bit map
//-------------------------------------------------------
extern volatile sbit TM2IN_8                   @ 0x24*8+0;
extern volatile sbit TM2IN_9                   @ 0x24*8+1;
extern volatile sbit TM2IN_10                  @ 0x24*8+2;
extern volatile sbit TM2IN_11                  @ 0x24*8+3;

//-------------------------------------------------------
//  TM2CNTH register bit map
//-------------------------------------------------------
extern volatile sbit TM2CNT_8                  @ 0x25*8+0;
extern volatile sbit TM2CNT_9                  @ 0x25*8+1;
extern volatile sbit TM2CNT_10                 @ 0x25*8+2;
extern volatile sbit TM2CNT_11                 @ 0x25*8+3;

//-------------------------------------------------------
//  TM2RH register bit map
//-------------------------------------------------------
extern volatile sbit TM2R_8                    @ 0x26*8+0;
extern volatile sbit TM2R_9                    @ 0x26*8+1;
extern volatile sbit TM2R_10                   @ 0x26*8+2;
extern volatile sbit TM2R_11                   @ 0x26*8+3;

//-------------------------------------------------------
//  TM3CNTH register bit map
//-------------------------------------------------------
extern volatile sbit TM3CNT_8                  @ 0x27*8+0;
extern volatile sbit TM3CNT_9                  @ 0x27*8+1;
extern volatile sbit TM3CNT_10                 @ 0x27*8+2;
extern volatile sbit TM3CNT_11                 @ 0x27*8+3;

//-------------------------------------------------------
//  PT3 register bit map
//-------------------------------------------------------
extern volatile sbit PT3_0                     @ 0x28*8+0;
extern volatile sbit PT3_1                     @ 0x28*8+1;
extern volatile sbit PT3_2                     @ 0x28*8+2;
extern volatile sbit PT3_3                     @ 0x28*8+3;
extern volatile sbit PT3_4                     @ 0x28*8+4;
extern volatile sbit PT3_5                     @ 0x28*8+5;

//-------------------------------------------------------
//  PT3EN register bit map
//-------------------------------------------------------
extern volatile sbit PT3EN_0                   @ 0x29*8+0;
extern volatile sbit PT3EN_1                   @ 0x29*8+1;
extern volatile sbit PT3EN_2                   @ 0x29*8+2;
extern volatile sbit PT3EN_3                   @ 0x29*8+3;
extern volatile sbit PT3EN_4                   @ 0x29*8+4;
extern volatile sbit PT3EN_5                   @ 0x29*8+5;

//-------------------------------------------------------
//  PT3PU register bit map
//-------------------------------------------------------
extern volatile sbit PT3PU_0                   @ 0x2A*8+0;
extern volatile sbit PT3PU_1                   @ 0x2A*8+1;
extern volatile sbit PT3PU_2                   @ 0x2A*8+2;
extern volatile sbit PT3PU_3                   @ 0x2A*8+3;
extern volatile sbit PT3PU_4                   @ 0x2A*8+4;
extern volatile sbit PT3PU_5                   @ 0x2A*8+5;

//-------------------------------------------------------
//  PT3CON register bit map
//-------------------------------------------------------
extern volatile sbit PT3CON_0                  @ 0x2B*8+0;
extern volatile sbit PT3CON_1                  @ 0x2B*8+1;
extern volatile sbit PT3CON_2                  @ 0x2B*8+2;
extern volatile sbit PT3CON_3                  @ 0x2B*8+3;
extern volatile sbit PT3CON_4                  @ 0x2B*8+4;
extern volatile sbit PT3CON_5                  @ 0x2B*8+5;

//-------------------------------------------------------
//  TM3RH register bit map
//-------------------------------------------------------
extern volatile sbit TM3R_8                    @ 0x2C*8+0;
extern volatile sbit TM3R_9                    @ 0x2C*8+1;
extern volatile sbit TM3R_10                   @ 0x2C*8+2;
extern volatile sbit TM3R_11                   @ 0x2C*8+3;

//-------------------------------------------------------
//  TM3CON2 register bit map
//-------------------------------------------------------
extern volatile sbit P3L_OEN                   @ 0x2D*8+0;
extern volatile sbit P3H_OEN                   @ 0x2D*8+1;
extern volatile sbit DT3_EN                    @ 0x2D*8+2;
extern volatile sbit DT3CNT_0                  @ 0x2D*8+3;
extern volatile sbit DT3CNT_1                  @ 0x2D*8+4;
extern volatile sbit DT3CNT_2                  @ 0x2D*8+5;
extern volatile sbit DT3CK_0                   @ 0x2D*8+6;
extern volatile sbit DT3CK_1                   @ 0x2D*8+7;

//-------------------------------------------------------
//  METCH1 register bit map
//-------------------------------------------------------
extern volatile sbit PWM2PO1                   @ 0x2E*8+1;
extern volatile sbit PWM2PO                    @ 0x2E*8+2;
extern volatile sbit PT1W_4                    @ 0x2E*8+3;
extern volatile sbit PT1W_5                    @ 0x2E*8+4;
extern volatile sbit PT1W_6                    @ 0x2E*8+5;
extern volatile sbit P3LINV                    @ 0x2E*8+6;
extern volatile sbit P3HINV                    @ 0x2E*8+7;

//-------------------------------------------------------
//  METCH register bit map
//-------------------------------------------------------
extern volatile sbit P14_CUR                   @ 0x2F*8+0;
extern volatile sbit T2RATE_3                  @ 0x2F*8+1;
extern volatile sbit T3RATE_3                  @ 0x2F*8+2;
extern volatile sbit PWMIS                     @ 0x2F*8+3;
extern volatile sbit REF_SEL_0                 @ 0x2F*8+4;
extern volatile sbit REF_SEL_1                 @ 0x2F*8+5;
extern volatile sbit REF_SEL_2                 @ 0x2F*8+6;
extern volatile sbit VTHSEL                    @ 0x2F*8+7;

//-------------------------------------------------------
//  PT5 register bit map
//-------------------------------------------------------
extern volatile sbit PT5_0                     @ 0x30*8+0;
extern volatile sbit PT5_1                     @ 0x30*8+1;

//-------------------------------------------------------
//  PT5EN register bit map
//-------------------------------------------------------
extern volatile sbit PT5EN_0                   @ 0x31*8+0;
extern volatile sbit PT5EN_1                   @ 0x31*8+1;

//-------------------------------------------------------
//  PT5PU register bit map
//-------------------------------------------------------
extern volatile sbit PT5PU_0                   @ 0x32*8+0;
extern volatile sbit PT5PU_1                   @ 0x32*8+1;

//-------------------------------------------------------
//  PT5CON register bit map
//-------------------------------------------------------
extern volatile sbit PT50OD                    @ 0x33*8+0;
extern volatile sbit PT51OD                    @ 0x33*8+1;

//-------------------------------------------------------
//  SRADCON0 register bit map
//-------------------------------------------------------
extern volatile sbit SRADCKS_0                 @ 0x34*8+0;
extern volatile sbit SRADCKS_1                 @ 0x34*8+1;
extern volatile sbit SRADACKS_0                @ 0x34*8+4;
extern volatile sbit SRADACKS_1                @ 0x34*8+5;

//-------------------------------------------------------
//  SRADCON1 register bit map
//-------------------------------------------------------
extern volatile sbit VREFS_0                   @ 0x35*8+0;
extern volatile sbit VREFS_1                   @ 0x35*8+1;
extern volatile sbit ENOV                      @ 0x35*8+3;
extern volatile sbit CALIF                     @ 0x35*8+4;
extern volatile sbit OFTEN                     @ 0x35*8+5;
extern volatile sbit SRADS                     @ 0x35*8+6;
extern volatile sbit SRADEN                    @ 0x35*8+7;

//-------------------------------------------------------
//  SRADCON2 register bit map
//-------------------------------------------------------
extern volatile sbit CHS_0                     @ 0x36*8+4;
extern volatile sbit CHS_1                     @ 0x36*8+5;
extern volatile sbit CHS_2                     @ 0x36*8+6;
extern volatile sbit CHS_3                     @ 0x36*8+7;

//-------------------------------------------------------
//  SRADL register bit map
//-------------------------------------------------------
extern volatile sbit SRAD_0                    @ 0x37*8+0;
extern volatile sbit SRAD_1                    @ 0x37*8+1;
extern volatile sbit SRAD_2                    @ 0x37*8+2;
extern volatile sbit SRAD_3                    @ 0x37*8+3;
extern volatile sbit SRAD_4                    @ 0x37*8+4;
extern volatile sbit SRAD_5                    @ 0x37*8+5;
extern volatile sbit SRAD_6                    @ 0x37*8+6;
extern volatile sbit SRAD_7                    @ 0x37*8+7;

//-------------------------------------------------------
//  SRADH register bit map
//-------------------------------------------------------
extern volatile sbit SRAD_8                    @ 0x38*8+0;
extern volatile sbit SRAD_9                    @ 0x38*8+1;
extern volatile sbit SRAD_10                   @ 0x38*8+2;
extern volatile sbit SRAD_11                   @ 0x38*8+3;

//-------------------------------------------------------
//  SROFTL register bit map
//-------------------------------------------------------
extern volatile sbit SROFT_0                   @ 0x39*8+0;
extern volatile sbit SROFT_1                   @ 0x39*8+1;
extern volatile sbit SROFT_2                   @ 0x39*8+2;
extern volatile sbit SROFT_3                   @ 0x39*8+3;
extern volatile sbit SROFT_4                   @ 0x39*8+4;
extern volatile sbit SROFT_5                   @ 0x39*8+5;
extern volatile sbit SROFT_6                   @ 0x39*8+6;
extern volatile sbit SROFT_7                   @ 0x39*8+7;

//-------------------------------------------------------
//  SROFTH register bit map
//-------------------------------------------------------
extern volatile sbit SROFT_8                   @ 0x3A*8+0;
extern volatile sbit SROFT_9                   @ 0x3A*8+1;
extern volatile sbit SROFT_10                  @ 0x3A*8+2;
extern volatile sbit SROFT_11                  @ 0x3A*8+3;

//-------------------------------------------------------
//  INTF2 register bit map
//-------------------------------------------------------
extern volatile sbit TM3IF                     @ 0x3C*8+4;
extern volatile sbit CMPIF                     @ 0x3C*8+7;

//-------------------------------------------------------
//  INTE2 register bit map
//-------------------------------------------------------
extern volatile sbit TM3IE                     @ 0x3D*8+4;
extern volatile sbit CMPIE                     @ 0x3D*8+7;

//-------------------------------------------------------
//  INTCFG register bit map
//-------------------------------------------------------
extern volatile sbit INTCFG_0                  @ 0x40*8+0;
extern volatile sbit INTCFG_1                  @ 0x40*8+1;
extern volatile sbit INTCFG_2                  @ 0x40*8+2;
extern volatile sbit INTCFG_LVD24                @ 0x40*8+6;
extern volatile sbit LVD36                     @ 0x40*8+7;

//-------------------------------------------------------
//  PT1CON1 register bit map
//-------------------------------------------------------
extern volatile sbit PT1CON0                   @ 0x41*8+0;

//-------------------------------------------------------
//  PT1PD register bit map
//-------------------------------------------------------
extern volatile sbit PT1PD_0                   @ 0x42*8+0;
extern volatile sbit PT1PD_1                   @ 0x42*8+1;
extern volatile sbit PT1PD_3                   @ 0x42*8+3;
extern volatile sbit PT1PD_4                   @ 0x42*8+4;
extern volatile sbit PT1PD_5                   @ 0x42*8+5;
extern volatile sbit PT1PD_6                   @ 0x42*8+6;

//-------------------------------------------------------
//  PT3PD register bit map
//-------------------------------------------------------
extern volatile sbit PT3PD_0                   @ 0x43*8+0;
extern volatile sbit PT3PD_1                   @ 0x43*8+1;
extern volatile sbit PT3PD_2                   @ 0x43*8+2;
extern volatile sbit PT3PD_3                   @ 0x43*8+3;
extern volatile sbit PT3PD_4                   @ 0x43*8+4;
extern volatile sbit PT3PD_5                   @ 0x43*8+5;

//-------------------------------------------------------
//  PT5PD register bit map
//-------------------------------------------------------
extern volatile sbit PT5PD_0                   @ 0x44*8+0;
extern volatile sbit PT5PD_1                   @ 0x44*8+1;

//-------------------------------------------------------
//  TM3CON3 register bit map
//-------------------------------------------------------
extern volatile sbit P3L2OEN                   @ 0x45*8+0;
extern volatile sbit P3H2OEN                   @ 0x45*8+1;
extern volatile sbit P3L2INV                   @ 0x45*8+2;
extern volatile sbit P3H2INV                   @ 0x45*8+3;
extern volatile sbit PWM3PO                    @ 0x45*8+7;

//-------------------------------------------------------
//  LCDCOM register bit map
//-------------------------------------------------------
extern volatile sbit LCDCOM_0                  @ 0x46*8+0;
extern volatile sbit LCDCOM_1                  @ 0x46*8+1;
extern volatile sbit LCDCOM_2                  @ 0x46*8+2;
extern volatile sbit LCDCOM_3                  @ 0x46*8+3;
extern volatile sbit LCDCOM_4                  @ 0x46*8+4;

//-------------------------------------------------------
//  WWDTCR register bit map
//-------------------------------------------------------
extern volatile sbit TR_0                      @ 0x47*8+0;
extern volatile sbit TR_1                      @ 0x47*8+1;
extern volatile sbit TR_2                      @ 0x47*8+2;
extern volatile sbit TR_3                      @ 0x47*8+3;
extern volatile sbit TR_4                      @ 0x47*8+4;
extern volatile sbit TR_5                      @ 0x47*8+5;
extern volatile sbit TR_6                      @ 0x47*8+6;
extern volatile sbit WWDTEN                    @ 0x47*8+7;

//-------------------------------------------------------
//  WWDTWR register bit map
//-------------------------------------------------------
extern volatile sbit WD_0                      @ 0x48*8+0;
extern volatile sbit WD_1                      @ 0x48*8+1;
extern volatile sbit WD_2                      @ 0x48*8+2;
extern volatile sbit WD_3                      @ 0x48*8+3;
extern volatile sbit WD_4                      @ 0x48*8+4;
extern volatile sbit WD_5                      @ 0x48*8+5;
extern volatile sbit WD_6                      @ 0x48*8+6;

//-------------------------------------------------------
//  ISPCON1 register bit map
//-------------------------------------------------------
extern volatile sbit CHKRSLT                   @ 0x60*8+7;

//-------------------------------------------------------
//  WRPRT register bit map
//-------------------------------------------------------
extern volatile sbit WRPRTF                    @ 0x63*8+0;

//-------------------------------------------------------
//  CMPCON0 register bit map
//-------------------------------------------------------
extern volatile sbit CMPOUT0                   @ 0x6A*8+0;
extern volatile sbit CMP_NSEL_0                @ 0x6A*8+1;
extern volatile sbit CMP_NSEL_1                @ 0x6A*8+2;
extern volatile sbit CMP_NSEL_2                @ 0x6A*8+3;
extern volatile sbit CMP_PSEL                  @ 0x6A*8+4;
extern volatile sbit HYSEN                     @ 0x6A*8+5;
extern volatile sbit INSHTEN                   @ 0x6A*8+6;
extern volatile sbit CMPEN                     @ 0x6A*8+7;

//-------------------------------------------------------
//  CMPCON1 register bit map
//-------------------------------------------------------
extern volatile sbit CMP_FLT_0                 @ 0x6B*8+0;
extern volatile sbit CMP_FLT_1                 @ 0x6B*8+1;
extern volatile sbit CMP_FLT_2                 @ 0x6B*8+2;
extern volatile sbit VTHOEN                    @ 0x6B*8+3;
extern volatile sbit CMPVTHS_0                 @ 0x6B*8+4;
extern volatile sbit CMPVTHS_1                 @ 0x6B*8+5;
extern volatile sbit CMPVTHEN                  @ 0x6B*8+6;
extern volatile sbit CMPOEN                    @ 0x6B*8+7;

//-------------------------------------------------------
//  CMPCON2 register bit map
//-------------------------------------------------------
extern volatile sbit TM2STP                    @ 0x6C*8+0;
extern volatile sbit TM3STP                    @ 0x6C*8+1;
extern volatile sbit CMPINTS_0                 @ 0x6C*8+4;
extern volatile sbit CMPINTS_1                 @ 0x6C*8+5;
extern volatile sbit CMPENS_0                  @ 0x6C*8+6;
extern volatile sbit CMPENS_1                  @ 0x6C*8+7;

//-------------------------------------------------------
//  CCSCON register bit map
//-------------------------------------------------------
extern volatile sbit CCSOEN                    @ 0x6D*8+0;

//-------------------------------------------------------
//  METCH2 register bit map
//-------------------------------------------------------
extern volatile sbit METCH2_0                  @ 0x79*8+0;
extern volatile sbit METCH2_1                  @ 0x79*8+1;
extern volatile sbit METCH2_2                  @ 0x79*8+2;
extern volatile sbit METCH2_3                  @ 0x79*8+3;

#endif
