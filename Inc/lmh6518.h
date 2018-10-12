#pragma once
#include <stm32f1xx_hal.h>

#define LMH6518_GPIO        GPIOA
#define LMH6518_CS_PIN      GPIO_PIN_4
#define LMH6518_SCK_PIN     GPIO_PIN_5
#define LMH6518_SDATA_PIN   GPIO_PIN_7

#define	LMH6518_CS1			LMH6518_GPIO->BSRR = LMH6518_CS_PIN
#define	LMH6518_CS0			LMH6518_GPIO->BRR = LMH6518_CS_PIN

#define	LMH6518_SCK1		LMH6518_GPIO->BSRR = LMH6518_CS_PIN
#define	LMH6518_SCK0		LMH6518_GPIO->BRR = LMH6518_CS_PIN

#define	LMH6518_SDATA1		LMH6518_GPIO->BSRR = LMH6518_SDATA_PIN
#define	LMH6518_SDATA0		LMH6518_GPIO->BRR = LMH6518_SDATA_PIN

#define LMH6518_READ		0x80
#define LMH6518_WRITE		0x00

typedef enum {
    PREAMP_HG, PREAMP_LG,
} LMH6518_PreAmpMode;

typedef enum {
    BD_FULL, BD_20MHz, BD_100MHz, BD_200MHz, BD_350MHz, BD_650MHz, BD_750MHz,
} LMH6518_FilterBandwith;

typedef enum {
    LA_0dB, LA_2dB, LA_4dB, LA_6dB, LA_8dB, LA_10dB, LA_12dB, LA_14dB, LA_16dB, LA_18dB, LA_20dB,
} LMH6518_LadderAttenuation;

typedef union {
    /* PGA Register */
    struct {
        unsigned ATTENUATION    : 4;
        _Bool PREAMP            : 1;
        _Bool                   : 1;
        unsigned FILTER         : 3;
        _Bool                   : 1;
        _Bool AUXHIGHZ          : 1;
        unsigned : 5;
    } BitField;
    /* SPI ReadWrite Value */
    uint16_t Data;
} LMH6518_RegTypeDef;

void LMH6518_Init(void);
void LMH6518_SetAuxOutput(_Bool is_enabled);
void LMH6518_SetPreAmp(LMH6518_PreAmpMode mode);
void LMH6518_SetBandWidth(LMH6518_FilterBandwith bandwith);
void LMH6518_SetAttenuation(LMH6518_LadderAttenuation attenuation);

static inline void LMH6518_WriteData(void);
extern uint8_t SPI1_ReadWriteByte(uint8_t data);



