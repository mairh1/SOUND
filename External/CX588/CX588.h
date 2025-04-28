#ifndef CX588_H
#define CX588_H

#include "ch32l103.h"

#define SOUND_DATA 0x08
#define CX588_SOUND_DATA    SOUND_DATA-1

#define CX588_POWERIN_DATA  SOUND_DATA+1
#define CX588_POWEROUT_DATA SOUND_DATA+2

#define CX588_OPEN_SOUND    SOUND_DATA+3
#define CX588_CLOSE_SOUND   SOUND_DATA+4


/* CX588 Busy_Line */
#define CX588_Busy_GPIO_Pin             GPIO_Pin_3
#define CX588_Busy_GPIO_Group           GPIOA
#define CX588_Busy_peripheral_clock     RCC_PB2Periph_GPIOA

/* CX588 One_Line */
#define CX588_1Line_GPIO_Pin            GPIO_Pin_4
#define CX588_1Line_GPIO_Group          GPIOA
#define CX588_1Line_peripheral_clock    RCC_PB2Periph_GPIOA


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

void CX588_GOIO_INIT(void);
void CX588_SET_Sound_Size(CX588_SoundSize Sound_Code);
void CX588_Play_Sound(uint8_t Sound_Code);
void CX588_Stop_Sound(void);

FlagStatus CX588_Get_Busy_State(void);

#endif