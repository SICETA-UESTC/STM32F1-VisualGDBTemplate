#include "usart.h"

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

UART_RxBufferTypeDef uart1_rx_buffer;

/* �ض���std������� USART1 */
size_t _write(int file, const uint8_t *pBuffer, size_t len)
{
	HAL_UART_Transmit(&huart1, pBuffer, len, 0xFF);
	return len;
}

/* USART1 ��ʼ�� */
void USART1_Init(void)
{
	huart1.Instance = USART1;                           //USART1
	huart1.Init.BaudRate = 115200;                      //������ 115200
	huart1.Init.WordLength = UART_WORDLENGTH_8B;        //����λ 8
	huart1.Init.StopBits = UART_STOPBITS_1;             //ֹͣλ 1
	huart1.Init.Parity = UART_PARITY_NONE;              //��У��λ
	huart1.Init.Mode = UART_MODE_TX_RX;                 //�շ�ģʽ
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;        //��Ӳ��������
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;    //������ 16��

	if (HAL_UART_Init(&huart1) != HAL_OK) 
	{
		_Error_Handler(__FILE__, __LINE__);
	}
    //��ʼDMA��������
	HAL_UART_Receive_DMA(&huart1, &uart1_rx_buffer.Data, UART_RX_BUFFER_SIZE);
}

/* USART3 ��ʼ�� */
void USART3_Init(void)
{
    huart3.Instance = USART3;                           //USART1
    huart3.Init.BaudRate = 115200;                      //������ 115200
    huart3.Init.WordLength = UART_WORDLENGTH_8B;        //����λ 8
    huart3.Init.StopBits = UART_STOPBITS_1;             //ֹͣλ 1
    huart3.Init.Parity = UART_PARITY_NONE;              //��У��λ
    huart3.Init.Mode = UART_MODE_TX_RX;                 //�շ�ģʽ
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;        //��Ӳ��������
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;    //������ 16��

    if (HAL_UART_Init(&huart3) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
}

void HAL_UART_MspInit(UART_HandleTypeDef* husart)
{
    GPIO_InitTypeDef GPIO_InitStruct;

	if (husart->Instance == USART1)
	{
		/* USART1 ʱ������ */
		__HAL_RCC_USART1_CLK_ENABLE();

		/**USART1 GPIO ӳ��
		PA9     ------> USART1_TX
		PA10     ------> USART1_RX
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_9;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USART1 RX DMA ��ʼ�� */
        hdma_usart1_rx.Instance = DMA1_Channel5;
        hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_usart1_rx.Init.Mode = DMA_NORMAL;
        hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
        if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
        {
            _Error_Handler(__FILE__, __LINE__);
        }

        __HAL_LINKDMA(husart, hdmarx, hdma_usart1_rx);

        /* USART1 TX DMA ��ʼ�� */
        hdma_usart1_tx.Instance = DMA1_Channel4;
        hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_usart1_tx.Init.Mode = DMA_NORMAL;
        hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
        if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
        {
            _Error_Handler(__FILE__, __LINE__);
        }

        __HAL_LINKDMA(husart, hdmatx, hdma_usart1_tx);

        /* USART1 �ж� ��ʼ�� */
		HAL_NVIC_SetPriority(USART1_IRQn, 0, 2);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
	}
    else if (husart->Instance == USART3)
    {
        /* USART3 ʱ������ */
        __HAL_RCC_USART3_CLK_ENABLE();

        /**USART3 GPIO ӳ��
        PB10     ------> USART3_TX
        PB11     ------> USART3_RX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_11;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* husart)
{
	if (husart->Instance == USART1)
	{
        /* USART1 ʱ�ӹر� */
        __HAL_RCC_USART1_CLK_DISABLE();
        /**USART1 GPIO �ر�
        PA9     ------> USART1_TX
        PA10     ------> USART1_RX
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);
        /* USART1 DMA �ر� */
        HAL_DMA_DeInit(husart->hdmarx);
        HAL_DMA_DeInit(husart->hdmatx);
        /* USART1 �ж� �ر� */
        HAL_NVIC_DisableIRQ(USART1_IRQn);
	}
    else if (husart->Instance == USART3)
    {
        /* USART3 ʱ�ӹر� */
        __HAL_RCC_USART3_CLK_DISABLE();
        /**USART3 GPIO �ر�
        PB10     ------> USART3_TX
        PB11     ------> USART3_RX
        */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10 | GPIO_PIN_11);
    }
}

/* USART1 �жϴ��� */
void USART1_IRQHandler(void)
{
	if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE))
	{
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);

		__HAL_DMA_DISABLE(huart1.hdmarx);
		uart1_rx_buffer.Count = UART_RX_BUFFER_SIZE - hdma_usart1_rx.Instance->CNDTR;
		hdma_usart1_rx.Instance->CNDTR = UART_RX_BUFFER_SIZE;
		__HAL_DMA_ENABLE(huart1.hdmarx);

		uart1_rx_buffer.IsComplete = 1;
		uart1_rx_buffer.Data[uart1_rx_buffer.Count] = '\0';
	}
	HAL_UART_IRQHandler(&huart1);
}

/* USART1 TX DMA �жϴ��� */
void DMA1_Channel4_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_usart1_tx);
}

/* USART1 RX DMA �жϴ��� */
void DMA1_Channel5_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_usart1_rx);
}


