#ifndef BUTTON_H
#define BUTTON_H

#include <ch32v00x.h>

/* Vibration Switch */
#define Vibration_GPIO_Pin          GPIO_Pin_4
#define Vibration_GPIO_Group        GPIOC
#define Vibration_peripheral_clock  RCC_APB2Periph_GPIOC
#define Vibration_Port_Sources      GPIO_PortSourceGPIOC
#define Vibration_Pin_sources       GPIO_PinSource4
#define Vibration_EXTI_Lines        EXTI_Line4

/* Hall Switch */
#define Hall_GPIO_Pin               GPIO_Pin_2
#define Hall_GPIO_Group             GPIOA
#define Hall_peripheral_clock       RCC_APB2Periph_GPIOA
#define Hall_Port_Sources           GPIO_PortSourceGPIOA
#define Hall_Pin_sources            GPIO_PinSource2
#define Hall_EXTI_Lines             EXTI_Line2

/* Charge Switch */
#define Charge_GPIO_Pin             GPIO_Pin_1
#define Charge_GPIO_Group           GPIOA
#define Charge_peripheral_clock     RCC_APB2Periph_GPIOA
#define Charge_Port_Sources         GPIO_PortSourceGPIOA
#define Charge_Pin_sources          GPIO_PinSource1
#define Charge_EXTI_Lines        EXTI_Line1

void Switch_GPIO_INIT(void);
void Switch_EXTI_INIT(FunctionalState sta);
#endif