/**********************************
* @file csu32p20_adc.c
* @brief
* @author
* @version V0.0.0
* @date 2019.04.03
* @details 
* @copyright
* @note 
**********************************/

/* Includes ------------------------------------------------------------------*/

#include "CSU32M10.h"
#include "csu32m10_adc.h"

/* Fuction Declare ----------------------------------------------------------*/

/***********************************
* @fn  void csu32p20_adc_init(void)
* @brief ADC init.
* @param[out] none.
* @param[in] none.
* @return none.
************************************/
void csu32m10_adc_init(void);

/***********************************
* @fn  static void delay_us(void)
* @brief delay time.
* @param[out] none.
* @param[in] none.
* @return none.
************************************/
static void delay_us(void);

/***********************************
* @fn  void csu32p20_adc_get_result(void)
* @brief get ADC result .
* @param[out] none.
* @param[in] none.
* @return none.
************************************/
void csu32m10_adc_get_result(void);

/***********************************
* @fn  static void delay_us(void)
* @brief delay time.
* @param[out] none.
* @param[in] none.
* @return none.
************************************/

static void delay_us(void)
{
  unsigned int j;
  for(j = 1000;j > 0;j--);
}

/***********************************
* @fn  void csu32p20_adc_init(void)
* @brief ADC init.
* @param[out] none.
* @param[in] none.
* @return none.
************************************/

void csu32m10_adc_init(void)
{
  csu32m10_set_adc_clk(); //set adc clock
  OFTEN = 0;
  CALIF = 0;
  ENOV = 0;
  VREFS_0 = 0;  //set VDD VREF 
  VREFS_1 = 0;  
  PT3EN_0 = 0; //set AIN0~AIN2 io input mode
  PT3EN_1 = 0;
  PT3EN_2 = 0;
}

/***********************************
* @fn  void csu32p20_adc_get_result(void)
* @brief get ADC result .
* @param[out] none.
* @param[in] none.
* @return none.
************************************/

void csu32m10_adc_get_result(void)
{
  static unsigned char channel = 0;
  static unsigned char conver_times = 0;
  static unsigned int result = 0;
  static unsigned int result_temp = 0;
     switch(channel)
     {
        case 0:{
                  REF_SEL_2 = 0; // 1.4V inside VREF
                  csu32m10_adc_select_channel(ADC_AIN8);
                  VREFS_0 = 0;  // set vdd VREF 
                  VREFS_1 = 0;  
                  delay_us(); 
               }break;
        case 1:{
                  PT3CON_0 = 1; //set PT3.0 analog input
                  csu32m10_adc_select_channel(ADC_AIN0);
                  VREFS_0 = 0;  // set vdd VREF 
                  VREFS_1 = 0;  
                  delay_us(); 
               }break;
        case 2:{
                  PT3CON_1 = 1; //set PT3.1 analog input
                  csu32m10_adc_select_channel(ADC_AIN1);
                  VREFS_0 = 0;  // set inside VREF 
                  VREFS_1 = 1;  
                  REF_SEL_2 = 0; //inside VREF = 1.4V
                  delay_us(); // wait VREF switch
               }break;
        case 3:{
                  PT3CON_2 = 1; //set PT3.2 analog input
                  csu32m10_adc_select_channel(ADC_AIN2);
                  VREFS_0 = 1;  // set outside VREF 
                  VREFS_1 = 0;    
                  delay_us();   // wait VREF switch
               }break;    
      default: {channel = 0;}
     }
     csu32m10_adc_enable(); //enable adc 
     csu32m10_adc_start_conver(); 
     csu32m10_adc_wait_complete(); // wait conver complet
     result = (SRADH << 8) | SRADL; //get adc result
     result_temp += result;
     conver_times++;
     if(conver_times >= 8) // 8 times sample
     {
        conver_times = 0;
        result = result_temp>>3;   //get average rate
        result_temp = 0;
        channel++; 
     }
}

