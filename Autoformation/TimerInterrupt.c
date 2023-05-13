#include "TimerInterrupt.h"

void TIM4config(void)
	{
	RCC->APB1ENR|= (1<<2);
	TIM4->PSC =  (84-1)*100 ; //1MHZ=>1microsecond (tim2 in APB1 timer)
	TIM4->ARR = 0xffff; // should be max / greater then us var
		
		 // Send an update event to reset the timer and apply settings.
  TIM4->EGR  |= TIM_EGR_UG;
  // Enable the hardware interrupt.
  TIM4->DIER |= TIM_DIER_UIE;
		
	NVIC_SetPriority(TIM4_IRQn, 1);
	NVIC_EnableIRQ (TIM4_IRQn);
	TIM4->CR1 |= (1<<0); // counter enable
	//while( !(TIM4->SR & (1<<0))); //UIF update interrupt flag

}


void TIM4_IRQHandler(void) {
  // Handle a timer 'update' interrupt event
  if (TIM4->SR & TIM_SR_UIF) {
		 PWR->CR &= ~PWR_CR_PDDS;            // Exit standby mode
  PWR->CSR |= PWR_CSR_WUF;            // Set wake-up flag
    TIM4->SR &= ~(TIM_SR_UIF);
    // Toggle the LED output pin.
    GPIOA->ODR ^= (1 << 5);
  }
}

