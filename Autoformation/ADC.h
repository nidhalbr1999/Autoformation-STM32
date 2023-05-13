#include "stm32f401xe.h"

void ADC_Config (void);

void ADC_Enable (void);

void ADC_START(int channel);

void ADC_WaitForConv (void);

uint16_t ADC_GetVal(void);

void ADC_Disable (void);

void ADC_IRQHandler(void);
