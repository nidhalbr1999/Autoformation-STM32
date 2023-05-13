#include "stm32f401xe.h"
#include "timer_config.h"


void ADC_Init2 (void);

void ADC_Enable2 (void);

void ADC_Start2 (void);

void DMA_Init2 (void);

void DMA_Config2 (uint32_t srcAdd, uint32_t destAdd, uint16_t size);

void DMA_Go2 (uint16_t datasize);

