#include "iwdg.h"

IWDG_HandleTypeDef hiwdg;

/* IWDG init function */
void IWDG_Init(void)
{
    hiwdg.Instance = IWDG;
    hiwdg.Init.Prescaler = IWDG_PRESCALER_4;
    hiwdg.Init.Reload = 4095;

    if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
}