#include "Button.h"

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
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    RCC_PB2PeriphClockCmd(Vibration_peripheral_clock, ENABLE);    
    GPIO_InitStructure.GPIO_Pin = Vibration_GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(Vibration_GPIO_Group, &GPIO_InitStructure);

    RCC_PB2PeriphClockCmd(Hall_peripheral_clock, ENABLE); 
    GPIO_InitStructure.GPIO_Pin = Hall_GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(Hall_GPIO_Group, &GPIO_InitStructure);

    RCC_PB2PeriphClockCmd(Charge_peripheral_clock, ENABLE);
    GPIO_InitStructure.GPIO_Pin = Charge_GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(Charge_GPIO_Group, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = Vibration_EXTI_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = Hall_EXTI_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = Charge_EXTI_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
 
    Switch_EXTI_DEINIT();
}


/*********************************************************************
 * @fn      Switch_EXTI_INIT
 *
 * @brief   Deinitializes the EXTI peripheral registers to their default
 *        reset values.
 *
 * @return  none
 */
void Switch_EXTI_DEINIT(void)
{
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
void Switch_EXTI_INIT(void)
{
    RCC_PB2PeriphClockCmd(RCC_PB2Periph_AFIO,ENABLE);
    EXTI_InitTypeDef EXTI_InitStructure = {0};

    /* Vibration */
    GPIO_EXTILineConfig(Vibration_Port_Sources, Vibration_Pin_sources);
    EXTI_InitStructure.EXTI_Line = Vibration_EXTI_Lines;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Hall */
    GPIO_EXTILineConfig(Hall_Port_Sources, Hall_Pin_sources);
    EXTI_InitStructure.EXTI_Line = Hall_EXTI_Lines;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Charge */
    GPIO_EXTILineConfig(Charge_Port_Sources, Charge_Pin_sources);
    EXTI_InitStructure.EXTI_Line = Charge_EXTI_Lines;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}


/*********************************************************************
 * @fn      LEDx_GPIO_INIT
 *
 * @brief   Initializes LED_GPIOX
 *
 * @return  none
 */
void LEDx_GPIO_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_PB2PeriphClockCmd(LED1_peripheral_clock, ENABLE);    
    GPIO_InitStructure.GPIO_Pin = LED1_GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(LED1_GPIO_Group, &GPIO_InitStructure);

}