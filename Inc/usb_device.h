#pragma once
#include <stm32f1xx.h>
#include <stm32f1xx_hal.h>
#include "usbd_def.h"

/** USB device core handle. */
extern USBD_HandleTypeDef hUsbDeviceFS;

/** USB Device initialization function. */
void USB_Init(void);