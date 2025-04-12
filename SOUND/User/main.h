#ifndef __MAIN_H
#define __MAIN_H

#include "debug.h"

/* Vibration Switch */
#define Vibration_GPIO_Pin GPIO_Pin_4
#define Vibration_GPIO_Group GPIOC
#define Vibration_peripheral_clock RCC_APB2Periph_GPIOC

void Vibration_Switch_GPIO_INIT(void);
void Vibration_Switch_EXTI_INIT(FunctionalState sta);
uint8_t Vibration_Switch_Read_Key(uint8_t button_id);
void BTN1_PRESS_DOWN_Handler(void* btn);

void TIM1_INT_Init( u16 arr, u16 psc);


/* LED */
void LED_GPIO_INIT(void);

#endif /* __MAIN_H */