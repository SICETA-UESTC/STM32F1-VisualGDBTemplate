#pragma once
#include <stm32f1xx_hal.h>

#define DHT11_GPIOX         GPIOB
#define DHT11_GPIO_PIN      GPIO_PIN_9

#define DHT11_IO_SET		DHT11_GPIOX->BSRR = DHT11_GPIO_PIN
#define DHT11_IO_RESET		DHT11_GPIOX->BSRR = (uint32_t)DHT11_GPIO_PIN << 16

#define DHT11_IO_READBIT	HAL_GPIO_ReadPin(DHT11_GPIOX, GPIO_PIN_9)

/* Global Functions */
void DHT11_Init(void);
uint8_t *DHT11_GetResult(void);

/* Static Local Functions */
static void DHT11_SetGPIOMode(uint8_t mode);
static uint8_t DHT11_ReadByte(void);
static _Bool DHT11_UpdateData(void);

/* External Functions */
extern void DelayMs(uint16_t);
extern void DelayUs(uint32_t);
