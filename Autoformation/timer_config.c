#include "timer_config.h"

void TIM2config(void)
	{
	RCC->APB1ENR|= (1<<0);
	TIM2->PSC =  84-1 ; //1MHZ=>1microsecond (tim2 in APB1 timer)
	TIM2->ARR = 0xffff; // should be max / greater then us var
	TIM2->CR1 |= (1<<0); // counter enable
	while( !(TIM2->SR & (1<<0))); //UIF update interrupt flag

}
	
void Delay_us(uint16_t us){
		
		TIM2->CNT=0;
		while(TIM2->CNT < us);
		
}
	
void Delay_ms( uint16_t ms){
	
	for (uint16_t i=0; i<ms;i++){
		
		Delay_us(1000);
	}
}


