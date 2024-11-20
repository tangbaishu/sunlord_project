/**********************************
* @file csu32m10_timer0.h
* @brief
* @author
* @version V0.0.0
* @date 2019.04.01
* @details 
* @copyright
* @note 
**********************************/

#ifndef  _CSU32M10_TIMER0_H
#define  _CSU32M10_TIMER0_H 

/* Includes 
------------------------------------------------------------------*/
#include "CSU32M10.h"

/* Fuction Define -----------------------------------------------------------*/

/**
* @def TIMER0_DIV32_VALUE_1MS
* @brief define timer0 1ms cnt
* Details.
*/
#define TIMER0_DIV32_VALUE_1MS    124 // 0.001*(4M/32)-1=124

/**
* @def CNT_8MS
* @brief 8ms cnt
* Details.
*/
#define CNT_8MS   8

/* Fuction Define -----------------------------------------------------------*/

/**
* @def timer0_set_cpuclk()  
* @brief set timer0 clk
* Details.
*/
#define timer0_set_cpuclk()       {TM0CON &= 0xFC;}

/**
* @def timer0_clk_div_32() 
* @brief set timer0 clk div 32
* Details.
*/
#define timer0_clk_div_32()       {T0RATE_0 = 1;T0RATE_1 = 0;T0RATE_2 = 1;}

/**
* @def timer0_div_32_1ms()  
* @brief set timer0 1ms cnt
* Details.
*/
#define timer0_div_32_1ms()       {TM0IN = TIMER0_DIV32_VALUE_1MS;}

/**
* @def timer0_int_flag_clr()  
* @brief clr timer0 int flag
* Details.
*/
#define timer0_int_flag_clr()     {TM0IF = 0;}

/**
* @def timer0_int_enable() 
* @brief enable timer0 int flag
* Details.
*/
#define timer0_int_enable()       {TM0IE = 1;GIE = 1;}

/**
* @def timer0_count_clr() 
* @brief clr timer0 count
* Details.
*/
#define timer0_count_clr()        {T0RSTB = 0;}

/**
* @def timer0_count_start() 
* @brief timer0 count start
* Details.
*/
#define timer0_count_start()      {T0EN = 1;}

/* Exported functions 
--------------------------------------------------------*/

extern void csu32m10_timer0_init(void);



#endif