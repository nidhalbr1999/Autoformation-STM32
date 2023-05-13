#include "DMA+uart.h"
#include "UART.h"
#include <string.h>

void UartConfig2(void){
	
	RCC->AHB1ENR |= (1<<0); //GPIOA
	RCC->APB1ENR |= (1<<17);// uart 2
	
	GPIOA->MODER |= (2<<4); //AF for PIN A2
	GPIOA->MODER |= (2<<6); //AF for PIN A3
	GPIOA->OSPEEDR |= ((3<<4) | (3<<6));// output speed
	
	GPIOA->AFR[0] |= (7<<8);
	GPIOA->AFR[0] |= (7<<12); 
	USART2->CR1 = 0x0000; //CR1 reset for 8 bits
	
	//USART2->BRR = (7<<0) | (273<<4);	//baud rate  TXRX=9600
	
	USART2->BRR = 0x16D;	//baud rate  TXRX=115 200
	
	USART2->CR1 |= (1<<2); //rx enable
	USART2->CR1 |=  (1<<3); // tx enable
	USART2->CR1 |= (1<<13); //usart enable
	USART2->CR3 |= (1<<6);  // enable DMA for reception
	
}

void DMA_Init1 (void)
{
	/*
	1. Enable DMA clock
	2. Set the DATA Direction
	3. Enable/Disable the Circular Mode
	4. Enable/Disable the Memory Increment and Peripheral Increment
	5. Set the Data Size
	6. Select the channel for the Stream
*/
	
	// Enable the DMA1 Clock
	RCC->AHB1ENR |= (1<<21);  // DMA1EN = 1
	
	
	// 2. Enable DMA Interrupts
	DMA1_Stream5->CR |= (1<<2)|(1<<3)|(1<<4);  // TCIE, HTIE, TEIE Enabled
	

	// Select the Data Direction
	DMA1_Stream5->CR &= ~(3<<6);  // Peripheral to memory
	
	// Select Circular mode
	DMA1_Stream5->CR |= (1<<8);  // CIRC = 1
	
	// Enable Memory Address Increment
	DMA1_Stream5->CR |= (1<<10);  // MINC = 1;

	// Set the size for data 
	DMA1_Stream5->CR &= ~((1<<11)|(1<<13));  // PSIZE = 00, MSIZE = 00, 8 bit data
	
	// Select channel for the stream
	DMA1_Stream5->CR &= ~(3<<25);  // Channel 4 selected //
	DMA1_Stream5->CR |= (1<<27);
	
	DMA1_Stream5->CR  |= (1<<16);// PL priority
	
}


void DMA_Config1 (uint32_t srcAdd, uint32_t destAdd, uint16_t size)
{
	
	/*
	1. Set the Data Size in the NDTR Register
	2. Set the Peripheral Address and the Memory Address
	3. Enable the DMA Stream
		 
	*/
	
	DMA1_Stream5->NDTR = size;   // Set the size of the transfer
	
	DMA1_Stream5->PAR = srcAdd;  // Source address is peripheral address
	
	DMA1_Stream5->M0AR = destAdd;  // Destination Address is memory address
	
	// Enable the DMA Stream
	DMA1_Stream5->CR |= (1<<0);  // EN =1
	
	NVIC_SetPriority (DMA1_Stream5_IRQn, 0);
	NVIC_EnableIRQ (DMA1_Stream5_IRQn);
}
#define RXSIZE 3
uint8_t RxBuf[6];
uint8_t MainBuf[6];

uint8_t indx=0;


void DMA1_Stream5_IRQHandler (void)
{
	if ((DMA1->HISR)&(1<<10))  // If the Half Transfer Complete Interrupt is set     //HISR stream 5 -> 7
	{
		memcpy (&MainBuf[indx], &RxBuf[0], RXSIZE/2);
		DMA1->HIFCR |= (1<<10);
		indx = indx+(RXSIZE/2);
		if (indx>49) indx=0;
	}
	
	if ((DMA1->HISR)&(1<<11))  // If the Transfer Complete Interrupt is set
	{
		memcpy (&MainBuf[indx], &RxBuf[RXSIZE/2], RXSIZE/2);
		DMA1->HIFCR |= (1<<11);
		indx = indx+(RXSIZE/2);
		if (indx>49) indx=0;
	}
}
