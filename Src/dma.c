#include "dma.h"

void DMA_Init(void)
{
/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Channel5_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 1, 2);
	HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
}