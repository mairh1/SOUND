#ifndef CX588_H
#define CX588_H

#include "debug.h"

/* CX588 Busy_Line */
#define CX588_Busy_GPIO_Pin GPIO_Pin_1
#define CX588_Busy_GPIO_Group GPIOC
#define CX588_Busy_peripheral_clock RCC_APB2Periph_GPIOC

/* CX588 One_Line */
#define CX588_1Line_GPIO_Pin GPIO_Pin_2
#define CX588_1Line_GPIO_Group GPIOC
#define CX588_1Line_peripheral_clock RCC_APB2Periph_GPIOC


/* CX588 CMD */
typedef enum
{
    SoundLeve_0=0xE0,
    SoundLeve_1,
    SoundLeve_2,
    SoundLeve_3,
    SoundLeve_4,
    SoundLeve_5,
    SoundLeve_6,
    SoundLeve_7,
    SoundLeve_8,
    SoundLeve_9,
    SoundLeve_10,
    SoundLeve_11,
    SoundLeve_12,
    SoundLeve_13,
    SoundLeve_14,
    SoundLeve_15
}CX588_SoundSize;

#define CX588_STOPPLAY_CMD  0xF0
#define CX588_LOOPPLAY_CMD  0xF2
#define CX588_SEQUENTIALPLAY_CMD  0xF3

#define CX588_SHUTDOWN_CMD  0xFE
#define CX588_POWERON_CMD   0x00

void CX588_GOIO_INIT(void);
void CX588_SET_Sound_Size(CX588_SoundSize Sound_Code);
FlagStatus CX588_Get_Busy_State(void);

void CX588_Stop_Sound(void);
void CX588_Play_Sound(uint8_t Sound_Code);
void CX588_Loop_Play_Sound(uint8_t Sound_Code);
void CX588_Sequential_Play_Sound(uint8_t arr[], int size);
#endif