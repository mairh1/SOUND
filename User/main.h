#ifndef __MAIN_H
#define __MAIN_H

#include "debug.h"

typedef enum {
    SLEEP_MODE = 0,
    WAKEUP_MODE,    // Fixed typo from WEAKUP_MODE
    CHARGE_MODE,
    OPERATING_MODE 
} SYSTEM_STATE;

typedef enum {
    BTN_VIBRATION = 1,
    BTN_HALL
} BUTTON_ID;

uint8_t Read_Button_GPIO(uint8_t button_id);
void BTN1_PRESS_DOWN_Handler(void* btn);
void BTN2_SINGLE_CLICK_Handler(void* btn);

void TIM1_INT_Init( u16 arr, u16 psc);
void Check_Power_Link(void);

#endif /* __MAIN_H */