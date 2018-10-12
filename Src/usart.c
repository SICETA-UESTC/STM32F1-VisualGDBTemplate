#include "usart.h"

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

UART_RxBufferTypeDef uart1_rx_buffer;

/* 重定向std输出流到 USART1 */
size_t _write(int file, const uint8_t *pBuffer, size_t len)
{
    HAL_UART_Transmit(&huart1, pBuffer, len, 0xFF);
    return len;
}

/* USART1 初始化 */
void USART1_Init(void)
{
    huart1.Instance = USART1;                           //USART1
    huart1.Init.BaudRate = 115200;                      //波特率 115200
    huart1.Init.WordLength = UART_WORDLENGTH_8B;        //数据位 8
    huart1.Init.StopBits = UART_STOPBITS_1;             //停止位 1
    huart1.Init.Parity = UART_PARITY_NONE;              //无校验位
    huart1.Init.Mode = UART_MODE_TX_RX;                 //收发模式
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;        //无硬件流控制
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;    //过采样 16倍

    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
    //开始DMA接收数据
    HAL_UART_Receive_DMA(&huart1, &uart1_rx_buffer.Data, UART_RX_BUFFER_SIZE);
}

/* USART3 初始化 */
void USART3_Init(void)
{
    huart3.Instance = USART3;                           //USART1
    huart3.Init.BaudRate = 115200;                      //波特率 115200
    huart3.Init.WordLength = UART_WORDLENGTH_8B;        //数据位 8
    huart3.Init.StopBits = UART_STOPBITS_1;             //停止位 1
    huart3.Init.Parity = UART_PARITY_NONE;              //无校验位
    huart3.Init.Mode = UART_MODE_TX_RX;                 //收发模式
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;        //无硬件流控制
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;    //过采样 16倍

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
        /* USART1 时钟启动 */
        __HAL_RCC_USART1_CLK_ENABLE();

        /**USART1 GPIO 映射
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

        /* USART1 RX DMA 初始化 */
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

        /* USART1 TX DMA 初始化 */
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

        /* USART1 中断 初始化 */
        HAL_NVIC_SetPriority(USART1_IRQn, 0, 2);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
    }
    else if (husart->Instance == USART3)
    {
        /* USART3 时钟启动 */
        __HAL_RCC_USART3_CLK_ENABLE();

        /**USART3 GPIO 映射
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
        /* USART1 时钟关闭 */
        __HAL_RCC_USART1_CLK_DISABLE();
        /**USART1 GPIO 关闭
        PA9     ------> USART1_TX
        PA10     ------> USART1_RX
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);
        /* USART1 DMA 关闭 */
        HAL_DMA_DeInit(husart->hdmarx);
        HAL_DMA_DeInit(husart->hdmatx);
        /* USART1 中断 关闭 */
        HAL_NVIC_DisableIRQ(USART1_IRQn);
    }
    else if (husart->Instance == USART3)
    {
        /* USART3 时钟关闭 */
        __HAL_RCC_USART3_CLK_DISABLE();
        /**USART3 GPIO 关闭
        PB10     ------> USART3_TX
        PB11     ------> USART3_RX
        */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10 | GPIO_PIN_11);
    }
}

/* USART1 中断处理 */
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

/* USART1 TX DMA 中断处理 */
void DMA1_Channel4_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_usart1_tx);
}

/* USART1 RX DMA 中断处理 */
void DMA1_Channel5_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_usart1_rx);
}


