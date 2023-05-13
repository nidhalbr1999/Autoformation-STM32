#include "I2C_config.h"

void I2C_configuration2(void){
	
	RCC->APB1ENR |= (1<<21); // enable I2C
	RCC->AHB1ENR |= (1<<1); // enable GPIO B
	GPIOB->MODER &= 0;
	GPIOB->MODER |= (2<<18) | (2<<16);// AF For pin 8 and 9
	GPIOB->OTYPER |= (1<<8) | (1<<9); // output open drain
	GPIOB->OSPEEDR |= (3<<16) | (3<<18);// very high speed
	GPIOB->PUPDR |= (1<<16) | (1<<18);
	GPIOB->AFR[1] |= (4<<0) | (4<<4);
	
	//I2C
	
	I2C1->CR1 |= (1<<15); //reset
	I2C1->CR1 &= ~(1<<15);
	
	I2C1->CR2 |= (42<<0); //set clock freq
	
	I2C1->CCR |= 210<<0; // clock control reg  217
	I2C1->TRISE |= (43<<0);// 42+1
	
	I2C1->CR1 |= (1<<0);
	
	
}

void I2C_start2 (void){

	I2C1->CR1 |= (1<<10);//Enable the ACK
	I2C1->CR1 |= (1<<8);// generate start
	while (! (I2C1->SR1 & (1<<0))); // wait for sb bit to set

}

void I2C_Send2(uint8_t data){
	while (!(I2C1->SR1 & (1<<7)));  // wait for TXE bit to set
	I2C1->DR = data;
	while (!(I2C1->SR1 & (1<<2)));  // wait for BTF bit to set (Byte transfer finished)
	
}

void I2C_address2 (uint8_t address){
	
	I2C1->DR = address;  //  send the address
	while (!(I2C1->SR1 & I2C_SR1_ADDR));  // wait for ADDR bit to set
	uint32_t temp = (I2C1->SR2);  // read SR1 and SR2 to clear the ADDR bit (recommended in slave mode)
	
}// rq address is on 8bits = 7 bits for slave + 1 bit for read or write


 void I2C_STOP2 (void){
 
	 I2C1->CR1 |= (1<<9);
 
 }
 
 void I2C_MultiSend2 (uint8_t *data, uint8_t size){
	 
	 while (!(I2C1->SR1 & (1<<7))); // wait for TXE bit to set
	 
	 while (size){
	 
		 I2C1->DR =(volatile uint32_t) *data++;
		 while (!(I2C1->SR1 & (1<<7))); // wait for TXE bit to set
		 size--;
	 }
	 
	 while (!(I2C1->SR1 & (1<<2)));  // wait for BTF bit to set
	 
 }
 
 void I2C_Message2(char data){
 
	 I2C_start2();
	 I2C_address2(0x4E);
	 I2C_Send2(data);
	 I2C_STOP2();

 
 }
 
 void I2C_Read2(uint8_t Address,uint8_t *buffer, uint8_t size){
 
 int remaining = size;
	
/**** STEP 1 ****/	
	if (size == 1)
	{
		/**** STEP 1-a ****/	
		I2C1->DR = Address;  //  send the address
		while (!(I2C1->SR1 & (1<<1)));  // wait for ADDR bit to set
		
		/**** STEP 1-b ****/	
		I2C1->CR1 &= ~(1<<10);  // clear the ACK bit 
		uint32_t temp = I2C1->SR2;  // read SR1 and SR2 to clear the ADDR bit.... EV6 condition
		I2C1->CR1 |= (1<<9);  // Stop I2C

		/**** STEP 1-c ****/	
		while (!(I2C1->SR1 & (1<<6)));  // wait for RxNE to set
		
		/**** STEP 1-d ****/	
		buffer[size-remaining] = I2C1->DR;  // Read the data from the DATA REGISTER
		
	}

/**** STEP 2 ****/		
	else 
	{
		/**** STEP 2-a ****/
		I2C1->DR = Address;  //  send the address
		while (!(I2C1->SR1 & (1<<1)));  // wait for ADDR bit to set
		
		/**** STEP 2-b ****/
		uint8_t temp = I2C1->SR1 | I2C1->SR2;  // read SR1 and SR2 to clear the ADDR bit
		
		while (remaining>2)
		{
			/**** STEP 2-c ****/
			while (!(I2C1->SR1 & (1<<6)));  // wait for RxNE to set
			
			/**** STEP 2-d ****/
			buffer[size-remaining] = I2C1->DR;  // copy the data into the buffer			
			
			/**** STEP 2-e ****/
			I2C1->CR1 |= 1<<10;  // Set the ACK bit to Acknowledge the data received
			
			remaining--;
		}
		
		// Read the SECOND LAST BYTE
		while (!(I2C1->SR1 & (1<<6)));  // wait for RxNE to set
		buffer[size-remaining] = I2C1->DR;
		
		/**** STEP 2-f ****/
		I2C1->CR1 &= ~(1<<10);  // clear the ACK bit 
		
		/**** STEP 2-g ****/
		I2C1->CR1 |= (1<<9);  // Stop I2C
		
		remaining--;
		
		// Read the Last BYTE
		while (!(I2C1->SR1 & (1<<6)));  // wait for RxNE to set
		buffer[size-remaining] = I2C1->DR;  // copy the data into the buffer
	}	
}
 



