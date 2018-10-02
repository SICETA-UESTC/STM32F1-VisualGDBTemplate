#pragma once
#include <stm32f1xx_hal.h>

#define UART_RX_BUFFER_SIZE 1024

/* ���ڻ������ṹ�� */
typedef struct {
	uint8_t Data[UART_RX_BUFFER_SIZE];
	_Bool IsComplete;
	uint16_t Count;
} UART_RxBufferTypeDef;

/* ȫ�ֺ��� */
void USART1_Init(void);
void USART3_Init(void);
/* �ⲿ���� */
extern void _Error_Handler(char * file, int line);