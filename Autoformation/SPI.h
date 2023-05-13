#include "stm32f401xe.h"

void SPI_Config(void);

void GPIO_conf(void);

void SPI_Enable(void);

void SPI_Disable(void);

void CS_Enable (void);
	
void CS_Disable (void);

void SPI_Transmit (uint8_t *data, int size);

void SPI_Receive (uint8_t *data, int size);

void adxl_write (uint8_t address, uint8_t value);

void adxl_read (uint8_t address);

void adxl_init (void);





