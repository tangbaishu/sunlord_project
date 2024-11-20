/**********************************
* @file csu32m10_timer0.c
* @brief
* @author
* @version V0.0.0
* @date 2019.04.01
* @details 
* @copyright
* @note 
**********************************/

/* Includes ------------------------------------------------------------------*/

#include "CSU32M10.h"
#include "csu32m10_timer0.h"

/* Fuction Declare ----------------------------------------------------------*/

void csu32m10_timer0_init(void);

/* Fuction Define -----------------------------------------------------------*/

/***********************************
* @fn  csu32m10_timer0_init(void)
* @brief 1ms count initialize .
* @param[out] none.
* @param[in] none.
* @return none.
************************************/

void csu32m10_timer0_init(void)
{
  timer0_set_cpuclk(); // set 4M cpuclk as CKT0
  timer0_clk_div_32(); // CKT0 32 div
  timer0_div_32_1ms(); // 1ms cnt
  timer0_int_flag_clr();  // clr timer0 interrupt flag
  timer0_int_enable(); // enable timer0 int
  timer0_count_clr();  // clr timer0 count
  timer0_count_start(); // timer0 start count
}


