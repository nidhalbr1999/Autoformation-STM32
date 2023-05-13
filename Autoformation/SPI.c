#include "SPI.h"

void SPI_Config(void){
	
	RCC->APB2ENR |= 1<<12;
	SPI1->CR1 &= ~(1<<0); //phase
	SPI1->CR1 &= ~(1<<1); //polarity
	SPI1->CR1 |= 1<<2 ;//master mode
	SPI1->CR1 |= (2<<3);// SPI Clock div 8
	SPI1->CR1 &=~(1<<7) ;//LSBfirst=0
	SPI1->CR1 |=(1<<8)|(1<<9) ;//SSM=SSi=1 software slave manager !!!!!  if SSM=1 any io can control slave / else if SSM=0 Slave can be controled only by NSS pin
	SPI1->CR1 &=~(1<<10) ;// RXONLY ,full_duplex
	SPI1->CR1 &=~(1<<11) ;//8bits
	SPI1->CR2 =0;
	
}

void GPIO_conf(void){
	
	RCC->AHB1ENR |= 1<<0;
	GPIOA->MODER |= (2<<10) | (2<<12) |(2<<14) |(1<<18);//alt function PA5,PA6,PA7,PA9
	GPIOA->OSPEEDR |= (3<<10) | (3<<12) |(3<<14) |(3<<18);
	GPIOA->AFR[0] |= (5<<20)|(5<<24)|(5<<28); // PA5 SCK , PA6 MISO , PA7 MOSI
}

void SPI_Enable(void){
	SPI1->CR1 |= 1<<6;
}
void SPI_Disable(void){
	SPI1->CR1 &= ~(1<<6);
}
void CS_Enable (void)
{
	GPIOA->BSRR |= (1<<9)<<16;
}

void CS_Disable (void)
{
	GPIOA->BSRR |= (1<<9);
}

void SPI_Transmit (uint8_t *data, int size)
{
	
	/*
	1. Wait for the TXE bit to set in the Status Register
	2. Write the data to the Data Register
	3. After the data has been transmitted, wait for the BSY bit to reset in Status Register
	4. Clear the Overrun flag by reading DR and SR
	*/		
	
	int i=0;
	while (i<size)
	{
	   while (!((SPI1->SR)&(1<<1))) {};  // wait for TXE bit to set -> This will indicate that the buffer is empty
	   SPI1->DR = data[i];  // load the data into the Data Register
	   i++;
	}	
	
	
/*During discontinuous communications, there is a 2 APB clock period delay between the
write operation to the SPI_DR register and BSY bit setting. As a consequence it is
mandatory to wait first until TXE is set and then until BSY is cleared after writing the last
data.
*/
	while (!((SPI1->SR)&(1<<1))) {};  // wait for TXE bit to set -> This will indicate that the buffer is empty
	while (((SPI1->SR)&(1<<7))) {};  // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication	
	
	//  Clear the Overrun flag by reading DR and SR
	uint8_t temp = SPI1->DR;
					temp = SPI1->SR;
	
}

void SPI_Receive (uint8_t *data, int size)
{
	/*
	1. Wait for the BSY bit to reset in Status Register
	2. Send some Dummy data before reading the DATA
	3. Wait for the RXNE bit to Set in the status Register
	4. Read data from Data Register
	*/		

	while (size)
	{
		while (((SPI1->SR)&(1<<7))) {};  // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication
		SPI1->DR = 0;  // send dummy data
		while (!((SPI1->SR) &(1<<0))){};  // Wait for RXNE to set -> This will indicate that the Rx buffer is not empty
	  *data++ = (SPI1->DR);
		size--;
	}	
}



float xg, yg, zg;
int16_t x,y,z;
uint8_t RxData[6];
	
void adxl_write (uint8_t address, uint8_t value)
{
	uint8_t data[2];
	data[0] = address|0x40;  // multibyte write
	data[1] = value;
	CS_Enable ();  // pull the cs pin low
	SPI_Transmit (data, 2);  // write data to register
	CS_Disable ();  // pull the cs pin high
}
	

void adxl_read (uint8_t address)
{
	address |= 0x80;  // read operation
	address |= 0x40;  // multibyte read
	uint8_t rec;
	CS_Enable ();  // pull the pin low
	SPI_Transmit (&address, 1);  // send address
	SPI_Receive (RxData, 6);  // receive 6 bytes data
	CS_Disable ();  // pull the pin high
}

void adxl_init (void)
{
	adxl_write (0x31, 0x01);  // data_format range= +- 4g
	adxl_write (0x2d, 0x00);  // reset all bits
	adxl_write (0x2d, 0x08);  // power_cntl measure and wake up 8hz
}





