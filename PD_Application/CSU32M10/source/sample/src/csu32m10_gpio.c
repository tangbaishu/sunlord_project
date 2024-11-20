/**********************************
* @file csu32m10_gpio.c
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
#include "cs32m10_gpio.h"

/* Fuction Declare ----------------------------------------------------------*/

/**********************************
* @fn  csu32p20_gpio_demo(void)
* @brief keep PT1_0 level the same as PT1_1.
* @param[out] none.
* @param[in] none.
* @return none.
**********************************/
void csu32m10_gpio_demo(void);

/* Fuction Define -----------------------------------------------------------*/

/**********************************
* @fn  csu32p20_gpio_demo(void)
* @brief keep PT1_0 level the same as PT1_1.
* @param[out] none.
* @param[in] none.
* @return none.
**********************************/

void csu32m10_gpio_demo(void)
{
   if(PT1_1 == 1)
   {
     PT1_0 = 1;
   }
   else
   {
     PT1_0 = 0;  
   }   
}
