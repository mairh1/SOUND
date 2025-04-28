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

/* ����� */
static uint32_t FALSE_RANDOM = 0;

/* ���� */
struct Button Vibration_but1;
struct Button Hall_but2;

/* ��ʱ�� */
#define IDLE_TIME_OUT 500  //500 * 10ms = 5S
uint32_t IDLE_TIME_COUNT = 0;

/* �������� */
bool Hall_FLAG = true;  // ��̬����������״̬

/* ����״̬��ģʽ��*/
SYSTEM_STATE CURRENT_MODE = SLEEP_MODE; 


bool WAKER_FLAG = false;

int main(void)
{
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );

    //LEDx_GPIO_INIT();
    //GPIO_ResetBits(LED1_GPIO_Group,LED1_GPIO_Pin);

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
    srand(FALSE_RANDOM);                //���ɲ��������ļ������

    while(1)
    {
        srand(FALSE_RANDOM);                //���ɲ��������ļ������
        //GPIO_ResetBits(LED1_GPIO_Group,LED1_GPIO_Pin);//����ʱ��LED��

        switch (CURRENT_MODE)
        {
            case SLEEP_MODE: 
                if (IDLE_TIME_COUNT == IDLE_TIME_OUT && CX588_Get_Busy_State())
                {
                    TIM_Cmd(TIM1, DISABLE);//�رռ�ʱ��
                    Switch_EXTI_INIT();
                    RCC_PB1PeriphClockCmd(RCC_PB1Periph_PWR, ENABLE);
                    /*close cc1,cc2 pull-down resistors */
                    USBPD->PORT_CC1 &= ~(0x01<<1);
                    USBPD->PORT_CC2 &= ~(0x01<<1);
                    //����ֹͣģʽ�������жϻ��ѣ����Ѻ�������жϻص�����

                    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
                }               
                break;
            case WAKEUP_MODE:
                IDLE_TIME_COUNT = 0;

                SystemCoreClockUpdate();
                printf("SystemClk:%d\r\n", SystemCoreClock);

                TIM1_INT_Init( 10-1, 48000-1);
                TIM_Cmd( TIM1, ENABLE ); 
                USART_Printf_Init(115200);

                Switch_EXTI_DEINIT();
                RCC_PB1PeriphClockCmd(RCC_PB1Periph_PWR, DISABLE);

                if (WAKER_FLAG)
                {
                    WAKER_FLAG = false;
                    CURRENT_MODE = OPERATING_MODE;//�л�������ģʽ
                }
                else
                {
                    CURRENT_MODE = SLEEP_MODE;//�л�������ģʽ
                }
                break;
            case CHARGE_MODE:
                break;
            case OPERATING_MODE:
                if (CX588_Get_Busy_State() && Hall_FLAG)
                    CX588_Play_Sound((rand() % (CX588_SOUND_DATA + 1))+1);//���������Ƶ������ֻ��8����Ƶ�ļ���

                FALSE_RANDOM += IDLE_TIME_COUNT;//������������
                IDLE_TIME_COUNT = 0;//��տ��м���
                CURRENT_MODE = SLEEP_MODE;//�л�������ģʽ
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
    IDLE_TIME_COUNT = 0;     // ��տ��м���
    CURRENT_MODE = OPERATING_MODE;//�л�������ģʽ 
}
void BTN2_SINGLE_CLICK_Handler(void* btn)
{
    if (Hall_FLAG)
    {
        //����������ر�����
        CX588_Play_Sound(CX588_CLOSE_SOUND);
        Delay_Ms(40);
        //CX588_GOIO_DEINIT();
        printf("�ر�����!\r\n");
    }
    else
    {
        //CX588_GOIO_INIT();
        //���������������
        Delay_Ms(40);
        CX588_Play_Sound(CX588_OPEN_SOUND);
        printf("������!\r\n");
    }

    Hall_FLAG = !Hall_FLAG;  // �л� Hall_FLAG ��״̬
    IDLE_TIME_COUNT = 0;     // ��տ��м���

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