#include "ExInterrupt.h"


void GPConf(void){

	RCC->AHB1ENR |= 1<<2;
	GPIOC->MODER &= ~(3<<26);
	GPIOC->PUPDR |= 1<<26;
	
}

void InterruptConf(void){
	
	RCC->APB2ENR |= (1<<14);  // Enable SYSCNFG
	SYSCFG->EXTICR[3] &= ~(0xf<<4);  // Bits[3:2:1:0] = (0:0:0:0)  -> configure EXTI13 line for PC13
	SYSCFG->EXTICR[3] |= (1<<5);
	
	EXTI->IMR |= (1<<13);  // Bit[1] = 1  --> Disable the Mask on EXTI 0
	EXTI->RTSR |= (1<<13);  // Enable Rising Edge Trigger for PA1
	EXTI->FTSR &= ~(1<<13);  // Disable Falling Edge Trigger for PA1
	
	NVIC_SetPriority (EXTI15_10_IRQn, 1);  // Set Priority
	NVIC_EnableIRQ (EXTI15_10_IRQn);  // Enable Interrupt
}

void EXTI15_10_IRQHandler (void){
	
	if (EXTI->PR & (1<<13))    // If the PA0 triggered the interrupt
{
	PWR->CR &= ~PWR_CR_PDDS;            // Exit standby mode
  PWR->CSR |= PWR_CSR_WUF;  // Set wake-up flag
    GPIOA->BSRR |=(1<<5);
		Delay_ms(1000);
		GPIOA->BSRR |=(1<<21);
		Delay_ms(1000);
	UART2_SendChar('A');
	EXTI->PR |= (1<<13);  // Clear the interrupt flag by writing a 1 
}
}








