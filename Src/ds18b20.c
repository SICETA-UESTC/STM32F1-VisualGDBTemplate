#include "ds18b20.h"

void DS18B20_Init(void)
{
	if (!Reset()) while (1);
	/*
	Write_Com(SKIP_ROM); //跳过ROM
	
	Write_Com(WRITE_REG);
	Write_Com(0x64);
	Write_Com(0x00);
	Write_Com(PRECISION); //设定精度
	*/
}

void DS18B20_GetTemperature(void)
{
	uint16_t MSB, LSB;
	uint16_t temp;

	Reset();
	Write_Com(SKIP_ROM); //跳过ROM
	Write_Com(START_CONVERT); //开始温度转换

	//DelayMs(1000);

	Reset();
	Write_Com(SKIP_ROM);
	Write_Com(READ_REG); //读温度寄存器

	LSB = Read_Byte();
	MSB = Read_Byte();

	_Bool minus = 0;

	if (MSB > 0x7F)      				//最高位为1时温度是负
	{
		LSB = ~LSB;					//补码转换，取反加一
		MSB = ~MSB + 1;
		minus = 1;
	}

	MSB <<= 8;

	printf("temperature = %.2f\r\n", (MSB | LSB) * 0.0625f);
}

static void Write_Com(uint8_t com)
{
	Mode_Output();

	for (uint8_t i = 0; i < 8; i++)
	{
		DS18B20_IO_RESET;
		DelayUs(5);

		if (com & 1)
		{
			DS18B20_IO_SET;
		}
		else
		{
			DS18B20_IO_RESET;
		}

		DelayUs(55);
		DS18B20_IO_SET;
		DelayUs(2);
		com >>= 1;
	}
}

static uint8_t Read_Byte(void)
{
	uint8_t data = 0;

	for (uint8_t i = 0; i < 8; i++)
	{
		Mode_Output();
		DS18B20_IO_SET;
		DelayUs(2);
		DS18B20_IO_RESET;
		DelayUs(2);
		DS18B20_IO_SET;
		Mode_Input();
		DelayUs(15);

		data >>= 1;
		if (DS18B20_IO_READBIT) data |= 0x80;
		
		DelayUs(45);


	}
	return data;
}

static _Bool Reset(void)
{
	Mode_Output();
	DS18B20_IO_RESET;
	DelayUs(500);
	DS18B20_IO_SET;
	DelayUs(24);

	Mode_Input();
	_Bool presence = !DS18B20_IO_READBIT;

	DelayUs(200);
	DS18B20_IO_SET;
	Mode_Output();
	return presence;
}

static void Mode_Input(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

static void Mode_Output(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}