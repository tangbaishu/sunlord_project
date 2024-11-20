/**********************************
* @file csu32p20_adc.h
* @brief
* @author
* @version V0.0.0
* @date 2019.04.01
* @details 
* @copyright
* @note 
**********************************/

#ifndef  _CSU32M10_ADC_H
#define  _CSU32M10_ADC_H 

/* Includes 
------------------------------------------------------------------*/
#include "CSU32M10.h"

/* Fuction Define -----------------------------------------------------------*/

/**
* @def ADC_AIN0
* @brief define channel AIN0
* Details.
*/
#define  ADC_AIN0    0x00

/**
* @def ADC_AIN1
* @brief define channel AIN1
* Details.
*/
#define  ADC_AIN1    0x10

/**
* @def ADC_AIN2
* @brief define channel AIN2
* Details.
*/
#define  ADC_AIN2    0x20

/**
* @def ADC_AIN3
* @brief define channel AIN3
* Details.
*/
#define  ADC_AIN3    0x30

/**
* @def ADC_AIN4
* @brief define channel AIN4
* Details.
*/
#define  ADC_AIN4    0x40

/**
* @def ADC_AIN5
* @brief define channel AIN5
* Details.
*/
#define  ADC_AIN5    0x80

/**
* @def ADC_AIN6
* @brief define channel AIN6
* Details.
*/
#define  ADC_AIN6    0x90

/**
* @def ADC_AIN7
* @brief define channel AIN7
* Details.
*/
#define  ADC_AIN7    0x50

/**
* @def ADC_AIN8
* @brief define channel AIN8
* Details.
*/
#define  ADC_AIN8    0x60 
          
/**
* @def ADC_AIN9
* @brief define channel AIN9
* Details.
*/
#define  ADC_AIN9    0x70

/* Fuction Define -----------------------------------------------------------*/

/**
* @def csu32p20_set_adc_clk()
* @brief set adc clk
* Details.
*/
#define csu32m10_set_adc_clk()            {SRADCON0 = 0x12;}

/**
* @def csu32p20_adc_enable()
* @brief enable adc function
* Details.
*/
#define csu32m10_adc_enable()             {SRADEN = 1;}

/**
* @def csu32p20_adc_start_conver() 
* @brief start adc conver
* Details.
*/
#define csu32m10_adc_start_conver()       {SRADS = 1;}

/**
* @def csu32p20_adc_wait_complete()  
* @brief wait adc conver complete
* Details.
*/
#define csu32m10_adc_wait_complete()      {unsigned char temp = 0; while(SRADS){if(++temp > 250)break;}}

/**
* @def csu32p20_adc_select_channel(ch) 
* @brief select adc channel
  param[in] adc channel
* Details.
*/
#define csu32m10_adc_select_channel(ch)   {SRADCON2 = ch;}
 
/* Exported functions 
--------------------------------------------------------*/

extern void csu32m10_adc_init(void);
extern void csu32m10_adc_get_result(void);



#endif