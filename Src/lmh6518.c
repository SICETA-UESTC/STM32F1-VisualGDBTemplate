#include "lmh6518.h"
#include "spi.h"

LMH6518_RegTypeDef reg;

void LMH6518_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    reg.Data = 0x0000;
    reg.BitField.AUXHIGHZ = 0;
    reg.BitField.PREAMP = PREAMP_LG;
    reg.BitField.ATTENUATION = LA_0dB;
    reg.BitField.FILTER = BD_20MHz;
    LMH6518_WriteData();

    LMH6518_CS1;
    LMH6518_SCK1;
}

void LMH6518_SetAuxOutput(_Bool is_enabled)
{
    reg.BitField.AUXHIGHZ = !is_enabled;
    LMH6518_WriteData();
}

void LMH6518_SetPreAmp(LMH6518_PreAmpMode mode)
{
    reg.BitField.PREAMP = mode;
    LMH6518_WriteData();
}

void LMH6518_SetBandWidth(LMH6518_FilterBandwith bandwith)
{
    reg.BitField.FILTER = bandwith;
    LMH6518_WriteData();
}

void LMH6518_SetAttenuation(LMH6518_LadderAttenuation attenuation)
{
    reg.BitField.ATTENUATION = attenuation;
    LMH6518_WriteData();
}

void LMH6518_GetReg(void)
{
    LMH6518_CS0;
    uint8_t temp[16];
    for (size_t i = 0; i < 8; i++)
    {
        LMH6518_SCK0;
        LMH6518_SDATA1;
        DelayMs(1);
        LMH6518_SCK1;
        DelayMs(1);
    }

    for (size_t i = 0; i < 16; i++)
    {
        LMH6518_SCK0;
        DelayMs(1);
        LMH6518_SCK1;
        temp[i] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) + '0';
        DelayMs(1);
    }

    printf("%s\n", temp);

    LMH6518_CS1;
}

static inline void LMH6518_WriteData(void)
{
    LMH6518_CS0;
    uint32_t temp = reg.Data << 8;

    for (uint8_t i = 0; i < 24; i++)
    {
        LMH6518_SCK0;
        DelayMs(1);
        if (temp & 0x80000000) {
            LMH6518_SDATA1;
        }
        else {
            LMH6518_SDATA0;
        }
        temp <<= 1;
        LMH6518_SCK1;
        DelayMs(1);
    }
    LMH6518_CS1;
    DelayMs(1);
}
