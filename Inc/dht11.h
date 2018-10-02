#pragma once
#include <stm32f1xx_hal.h>

#define DHT11_IO_SET		GPIOB->BSRR = GPIO_PIN_9
#define DHT11_IO_RESET		GPIOB->BRR = GPIO_PIN_9
#define DHT11_IO_READBIT	HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9)

union
{
	uint32_t VALUE;

	struct {
		uint8_t Hum_H;
		uint8_t Hum_L;
		uint8_t Temp_H;
		uint8_t Temp_L;
	};

}DHT11_Data;

void DHT11_Init(void);
static void Mode_Input(void);
static void Mode_Output(void);
static uint8_t ReadByte(void);
static _Bool UpdateData(void);
void DHT11_GetResult(void);
extern void DelayMs(uint16_t);
extern void DelayUs(uint32_t);
