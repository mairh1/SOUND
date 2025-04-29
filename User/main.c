//             DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//                     Version 2, December 2004

//  Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>

//  Everyone is permitted to copy and distribute verbatim or modified
//  copies of this license document, and changing it is allowed as long
//  as the name is changed.

//             DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//    TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

//   0. You just DO WHAT THE FUCK YOU WANT TO.

#include <stdlib.h>
#include <stdbool.h>

#include "debug.h"
#include "main.h"
#include "ch32l103_it.h"

#include "CX588.h"
#include "Button.h"
#include "multi_button.h"

/* 随机数 */
static uint32_t FALSE_RANDOM = 0;

/* 按键 */
struct Button Vibration_but1;
struct Button Hall_but2;

/* 定时器 */
#define IDLE_TIME_OUT 500  //500 * 10ms = 5S
uint32_t IDLE_TIME_COUNT = 0;

/* 霍尔开关 */
bool Hall_FLAG = true;  // 静态变量，保留状态

/* 运行状态（模式）*/
SYSTEM_STATE CURRENT_MODE = SLEEP_MODE; 

/* 唤醒标志 */
bool WAKER_FLAG = false;

int main(void)
{
    SystemCoreClockUpdate();
    Delay_Init();
    Close_Useless_GPIO();

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
    CX588_SET_Sound_Size(SoundLeve_15);
    srand(FALSE_RANDOM);                //生成播放声音文件随机数

    while(1)
    {
        srand(FALSE_RANDOM);            //生成播放声音文件随机数
        Check_Charger_Link();           //检查充电器连接

        switch (CURRENT_MODE)
        {
            case SLEEP_MODE: 
                if (IDLE_TIME_COUNT == IDLE_TIME_OUT && CX588_Get_Busy_State())
                {
                    TIM_Cmd(TIM1, DISABLE);//关闭计时器
                    Switch_EXTI_INIT();
                    RCC_PB1PeriphClockCmd(RCC_PB1Periph_PWR, ENABLE);
                    //进入停止模式，任意中断唤醒，唤醒后会进入该中断回调函数
                    PWR_EnterSTOPMode_RAM_LV(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
                }               
                break;

            case WAKEUP_MODE:
                IDLE_TIME_COUNT = 0;//清空计数器

                SystemCoreClockUpdate();
                TIM1_INT_Init( 10-1, 48000-1);
                TIM_Cmd( TIM1, ENABLE ); 

                Switch_EXTI_DEINIT();//关闭GPIO端口EXTI中断
                RCC_PB1PeriphClockCmd(RCC_PB1Periph_PWR, DISABLE);

                if (WAKER_FLAG)
                {
                    WAKER_FLAG = false;
                    CURRENT_MODE = OPERATING_MODE;//切换到工作模式
                }
                else
                    CURRENT_MODE = SLEEP_MODE;//切换到工作模式
                break;

            case CHARGE_MODE:
                button_stop(&Vibration_but1);
                button_stop(&Hall_but2);
                IDLE_TIME_COUNT = 0; //清空计数器
                break;

            case OPERATING_MODE:
                if (CX588_Get_Busy_State() && Hall_FLAG)
                    CX588_Play_Sound((rand() % (CX588_SOUND_DATA + 1))+1);//随机播放音频（里面只有8段音频文件）

                FALSE_RANDOM += IDLE_TIME_COUNT;//填充随机数种子
                IDLE_TIME_COUNT = 0;//清空空闲计数
                CURRENT_MODE = SLEEP_MODE;//切换到休眠模式
                break;  

            default:
                break;
        }
    }
}

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

void BTN1_PRESS_DOWN_Handler(void* btn)
{
    IDLE_TIME_COUNT = 0;     // 清空空闲计数
    CURRENT_MODE = OPERATING_MODE;//切换到工作模式 
}

void BTN2_SINGLE_CLICK_Handler(void* btn)
{
    if (Hall_FLAG)
        CX588_Play_Sound(CX588_CLOSE_SOUND);//语音输出：关闭语音
    else
        CX588_Play_Sound(CX588_OPEN_SOUND);//语音输出：打开语音

    Hall_FLAG = !Hall_FLAG;  // 切换 Hall_FLAG 的状态
    IDLE_TIME_COUNT = 0;     // 清空空闲计数
}

void Check_Charger_Link(void)
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
            CX588_Play_Sound(CX588_POWERIN_DATA);
        }
    }    
    else 
    {
        if (CHARGE_FLAG)  // 判断静态变量
        {
            CHARGE_FLAG = false;
            SOUND_FLAG = false;
            button_start(&Vibration_but1);
            button_start(&Hall_but2);
            CX588_Play_Sound(CX588_POWEROUT_DATA);
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

    RCC_PB2PeriphClockCmd(RCC_PB2Periph_TIM1, ENABLE );

    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit( TIM1, &TIM_TimeBaseInitStructure);

    TIM_ClearITPendingBit( TIM1, TIM_IT_Update );

    NVIC_InitStructure.NVIC_IRQChannel =TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;
    NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

}

/*********************************************************************
 * @fn      Close_Useless_GPIO
 *
 * @brief   Turn off useless GPIO pins
 *
 * @return  none
 */
void Close_Useless_GPIO(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOA, ENABLE);    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 |GPIO_Pin_5 |GPIO_Pin_6 |GPIO_Pin_7 |GPIO_Pin_8 |GPIO_Pin_9 |GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOB, ENABLE);    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1 |GPIO_Pin_6 |GPIO_Pin_10 |GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}