#include "PWM.h"

void GPIOinit(void){


	RCC->AHB1ENR|=(1<<1); //enable GPIO B
	
	GPIOB->MODER |= (1<<13);// output alterf PIN 6  (write 1 in 10)
	GPIOB->MODER &= ~(1<<12); //output PIN6  (write 0 in 11)
	GPIOB->OTYPER &= ~(1<<6); // output push pull
	GPIOB->OSPEEDR |= (3<<6);//very high speed
	GPIOB->AFR[0] &= ~(1<<0);
	GPIOB->AFR[0] |= (2<<24);
}	
	
	
void PWMConfig(void){
	
	GPIOinit();
	RCC->APB1ENR|= (1<<2);// enable timer4
	
	//pwm freq=Fclk/PSC/ARR       50 * 28 * 60000 = 84000000
	
	TIM4->PSC =  84-1 ; //84MHz / 50Hz = 1680000 (tim2 in APB1 timer)
	TIM4->ARR = 20000; // 
	TIM4->CCR1 =2000 ; //180:2000   90:1500  0:1000
	TIM4->CCMR1 &= ~(3<<0); // CC1 channel is configured as output
	TIM4->CCMR1|= 6<<4;//Output compare 1 mode 110: PWM mode 1
	TIM4->CCMR1|= 1<<3;//Output compare 1 preload enable
	TIM4->CCER|= 1<<0; // enable channel 1
//	TIM3->DIER |= (1 << 0); //Enable update interrupt)

	TIM4-> BDTR |= TIM_BDTR_MOE;
	 TIM4-> CR1 |= TIM_CR1_ARPE;
	 TIM4-> EGR |= TIM_EGR_UG;
	TIM4->CR1 |= (1<<0); // counter enable
	

	
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void servo_write(int angle)
	{
	
	if(angle<0){angle=0;}
	if(angle>180){angle=180;}
	
	
TIM4->CCR1=map (angle,0,180,1000,2000);
	
	
	
	}

