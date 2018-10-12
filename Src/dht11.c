#include"dht11.h"
#include <stdio.h>

static uint8_t s_humidty_h8b;
static uint8_t s_humidty_l8b;
static uint8_t s_temperature_h8b;
static uint8_t s_temperature_l8b;

static uint8_t s_str_buffer[64];

void DHT11_Init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    Mode_Output();
    s_humidty_h8b = 0;
    s_humidty_l8b = 0;
    s_temperature_h8b = 0;
    s_temperature_l8b = 0;
}

uint8_t *DHT11_GetResult(void)
{
    if (DHT11_UpdateData())
    {
        sprintf(s_str_buffer, "Temperature is %u degree, Humidity is %u %%\n", s_temperature_h8b, s_temperature_l8b);
    }
    else
    {
        sprintf(s_str_buffer, "DHT11 Read Failed!\n");
    }
    return s_str_buffer;
}

static _Bool DHT11_UpdateData(void)
{
    DHT11_SetGPIOMode(GPIO_MODE_OUTPUT_PP);

    DHT11_IO_RESET;
    DelayMs(20);
    DHT11_IO_SET;
    DelayUs(20);

    DHT11_SetGPIOMode(GPIO_MODE_INPUT);

    //	if (!DHT11_IO_READBIT)
    //	{
    while (DHT11_IO_READBIT);
    while (!DHT11_IO_READBIT);
    while (DHT11_IO_READBIT);

    s_humidty_h8b = ReadByte();
    s_humidty_l8b = ReadByte();
    s_temperature_h8b = ReadByte();
    s_temperature_l8b = ReadByte();
    uint8_t checksum = ReadByte();

    return (s_humidty_h8b + s_humidty_l8b + s_temperature_h8b + s_temperature_l8b == checksum);
    //	}
    //	return 0;
}

static void DHT11_SetGPIOMode(uint8_t mode)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = mode;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

static uint8_t DHT11_ReadByte(void)
{
    uint8_t byte = 0;

    for (uint8_t i = 0; i < 8; i++)
    {
        while (!DHT11_IO_READBIT);

        DelayUs(30);
        byte <<= 1;

        if (DHT11_IO_READBIT == 1)
        {
            byte |= 1;
        }
        while (DHT11_IO_READBIT);
    }

    return byte;
}

