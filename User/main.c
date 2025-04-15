//             DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//                     Version 2, December 2004

//  Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>

//  Everyone is permitted to copy and distribute verbatim or modified
//  copies of this license document, and changing it is allowed as long
//  as the name is changed.

//             DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//    TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

//   0. You just DO WHAT THE FUCK YOU WANT TO.

#include "debug.h"
#include <stdlib.h>
#include <stdbool.h>
#include <ch32v00x_it.h>
#include "main.h"

#include "multi_button.h"
#include "CX588.h"
#include "Button.h"

/* 按键 */
struct Button Vibration_but1;
struct Button Hall_but2;

/* 定时器 */
#define IDLE_TIME_OUT 500  //500 * 10ms = 5S
uint32_t IDLE_TIME_COUNT = 0;

/* 随机数 */
static uint32_t FALSE_RANDOM = 0;

/* 运行状态（模式）*/
SYSTEM_STATE CURRENT_MODE = SLEEP_MODE; 

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

    Switch_GPIO_INIT();

    button_init(&Vibration_but1,Read_Button_GPIO,0,BTN_VIBRATION);
    button_attach(&Vibration_but1,PRESS_DOWN,BTN1_PRESS_DOWN_Handler);
    button_start(&Vibration_but1);

    button_init(&Hall_but2,Read_Button_GPIO,0,BTN_HALL);
    button_attach(&Hall_but2,SINGLE_CLICK,BTN2_SINGLE_CLICK_Handler);
    button_start(&Hall_but2);

    TIM1_INT_Init( 10-1, 48000-1);
    TIM_Cmd( TIM1, ENABLE );//10MS

    CX588_GOIO_INIT();
    Delay_Ms(20);
    CX588_SET_Sound_Size(SoundLeve_15);
    srand(FALSE_RANDOM);

    while(1)
    {
        Check_Power_Link();

        switch (CURRENT_MODE)
        {
            case SLEEP_MODE:
                if (IDLE_TIME_COUNT == IDLE_TIME_OUT && CX588_Get_Busy_State())
                {
                    TIM_Cmd( TIM1, DISABLE );//关闭计时器
                    Switch_EXTI_INIT(ENABLE);
                    printf("Enters Sleep\r\n");
                    __WFI();//进入休眠模式，任意中断唤醒，唤醒后会进入该中断回调函数
                }   
                break;

            case WAKEUP_MODE:
                IDLE_TIME_COUNT = 0;
                TIM_Cmd( TIM1, ENABLE );
                //Switch_EXTI_INIT(DISABLE);
                EXTI_DeInit();
                CURRENT_MODE = OPERATING_MODE;//切换到工作模式
                break;

            case CHARGE_MODE:
                button_stop(&Vibration_but1);
                button_stop(&Hall_but2);
                IDLE_TIME_COUNT = 0; //清空计数器
                break;
            case OPERATING_MODE:
                if (CX588_Get_Busy_State())
                {
                    CX588_Play_Sound((rand() % (0x07 + 1))+1);//随机播放音频
                    printf("rand() ID: %d \n", rand() % (0x07 + 1));
                }
                FALSE_RANDOM += IDLE_TIME_COUNT;//填充随机数种子
                IDLE_TIME_COUNT = 0;//清空空闲计数
                CURRENT_MODE = SLEEP_MODE;//切换到休眠模式
                break;
            
            default:
                break;
        }
    }
}


/*********************************************************************
 * @fn      read_button_GPIO
 *
 * @param   GPIO_Pin - specifies the port bit to be written.
 *            This parameter can be one of GPIO_Pin_x where x can be (0..7).
 *
 * @return  Bit_SET     - Key Down.
 *          Bit_RESET   - The button is not pressed.
 */
uint8_t Read_Button_GPIO(uint8_t button_id)
{
    switch (button_id) 
    {
        case BTN_VIBRATION: 
            return GPIO_ReadInputDataBit(Vibration_GPIO_Group,Vibration_GPIO_Pin);
            break;
        case BTN_HALL: 
            return GPIO_ReadInputDataBit(Hall_GPIO_Group,Hall_GPIO_Pin);
            break;
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

    IDLE_TIME_COUNT = 0;     // 清空空闲计数
    CURRENT_MODE = OPERATING_MODE;//切换到工作模式  
}

/*********************************************************************
 * @fn      BTN2_SINGLE_CLICK_Handler
 *
 * @param   * btn - struct Button Structure pointer.
 *            
 * @return  none
 *
 */
void BTN2_SINGLE_CLICK_Handler(void* btn)
{
    struct Button *button = (struct Button *)btn; // 强制类型转换
    printf("Button ID: %d Clicked!\n", button->button_id);

    static bool Hall_FLAG = true;  // 静态变量，保留状态

    if (Hall_FLAG)
    {
        //语音输出：关闭语音
        CX588_Play_Sound(0x0c);
        CX588_GOIO_DEINIT();
    }
    else
    {
        CX588_GOIO_INIT();
        //语音输出：打开语音
        CX588_Play_Sound(0x0b);
    }

    Hall_FLAG = !Hall_FLAG;  // 切换 Hall_FLAG 的状态
    IDLE_TIME_COUNT = 0;     // 清空空闲计数
}

/*********************************************************************
 * @fn      Check_Power_Link
 *
 * @brief   Check that the power cord is connected.
 *            
 * @return  none
 *
 */
void Check_Power_Link(void)
{
    static bool CHARGE_FLAG = false;  // 静态变量，保留状态
    static bool SOUND_FLAG = false;  // 静态变量，保留状态

    if (GPIO_ReadInputDataBit(Charge_GPIO_Group, Charge_GPIO_Pin) != Bit_SET)
    {
        CHARGE_FLAG = true;
        CURRENT_MODE = CHARGE_MODE; // 切换到充电模式
        if (SOUND_FLAG != true)
        {
            SOUND_FLAG = true;
            CX588_Play_Sound(0x09);
        }
    }    
    else 
    {
        if (CHARGE_FLAG)  // 判断静态变量
        {
            button_start(&Vibration_but1);
            button_start(&Hall_but2);
            CHARGE_FLAG = false;
            SOUND_FLAG = false;
            CX588_Play_Sound(0x0a);
            IDLE_TIME_COUNT = 0;     // 清空空闲计数
            CURRENT_MODE = SLEEP_MODE; // 切换到休眠模式
        }           
    }
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
