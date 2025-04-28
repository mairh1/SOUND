#ifndef BUTTON_H
#define BUTTON_H

#include "ch32l103.h"

/* Vibration Switch */
#define Vibration_GPIO_Pin          GPIO_Pin_0
#define Vibration_GPIO_Group        GPIOA
#define Vibration_peripheral_clock  RCC_PB2Periph_GPIOA
#define Vibration_Port_Sources      GPIO_PortSourceGPIOA
#define Vibration_Pin_sources       GPIO_PinSource0
#define Vibration_EXTI_Lines        EXTI_Line0
#define Vibration_EXTI_IRQn         EXTI0_IRQn

/* Hall Switch */
#define Hall_GPIO_Pin               GPIO_Pin_1
#define Hall_GPIO_Group             GPIOA
#define Hall_peripheral_clock       RCC_PB2Periph_GPIOA
#define Hall_Port_Sources           GPIO_PortSourceGPIOA
#define Hall_Pin_sources            GPIO_PinSource1
#define Hall_EXTI_Lines             EXTI_Line1
#define Hall_EXTI_IRQn              EXTI1_IRQn

/* Charge Switch */
#define Charge_GPIO_Pin             GPIO_Pin_11
#define Charge_GPIO_Group           GPIOB
#define Charge_peripheral_clock     RCC_PB2Periph_GPIOB
#define Charge_Port_Sources         GPIO_PortSourceGPIOB
#define Charge_Pin_sources          GPIO_PinSource11
#define Charge_EXTI_Lines           EXTI_Line11
#define Charge_EXTI_IRQn            EXTI15_10_IRQn

/* LEDx */
#define LED1_GPIO_Pin             GPIO_Pin_8
#define LED1_GPIO_Group           GPIOA
#define LED1_peripheral_clock     RCC_PB2Periph_GPIOA


void Switch_GPIO_INIT(void);
void Switch_EXTI_INIT(void);
void Switch_EXTI_DEINIT(void);
void LEDx_GPIO_INIT(void);

#endif