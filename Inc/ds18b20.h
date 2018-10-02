#pragma once
#include <stm32f1xx_hal.h>

#define PRECISION		0x7F
#define SKIP_ROM		0xCC
#define START_CONVERT	0x44
#define	WRITE_REG		0x4E
#define READ_REG		0xBE

#define DS18B20_IO_SET GPIOB->BSRR = GPIO_PIN_11
#define DS18B20_IO_RESET GPIOB->BRR = GPIO_PIN_11
#define DS18B20_IO_READBIT HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11)

void DS18B20_Init(void);
void DS18B20_GetTemperature(void);

static void Mode_Input(void);
static void Mode_Output(void);
static void Write_Com(uint8_t);
static uint8_t Read_Byte(void);
static _Bool Reset(void);

extern void DelayUs(uint32_t);