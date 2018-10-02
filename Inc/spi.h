#pragma once
#include <stm32f1xx_hal.h>

/* 全局函数 */
void SPI1_Init(void);
void SPI2_Init(void);
uint8_t SPI1_ReadWriteByte(uint8_t data);
uint8_t SPI2_ReadWriteByte(uint8_t data);

/* 外部函数 */
extern void _Error_Handler(char * file, int line);