#include "ad983x.h"
#include "spi.h"

static AD983X_ControlRegTypeDef s_control_reg;
static AD983X_FreqRegTypeDef s_freq_reg;
static AD983X_PhaseRegTypeDef s_phase_reg;

void AD983X_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	FSYNC1;
	SCLK0;

    s_control_reg.Data = 0x0000;
    s_freq_reg.Data = 0x0000;
    s_phase_reg.Data = 0xC000;

	s_control_reg.BitField.B28 = 1;
	s_control_reg.BitField.FSEL = 0;
	s_control_reg.BitField.PSEL = 0;
	//	s_control_reg.BitField.OPBITEN = 1;
	s_control_reg.BitField.MODE = 0;

	WriteReg(s_control_reg.Data);
	WriteReg(s_phase_reg.Data); //œ‡Œªºƒ¥Ê∆˜÷√¡„
}


void AD983X_SetFreq(uint32_t freq, _Bool reg_select) //HZ
{
	freq *= AD983X_FREQ_FACTOR;

	s_freq_reg.BitField.FLAG = 0x01 << reg_select; //FREQ0 ~ [0 : 1], FREQ1 ~ [1 : 0]

	s_freq_reg.BitField.FLAG = freq & 0x3FFF;
	WriteReg(s_freq_reg.Data);

	s_freq_reg.BitField.FREQ = (freq & 0x0FFFC000) >> 14;
	WriteReg(s_freq_reg.Data);
}

void AD983X_SwitchMode(void)
{
	s_control_reg.BitField.MODE = !s_control_reg.BitField.MODE;
	WriteReg(s_control_reg.Data);
}

static void AD983X_WriteReg(uint16_t data)
{
	FSYNC0;
	for (uint8_t i = 0; i < 16; ++i)
	{
		SCLK1;
		if (data & 0x8000) SDATA1;
		else SDATA0;
		SCLK0;
		data <<= 1;
	}

	SCLK1;
	FSYNC1;
}