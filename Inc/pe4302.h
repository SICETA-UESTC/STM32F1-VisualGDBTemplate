#pragma once
#include <stm32f1xx_hal.h>

void PE4302_Init(void);
void PE4302_SetLoss(uint8_t channel, uint8_t twoTimes_dB);