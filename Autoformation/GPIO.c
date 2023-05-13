#include "GPIO.h"

void GPIOconfig (void){
	
	RCC->AHB1ENR|=(1<<0); //enable GPIO A
	GPIOA->MODER |= (1<<10);// output PIN 5  (write 1 in 10)
	GPIOA->MODER &= ~(1<<11); //output PIN5  (write 0 in 11)
	GPIOA->OTYPER &= ~(1<<5); // output push pull
	GPIOA->OSPEEDR |= (1<<11) ;// PIN READING SPEED
	GPIOA->PUPDR &= ~((1<<10) | (1<<11));// PULL-UP PIN 5
	
}