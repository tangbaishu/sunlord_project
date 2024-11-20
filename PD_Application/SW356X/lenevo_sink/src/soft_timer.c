/****************************************************************************
 * @copyright Copyright(C) 2020-2022 Ismartware Limited. All rights reserved.
 * @file soft_timer.c
 * @brief  The generic soft timer implementation
 * @author SD1 team
 ****************************************************************************/
#include "soft_timer.h"
#include "api.h"
#include "zr_nvic.h"
#include "zr_rcc.h"
#include "zr_btm.h"

#define TIME_INTERVAL  (2)        // 2ms
#define HARDWARE_TIMER_RELOAD (125 * TIME_INTERVAL - 1)
#define HARDWARE_TIMER_IRQ (BTM4_IRQn)

static volatile u32 gSysTick = 0;
static volatile u8 gSoftTimerNumber = 0;

void BTM4_Handler(void)
{       
    Low_Power_Not_Sleep();  
    Btm4_Clear_Int_Pending();    
    gSysTick += TIME_INTERVAL;
}

/**
 * @brief  start a soft timer
 * @param[in]  self soft timer, allocate memory at consumer
 * @param[in]  timeInMs the interval in Ms
 */
void Soft_Timer_Start(soft_timer_t* self, u32 timeInMs)
{
    if (timeInMs == 0)
    {
        return;
    }

    if (gSoftTimerNumber == 0)
    {
        Btm4_Enable();  //enable 
    }
    
    // shall be here after clock enabled
    gSoftTimerNumber++;

    self->startTime = gSysTick;
    self->interval = timeInMs;
}

/**
 * @brief  stop a soft timer
 * @param[in]  self soft timer, allocate memory at consumer
 */
void Soft_Timer_Stop(soft_timer_t* self)
{
    if (gSoftTimerNumber && self->interval)
    {
        gSoftTimerNumber--;
    }
    self->interval = 0;

    if (!gSoftTimerNumber)
    {
        Btm4_Disable();   //disable
    }
}

/**
 * @brief  check whether soft timer is expired
 * @param[in]  self the soft timer
 * @return true when it is expired
 */
bool Soft_Timer_Is_Expired(soft_timer_t* self)
{
    return (self->interval && ((gSysTick - self->startTime) >= self->interval));
}

/**
 * @brief  init soft timer
 */
void Soft_Timer_Init(void)
{
	Rcc_Module_Clk_Enable(BTM_ID);
	Btm4_Set_Reload_Value(HARDWARE_TIMER_RELOAD);
	Btm4_Int_Enable();
	
    Nvic_Clear_Irq_Pending(HARDWARE_TIMER_IRQ);
    Nvic_Irq_Enable(HARDWARE_TIMER_IRQ, PRIORITY_3);   //en irq channel  

	gSysTick = 0;	
	gSoftTimerNumber = 0;	
}
