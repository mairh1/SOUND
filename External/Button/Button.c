#include "Button.h"
#include <ch32v00x.h>

/*********************************************************************
 * @fn      Switch_GPIO_INIT
 *
 * @brief   Initializes GPIOX
 *
 * @return  none
 */
void Switch_GPIO_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_APB2PeriphClockCmd(Vibration_peripheral_clock, ENABLE);    
    GPIO_InitStructure.GPIO_Pin = Vibration_GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(Vibration_GPIO_Group, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(Hall_peripheral_clock, ENABLE);
    GPIO_InitStructure.GPIO_Pin = Hall_GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(Hall_GPIO_Group, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(Charge_peripheral_clock, ENABLE);
    GPIO_InitStructure.GPIO_Pin = Charge_GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(Charge_GPIO_Group, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI7_0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //Switch_EXTI_INIT(DISABLE);
    EXTI_DeInit();
}


/*********************************************************************
 * @fn      Switch_EXTI_INIT
 *
 * @brief   Initializes Switch_GPIO_EXTI_Line
 *
 * @param   sta - DISABLE or ENABLE
 *
 * @return  none
 */
void Switch_EXTI_INIT(FunctionalState sta)
{
    EXTI_InitTypeDef EXTI_InitStructure = {0};

    /* Vibration */
    GPIO_EXTILineConfig(Vibration_Port_Sources, Vibration_Pin_sources);
    EXTI_InitStructure.EXTI_Line = Vibration_EXTI_Lines;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = sta;
    EXTI_Init(&EXTI_InitStructure);

    GPIO_EXTILineConfig(Hall_Port_Sources, Hall_Pin_sources);
    EXTI_InitStructure.EXTI_Line = Hall_EXTI_Lines;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = sta;
    EXTI_Init(&EXTI_InitStructure);

    GPIO_EXTILineConfig(Charge_Port_Sources, Charge_Pin_sources);
    EXTI_InitStructure.EXTI_Line = Charge_EXTI_Lines;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = sta;
    EXTI_Init(&EXTI_InitStructure);

}