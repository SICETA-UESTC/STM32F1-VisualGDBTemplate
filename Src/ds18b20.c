#include "ds18b20.h"

uint8_t s_resolution;

void DS18B20_Init(void)
{
    if (!DS18B20_Reset())
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    DS18B20_WriteCommand(WRITE_REG);
    DS18B20_WriteCommand(0x64);
    DS18B20_WriteCommand(0x00);
    //DS18B20_SetResolution(DS18B20_RESOLUTION_12B);
}

void DS18B20_SetResolution(uint8_t resolution)
{
    s_resolution = resolution;
    DS18B20_WriteCommand(RESOLUTION_BASE | (resolution << 5));
}

float DS18B20_GetTemperature(void)
{
    DS18B20_Reset();
    DS18B20_WriteCommand(SKIP_ROM);
    DS18B20_WriteCommand(START_CONVERT);
    //DelayMs(1000);
    DS18B20_Reset();
    DS18B20_WriteCommand(SKIP_ROM);
    DS18B20_WriteCommand(READ_REG);

    int16_t temperature = 0x0000;
    temperature |= DS18B20_ReadByte();
    temperature |= DS18B20_ReadByte() << 8;

    return temperature * decimal_setps_table[s_resolution];
}

static _Bool DS18B20_Reset(void)
{
    DS18B20_SetGPIOMode(GPIO_MODE_OUTPUT_OD);
    DS18B20_RESET;
    DelayUs(500);
    DS18B20_SET;
    DelayUs(24);

    DS18B20_SetGPIOMode(GPIO_MODE_INPUT);
    _Bool presence = !DS18B20_READBIT;

    DelayUs(200);
    DS18B20_SET;
    DS18B20_SetGPIOMode(GPIO_MODE_OUTPUT_OD);
    return presence;
}

static void DS18B20_WriteCommand(uint8_t com)
{
    DS18B20_SetGPIOMode(GPIO_MODE_OUTPUT_OD);

    for (uint8_t i = 0; i < 8; i++)
    {
        DS18B20_RESET;
        DelayUs(5);

        if (com & 1)
        {
            DS18B20_SET;
        }
        else
        {
            DS18B20_RESET;
        }

        DelayUs(55);
        DS18B20_SET;
        DelayUs(2);
        com >>= 1;
    }
}

static uint8_t DS18B20_ReadByte(void)
{
    uint8_t data = 0;

    for (uint8_t i = 0; i < 8; i++)
    {
        DS18B20_SetGPIOMode(GPIO_MODE_OUTPUT_OD);
        DS18B20_SET;
        DelayUs(2);
        DS18B20_RESET;
        DelayUs(2);
        DS18B20_SET;
        DS18B20_SetGPIOMode(GPIO_MODE_INPUT);
        DelayUs(15);

        data >>= 1;
        if (DS18B20_READBIT) data |= 0x80;

        DelayUs(45);
    }
    return data;
}

static void DS18B20_SetGPIOMode(uint8_t mode)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Mode = mode;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}