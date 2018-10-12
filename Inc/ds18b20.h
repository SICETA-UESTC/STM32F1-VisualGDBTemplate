#pragma once
#include <stm32f1xx_hal.h>

#define DS18B20_GPIOX       GPIOB
#define DS18B20_GPIO_PIN    GPIO_PIN_11

#define DS18B20_SET         DS18B20_GPIOX->BSRR = DS18B20_GPIO_PIN
#define DS18B20_RESET       DS18B20_GPIOX->BRR = DS18B20_GPIO_PIN
#define DS18B20_READBIT     HAL_GPIO_ReadPin(DS18B20_GPIOX, DS18B20_GPIO_PIN)

typedef enum {
    // Resolution Commands
    RESOLUTION_BASE         = 0x1F,
    // ROM Commands
    SEARCH_ROM              = 0xF0,
    READ_ROM                = 0x33,
    MATCH_ROM               = 0x55,
    SKIP_ROM                = 0xCC,
    ALARM_SEARCH            = 0xEC,
    // Function Commands
    START_CONVERT           = 0x44,
    WRITE_REG               = 0x4E,
    READ_REG                = 0xBE,
    SAVE_REG                = 0x48,
    LOAD_REG                = 0xB8,
    READ_POWER              = 0xB4,
} DS18B20_CommandTypeDef;

typedef enum {
    DS18B20_RESOLUTION_9B,
    DS18B20_RESOLUTION_10B,
    DS18B20_RESOLUTION_11B,
    DS18B20_RESOLUTION_12B,
} DS18B20_ResolutionTypeDef;

static const float decimal_setps_table[] = { 0.5f, 0.25f, 0.125f, 0.0625f };

/* Global Functions */
void DS18B20_Init(void);
void DS18B20_SetResolution(uint8_t resolution);
float DS18B20_GetTemperature(void);

/* Static Local Functions */
static _Bool DS18B20_Reset(void);
static void DS18B20_WriteCommand(uint8_t);
static uint8_t DS18B20_ReadByte(void);
static void DS18B20_SetGPIOMode(uint8_t mode);

/* External Functions */
extern void DelayUs(uint32_t);
extern void _Error_Handler(char *file, int line);