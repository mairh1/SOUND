#include "CX588.h"

//��ʼ��CX588
void CX588_GOIO_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_PB2PeriphClockCmd(CX588_Busy_peripheral_clock, ENABLE);
    GPIO_InitStructure.GPIO_Pin = CX588_Busy_GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(CX588_Busy_GPIO_Group, &GPIO_InitStructure);

    RCC_PB2PeriphClockCmd(CX588_1Line_peripheral_clock, ENABLE);
    GPIO_InitStructure.GPIO_Pin = CX588_1Line_GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(CX588_1Line_GPIO_Group, &GPIO_InitStructure);

    GPIO_SetBits(CX588_1Line_GPIO_Group, CX588_1Line_GPIO_Pin);
}

// ������дһλ����
static void CX588_OneWire_WriteBit(uint8_t bit) 
{
    if (bit) // д0ʱ���ߵ�ƽ400us���͵�ƽ1200us
    {     
        GPIO_SetBits(CX588_1Line_GPIO_Group, CX588_1Line_GPIO_Pin);
        Delay_Us(1200);
        GPIO_ResetBits(CX588_1Line_GPIO_Group, CX588_1Line_GPIO_Pin);
        Delay_Us(400);
    } 
    else // д1ʱ���ߵ�ƽ1200us���͵�ƽ400us
    {         
        GPIO_SetBits(CX588_1Line_GPIO_Group, CX588_1Line_GPIO_Pin);
        Delay_Us(400);
        GPIO_ResetBits(CX588_1Line_GPIO_Group, CX588_1Line_GPIO_Pin);
        Delay_Us(1200);
    }
}

// ������дһ���ֽ�����
static void CX588_OneWire_WriteByte(uint8_t byte) 
{
    GPIO_ResetBits(CX588_1Line_GPIO_Group, CX588_1Line_GPIO_Pin);
    Delay_Ms(6);//��������6ms

    for (uint8_t i = 0; i < 8; i++) 
    {
        CX588_OneWire_WriteBit(byte & 0x01);// ��λд���ֽ�����
        byte >>= 1;
    }
    GPIO_SetBits(CX588_1Line_GPIO_Group, CX588_1Line_GPIO_Pin);
}

//����CX588������С
void CX588_SET_Sound_Size(CX588_SoundSize Sound_Code)
{
    CX588_OneWire_WriteByte(Sound_Code);
}

//CX588���ŵ�ǰ����
void CX588_Play_Sound(uint8_t Sound_Code)
{
    if (Sound_Code < 0xCF) 
    {
        CX588_OneWire_WriteByte(Sound_Code);
        Delay_Ms(20);
    }
}

//CX588ֹͣ������Ƶ
void CX588_Stop_Sound(void)
{
    CX588_OneWire_WriteByte(CX588_STOPPLAY_CMD);
}

// ���CX588�Ƿ�Ϊæµ״̬
FlagStatus CX588_Get_Busy_State(void)
{
    return GPIO_ReadInputDataBit(CX588_Busy_GPIO_Group,CX588_Busy_GPIO_Pin);
}