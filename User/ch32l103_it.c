/********************************** (C) COPYRIGHT *******************************
 * File Name          : ch32l103_it.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/10/30
 * Description        : Main Interrupt Service Routines.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/
#include <stdbool.h>
#include "ch32l103_it.h"
#include "main.h"
#include "multi_button.h"
#include "Button.h"

extern int IDLE_TIME_COUNT;
extern SYSTEM_STATE CURRENT_MODE;

extern bool WAKER_FLAG;
/*********************************************************************
 * @fn      NMI_Handler
 *
 * @brief   This function handles NMI exception.
 *
 * @return  none
 */
void NMI_Handler(void)
{
    while (1)
    {
    }
}


/*********************************************************************
 * @fn      HardFault_Handler
 *
 * @brief   This function handles Hard Fault exception.
 *
 * @return  none
 */
void HardFault_Handler(void)
{
    NVIC_SystemReset();
    while (1)
    {
    }
}

/*********************************************************************
 * @fn      TIM1_UP_IRQHandler
 *
 * @brief   This function handles TIM1 UP exception.
 *
 *
 * @return  none
 */
void TIM1_UP_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM1, TIM_IT_Update)==SET)
    {
/*         if (IDLE_TIME_COUNT %100 == 0) 
        {
            printf("IDLE_TIME_COUNT = %d\r\n",IDLE_TIME_COUNT);
            printf("CURRENT_MODE = %d\r\n",CURRENT_MODE);
        } */
        

        button_ticks();
        if (IDLE_TIME_COUNT < 0xfffffffe) 
            IDLE_TIME_COUNT++;
    }
    TIM_ClearITPendingBit( TIM1, TIM_IT_Update );
}

/*********************************************************************
 * @fn      EXTI0_IRQHandler
 *
 * @brief   This function handles EXTI0 exception.
 *
 * @return  none
 */
void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(Vibration_EXTI_Lines)!=RESET)
    {
        SystemInit();
        WAKER_FLAG = true;
        // printf("Vibration_EXTI_Lines Wake_up\r\n");
        EXTI_ClearITPendingBit(Vibration_EXTI_Lines);     /* Clear Flag */
        CURRENT_MODE = WAKEUP_MODE;//ÇÐ»»µ½»½ÐÑÃßÄ£Ê½
    }
}

/*********************************************************************
 * @fn      EXTI1_IRQHandler
 *
 * @brief   This function handles EXTI1 exception.
 *
 * @return  none
 */
void EXTI1_IRQHandler(void)
{
    if(EXTI_GetITStatus(Hall_EXTI_Lines)!=RESET)
    {
        SystemInit();
        // printf("Hall_EXTI_Lines Wake_up\r\n");
        EXTI_ClearITPendingBit(Hall_EXTI_Lines);     /* Clear Flag */
        CURRENT_MODE = WAKEUP_MODE;//ÇÐ»»µ½»½ÐÑÃßÄ£Ê½
    }
}

/*********************************************************************
 * @fn      EXTI15_10_IRQHandler
 *
 * @brief   This function handles EXTI15_10 exception.
 *
 * @return  none
 */
void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(Charge_EXTI_Lines)!=RESET)
    {
        SystemInit();
        // printf("Charge_EXTI_Lines Wake_up\r\n");
        EXTI_ClearITPendingBit(Charge_EXTI_Lines);     /* Clear Flag */
        CURRENT_MODE = WAKEUP_MODE;//ÇÐ»»µ½»½ÐÑÃßÄ£Ê½
    }
}