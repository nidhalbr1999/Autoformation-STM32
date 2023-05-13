#include "ADC.h"
#include "UART.h"

void ADC_Config (void){
	
	RCC->APB2ENR |= (1<<8);
	RCC->AHB1ENR  |= (1<<0);
	
	ADC->CCR |= (1<<16); //div4
	
	ADC1->CR1 |= (1<<8); // scan mode (for multi channels)
	ADC1->CR1 &= ~(1<<24); // resolution (12bits)
	
	ADC1->CR2 |= (1<<1); // continous conv
	ADC1->CR2 |= (1<<10); // end of conv bit
	ADC1->CR2 &= ~(1<<11); // align right 
	
	ADC1->SMPR2 &= ~((1<<3) | (1<<12)); // channel 1 and 4 (sampling rate)
	ADC1->SQR1 |= (1<<20);// 2 channels (Length)
	
	GPIOA->MODER |= (3<<2); //analog mode for PA1
	GPIOA->MODER |= (3<<8);// analog mode for PA4
	
	//analog watchdog
	ADC1->CR1 |= 1<<23; // enable the analog watchdog
	ADC1->CR1 |= (1<<6); // Enable analog watchdog interrupt
  ADC1->CR1 |= ADC_CR1_AWDSGL; // Single channel  mode watchdog
	ADC1->CR1 |= (1<<2);     // Select channel 1 for the analog watchdog
	 
  ADC1->HTR |= 0x000F;              // Set the high threshold to 0xFFF
  ADC1->LTR |= 0x00800;              // Set the low threshold to 0x800
	
	//ADC1->CR1 |= ADC_CR1_EOCIE; // Enable the interrupt when the conversion is complete
	NVIC_SetPriority(ADC_IRQn,1);
	NVIC_EnableIRQ(ADC_IRQn);
}

void ADC_Enable (void){
	
	
	ADC1->CR2 |=(1<<0);// adon (enable)
	
	uint32_t delay =10000;
	while (delay--);

}

void ADC_START(int channel){
	
	ADC1->SQR3 =0;
	ADC1->SQR3 |= (channel << 0); // reading the channel (first in SQR3[0])
	ADC1->SR=0;
	ADC1->CR2 |= (1<<30); // start conv **
	
}

void ADC_WaitForConv (void){
	
	while (!(ADC1->SR & (1<<1)));
	
}

uint16_t ADC_GetVal(void){
	
	return ADC1->DR;
	
}

void ADC_Disable (void){
	
	ADC1->CR2 &= ~(1<<0);
	
}

void ADC_IRQHandler(void)
{
    if (ADC1->SR & ADC_SR_AWD)
    {	
			UART2_SendString("nidhal\n");
			ADC1->SR &= ~ADC_SR_AWD;
		} 
		
}


