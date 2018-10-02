#include"dht11.h"

void DHT11_Init(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	Mode_Output();
	DHT11_Data.VALUE = 0;
}

void DHT11_GetResult(void)
{
	if (UpdateData())
	{
		printf("Temperature is %u degree, Humidity is %u %%\n", DHT11_Data.Temp_H, DHT11_Data.Hum_H);
	}
	else
	{
		puts("Failed!");
	}
}

static _Bool UpdateData(void)
{
	Mode_Output();
	DHT11_IO_RESET;
	DelayMs(20);
	DHT11_IO_SET;
	DelayUs(20);

	Mode_Input();

	//	if (!DHT11_IO_READBIT)
	//	{
	while (DHT11_IO_READBIT);
	while (!DHT11_IO_READBIT);
	while (DHT11_IO_READBIT);

	DHT11_Data.Hum_H = ReadByte();
	DHT11_Data.Hum_L = ReadByte();
	DHT11_Data.Temp_H = ReadByte();
	DHT11_Data.Temp_L = ReadByte();
	uint8_t Check = ReadByte();

	return (DHT11_Data.Hum_H + DHT11_Data.Hum_L + DHT11_Data.Temp_H + DHT11_Data.Temp_L == Check);
	//	}
	//	return 0;
}

static void Mode_Input(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

static void Mode_Output(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

static uint8_t ReadByte(void)
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

