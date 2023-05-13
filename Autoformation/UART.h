#include "stm32f401xe.h"

void UartConfig(void);

void UART2_SendChar (int car);

void UART2_SendString (char *string);

uint8_t UART_GetChar (void);
