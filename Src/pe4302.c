#include "pe4302.h"

const static uint16_t GPIO_Latch[4] = { GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_10, GPIO_PIN_11 };

void PE4302_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_10 | GPIO_PIN_11;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}


void PE4302_SetLoss(uint8_t channel, uint8_t twoTimes_dB)
{
	twoTimes_dB = (twoTimes_dB > 63) ? 63 : twoTimes_dB; //�ᣬ��˥�۸�������ӣ���
		
	GPIOB->BSRR = GPIO_Latch[channel];	//ȡ������
	GPIOA->ODR = twoTimes_dB;			//��˥��������
	GPIOB->BRR = GPIO_Latch[channel];	//������ס
	
	printf("PE4302_%u�� ˥��ֵ��Ϊ: %4.1f dB\n",channel, twoTimes_dB * 0.5f); //log
}