#include "DHT11.h"
#include <stdio.h>


void DHT11_Start(void){
	
	RCC->AHB1ENR|=(1<<0); //enable GPIO A
	GPIOA->MODER |= (1<<2);// output PIN 1
	GPIOA->MODER &= ~(1<<3); //output PIN 1
	GPIOA->BSRR |= (1<<17);
	Delay_ms(18);
	GPIOA->BSRR |= (1<<1);
	Delay_us(30);
	GPIOA->MODER &= ~(1<<2);// input PIN 1
	GPIOA->MODER &= ~(1<<3); //input PIN 1
}

uint8_t Check_Response (void){
	uint8_t Response = 0;
	Delay_us(80);
	if (!(GPIOA->IDR & 1<<1))
	{
		Delay_us (80);
		if ((GPIOA->IDR & 1<<1)) Response = 1;
		else Response = -1;
	}
	while ((GPIOA->IDR & 1<<1));   // wait for the pin to go low

	return Response;
	
}

uint8_t DHT11_Read (void)
{
	uint8_t value,j;
	for (j=0;j<8;j++)
	{
		while (!(GPIOA->IDR & 1<<1));   // wait for the pin to go high
		Delay_us(40);   // wait for 40 us
		if (!(GPIOA->IDR & 1<<1))   // if the pin is low
		{
			value &= ~(1<<(7-j));   // write 0
		}
		else value|= (1<<(7-j));  // if the pin is high, write 1
		while (GPIOA->IDR & 1<<1);  // wait for the pin to go low
	}
	return value;
}

void DHT_GetData (void)
{
	//uint8_t Presence = Check_Response ();
	uint8_t Rh_byte1 = DHT11_Read ();
	uint8_t Rh_byte2 = DHT11_Read ();
	uint8_t Temp_byte1 = DHT11_Read ();
	uint8_t Temp_byte2 = DHT11_Read ();
	uint8_t SUM = DHT11_Read();
	
	char dis_buf[7];
	char t[50];
	uint8_t Temp= Temp_byte1;
	char message[] = "00.0";
	
	message[0] = (Temp_byte1/10 )+ 48;
  message[1] = (Temp_byte1%10) + 48;
  message[3] = (Temp_byte2%10 )+ 48;
	message[4] = '\n';
	
	UART2_SendString(message);
	
	
	//sprintf( t, "Temperature: %d.%dC\r\n", Temp_byte1,Temp_byte2);//sprintf( dis_buf, "%02u.%1u%cC", ((Temp / 10) % 10) +48, (Temp % 10) +48 );
	sprintf(t,"Temperature: %c%c.%cC\n", 48+(Temp_byte1/10),48+(Temp_byte1%10),48+(Temp_byte2%10));
	UART2_SendString(t);
	sprintf(t,"Humidity: %c%c.%c\n", 48+((Rh_byte1/10)%10),48+(Rh_byte1%10),48+(Rh_byte2%10));
	UART2_SendString(t);
	

}


