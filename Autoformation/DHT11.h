#include "stm32f401xe.h"
#include "timer_config.h"
#include "UART.h"


void DHT11_Start(void);

uint8_t Check_Response (void);

uint8_t DHT11_Read (void);
void DHT_GetData (void);