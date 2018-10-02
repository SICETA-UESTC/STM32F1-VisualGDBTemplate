#pragma once
#include<stm32f1xx_hal.h>

#define AD983X_MCLK             50000000U
#define AD983X_FREQ_FACTOR      5.3687091

#define SDATA1                  GPIOA->BSRR = GPIO_PIN_0
#define SDATA0                  GPIOA->BRR = GPIO_PIN_0
#define SCLK1                   GPIOA->BSRR = GPIO_PIN_1
#define SCLK0                   GPIOA->BRR = GPIO_PIN_1
#define FSYNC1                  GPIOA->BSRR = GPIO_PIN_2
#define FSYNC0                  GPIOA->BRR = GPIO_PIN_2

/* AD983X Control Register */
typedef union {
    struct {
        _Bool           : 1;
        _Bool MODE      : 1;
        _Bool           : 1;
        _Bool DIV2      : 1;
        _Bool SIGN_PB   : 1;
        _Bool OPBITEN   : 1;
        _Bool SLEEP12   : 1;
        _Bool SLEEP1    : 1;
        _Bool RESET     : 1;
        _Bool PIN_SW    : 1;
        _Bool PSEL      : 1;
        _Bool FSEL      : 1;
        _Bool HLB       : 1;
        _Bool B28       : 1;
        uint16_t        : 2;
    } BitField;
    uint16_t Data;
} AD983X_ControlRegTypeDef;

/* AD983X Frequency Register */
typedef union {
    struct {
        uint16_t FREQ   : 14;
        uint16_t FLAG   : 2;
    } BitField;
    uint16_t Data;
} AD983X_FreqRegTypeDef;

/* AD983X Phase Register */
typedef union {
    struct {
        uint16_t FREQ   : 12;
        _Bool           : 1;
        _Bool FLAG      : 1;
        uint16_t        : 2;
    } BitField;
    uint16_t Data;
} AD983X_PhaseRegTypeDef;


/* Global Functions */
void AD983X_Init(void);
void AD983X_SetFreq(uint32_t freq, _Bool reg_select);
void AD983X_SwitchMode(void);
/* Static Local Functions */
static void AD983X_WriteReg(uint16_t);
/* External Functions */
extern void DelayUs(uint32_t);