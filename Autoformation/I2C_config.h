#include "stm32f401xe.h"

void I2C_configuration2(void);

void I2C_start2 (void);
	
void I2C_Send2(uint8_t data);
	
void I2C_address2 (uint8_t address);
	
void I2C_STOP2 (void);
	
void I2C_MultiSend2 (uint8_t *data, uint8_t size);

void I2C_Message2(char data);

void I2C_Read2 (uint8_t Address,uint8_t *buffer, uint8_t size);

