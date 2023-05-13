#include "UART.h"

void UartConfig(void){
	
	RCC->AHB1ENR |= (1<<0); //GPIOA
	RCC->APB1ENR |= (1<<17);// uart 2
	
	GPIOA->MODER |= (2<<4); //AF for PIN A2
	GPIOA->MODER |= (2<<6); //AF for PIN A3
	GPIOA->OSPEEDR |= ((3<<4) | (3<<6));// output speed
	
	GPIOA->AFR[0] |= (7<<8);
	GPIOA->AFR[0] |= (7<<12); 
	USART2->CR1 = 0x0000; //CR1 reset for 8 bits
	
	USART2->BRR = 0x16D;	//baud rate  TXRX=115 200
	
	USART2->CR1 |= (1<<2); //rx enable
	USART2->CR1 |=  (1<<3); // tx enable
	USART2->CR1 |= (1<<13); //usart enable
	
}

void UART2_SendChar (int car){
	
USART2->DR =car;
while (!(USART2->SR & (1<<6)));	
	
}

void UART2_SendString (char *string){
	
	while (*string){ 
		UART2_SendChar (*string++);
	}
}


uint8_t UART_GetChar (void){
	
	uint8_t temp;
	while (!(USART2->SR & (1<<5)));
	temp=USART2->DR;
	return temp;

}
