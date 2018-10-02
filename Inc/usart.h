#pragma once
#include <stm32f1xx_hal.h>

#define UART_RX_BUFFER_SIZE 1024

/* 串口缓冲区结构体 */
typedef struct {
	uint8_t Data[UART_RX_BUFFER_SIZE];
	_Bool IsComplete;
	uint16_t Count;
} UART_RxBufferTypeDef;

/* 全局函数 */
void USART1_Init(void);
void USART3_Init(void);
/* 外部函数 */
extern void _Error_Handler(char * file, int line);