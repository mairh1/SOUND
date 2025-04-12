/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : Maiooo
 * Version            : V1.0.0
 * Date               : 2025/4/5
 * Description        : Main program body.
 *********************************************************************************/
#include "debug.h"
#include <ch32v00x_it.h>
#include "main.h"

#include "multi_button.h"
#include "CX588.h"

const uint8_t btn1_id = 0;
struct Button Vibration_but1;

int IDLE_TIME_OUT=1000;  //1000 * 10ms = 10S
int IDLE_TIME_10S_FLAG = 0;



/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );

    Vibration_Switch_GPIO_INIT();

    button_init(&Vibration_but1,Vibration_Switch_Read_Key,0,btn1_id);
    button_attach(&Vibration_but1,PRESS_DOWN,BTN1_PRESS_DOWN_Handler);
    button_start(&Vibration_but1);

    TIM1_INT_Init( 10-1, 48000-1);
    TIM_Cmd( TIM1, ENABLE );//10MS

    LED_GPIO_INIT(); 
TEST_LED_GPIO_INIT();

    while(1)
    {
        GPIO_WriteBit(GPIOC,GPIO_Pin_1,Bit_RESET); 
        Delay_Ms(1000);
    }
}

/*********************************************************************
 * @fn      Vibration_Switch_GPIO_INIT
 *
 * @brief   Initializes Vibration_GPIO_Pin
 *
 * @return  none
 */
void Vibration_Switch_GPIO_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_APB2PeriphClockCmd(Vibration_peripheral_clock | RCC_APB2Periph_AFIO, ENABLE);
    GPIO_InitStructure.GPIO_Pin = Vibration_GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(Vibration_GPIO_Group, &GPIO_InitStructure);

    Vibration_Switch_EXTI_INIT(DISABLE);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI7_0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*********************************************************************
 * @fn      Vibration_Switch_EXTI_INIT
 *
 * @brief   Initializes Vibration_GPIO_EXTI_Line
 *
 * @param   sta - DISABLE or ENABLE
 *
 * @return  none
 */
void Vibration_Switch_EXTI_INIT(FunctionalState sta)
{
    EXTI_InitTypeDef EXTI_InitStructure = {0};

    /* GPIOC.4 ----> EXTI_Line4 */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4);
    EXTI_InitStructure.EXTI_Line = EXTI_Line4;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = sta;
    EXTI_Init(&EXTI_InitStructure);
}

/*********************************************************************
 * @fn      Vibration_Switch_Read_Key
 *
 * @param   GPIO_Pin - specifies the port bit to be written.
 *            This parameter can be one of GPIO_Pin_x where x can be (0..7).
 *
 * @return  Bit_SET     - Key Down.
 *          Bit_RESET   - The button is not pressed.
 */
uint8_t Vibration_Switch_Read_Key(uint8_t button_id)
{
    switch (button_id) 
    {
        case 0: 
            return GPIO_ReadInputDataBit(Vibration_GPIO_Group,Vibration_GPIO_Pin);
        default:
            return 0;
    }
    
}

/*********************************************************************
 * @fn      BTN1_PRESS_DOWN_Handler
 *
 * @param   * btn - struct Button Structure pointer.
 *            
 * @return  none
 *
 */
void BTN1_PRESS_DOWN_Handler(void* btn)
{
    struct Button *button = (struct Button *)btn; // 强制类型转换
    printf("Button ID: %d Clicked!\n", button->button_id);

    IDLE_TIME_10S_FLAG = 0;//清空空闲计数
    GPIO_WriteBit(GPIOC,GPIO_Pin_1,Bit_SET); 
    Delay_Ms(100);
}

/*********************************************************************
 * @fn      TIM1_INT_Init
 *
 * @brief   Initializes TIM1 output compare.
 *
 * @param   arr - the period value.
 *          psc - the prescaler value.
 *
 * @return  none
 */
void TIM1_INT_Init( u16 arr, u16 psc)
{

    NVIC_InitTypeDef NVIC_InitStructure={0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure={0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE );

    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit( TIM1, &TIM_TimeBaseInitStructure);

    TIM_ClearITPendingBit( TIM1, TIM_IT_Update );

    NVIC_InitStructure.NVIC_IRQChannel =TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;
    NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
}

/*********************************************************************
 * @fn      LED_GPIO_INIT
 *
 * @brief   Initializes GPIOC.1.2
 *
 * @return  none
 */
void LED_GPIO_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void TEST_LED_GPIO_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}