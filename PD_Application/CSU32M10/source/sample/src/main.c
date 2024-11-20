/**********************************
 * @file main.c
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
/* Fuction Define -----------------------------------------------------------*/

/**********************************
 * @fn  void INT_FUNCTION(void) interrupt
 * @brief interrupt service funcion.
 * @param[out] none.
 * @param[in]  none.
 * @return none.
 **********************************/

void INT_FUNCTION(void) interrupt
{
}

/**********************************
 * @fn void main()
 * @brief main funcion.
 * @param[out] none.
 * @param[in] none.
 * @return none.
 **********************************/

void main()
{
    csu32m10_timer0_init();
    PT1EN_1 = 0; // set PT1_1 input mode
    PT1PU_1 = 1; // enable pull up
    PT1PD_1 = 0;
    PT1EN_0 = 1; // set PT1_0 output mode
    while (1)
    {
        csu32m10_gpio_demo();
    }
}
