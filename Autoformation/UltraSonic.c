#include "UltraSonic.h"
#include "timer_config.h"
#include <stdio.h>
#include "UART.h"

uint32_t rising_time, falling_time, distance;



void TIM3_IRQHandler(void)
{
  if (TIM3->SR & TIM_SR_CC1IF)
  {
			if (GPIOA->IDR & (1<<6)){
      rising_time = TIM3->CCR1;
      
			GPIOA->BSRR |=(1<<21);
			}else{
				falling_time=TIM3->CCR1;
				distance= (falling_time-rising_time)*0.034/2; 
		
    }
    TIM3->SR &= ~TIM_SR_CC1IF;
  }

}


void Pinecho(void){
	
	RCC->AHB1ENR|=(1<<0); //enable GPIO A
	GPIOA->MODER |= (1<<13);// output alterf PIN 6  (write 1 in 10)
	GPIOA->MODER &= ~(1<<12); //output PIN6  (write 0 in 11)
	GPIOA->OTYPER &= ~(1<<6); // output push pull
	GPIOA->OSPEEDR |= (3<<6);//very high speed
	GPIOA->AFR[0] &= ~(1<<0);
	GPIOA->AFR[0] |= (2<<24);
	
	GPIOA->MODER |= (1<<16);// output PIN 8 
	GPIOA->MODER &= ~(1<<17); //output PIN8  
	
}

void TIMecho(void){
	
	
	RCC->APB1ENR|= (1<<1);// enable timer3
	TIM3->PSC =  84-1 ; // 1MHz  (tim2 in APB1 timer)
	TIM3->ARR = 0xffff; // 

	TIM3->CR1 &= ~TIM_CR1_UDIS;
	TIM3->EGR |= TIM_EGR_UG;
	while(!(TIM3->SR & TIM_SR_UIF));
	
	TIM3->CCMR1 &= ~TIM_CCMR1_CC1S;
	TIM3->CCMR1 |= TIM_CCMR1_CC1S_0; // CC1 channel is configured as input
	

	
	
	//TIM3->SMCR |= TIM_SMCR_TS_2 | TIM_SMCR_TS_0 | TIM_SMCR_SMS_2 ; //slave interrupt
	
	// IC1 capture polarity is configured non-inversed
	//TIM3->CCER |= TIM_CCER_CC1P; // rising edge
	//TIM3->CCER &= ~TIM_CCER_CC1P; // rising edge
	TIM3->CCER &= ~TIM_CCER_CC1P;
  TIM3->CCER |= TIM_CCER_CC1P | TIM_CCER_CC1NP;
	
	
	TIM3->CCER|= TIM_CCER_CC1E; // capture enable  CC1E
	
	
	 // Enable TIM3 interrupt
	NVIC_SetPriority(TIM3_IRQn,1);
  NVIC_EnableIRQ(TIM3_IRQn);
	
}

void configchan(void){
	
	TIM3->DIER |= TIM_DIER_CC1IE; //Enable interrupt
	
	TIM3->CCER|= TIM_CCER_CC1E;
	TIM3->CR1 |= TIM_CR1_CEN;
	
}

void configecho (void){
	
	
	Pinecho();
	TIMecho();
	configchan();
	
}

void trigger(void){
	
	GPIOA->BSRR |=(1<<8);
	Delay_us(10);
	GPIOA->BSRR |=(1<<24);
	Delay_ms(60);
	
}

void sendedge(void){
	
	char result[50] ;
	
	sprintf(result, "%d", distance);
	UART2_SendString(result);
	UART2_SendChar('c');
	UART2_SendChar('m');
	UART2_SendChar('\n');
	
	Delay_ms(500);
	
	
	

	
	
	
}

  
  

  
  
  
  
  
  
