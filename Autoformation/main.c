#include "RCC_config.h"
#include "timer_config.h"
#include "ADC.h"
#include "UART.h"
#include "GPIO.h"
#include "I2C_config.h"
#include "LCD2.h"
#include "stdlib.h"
#include<stdio.h>
#include "PWM.h"
#include "UltraSonic.h"
#include "DHT11.h"
#include "SPI.h"

#include "ExInterrupt.h"
#include "DMA+adc.h"
#include "DMA+uart.h"
#include "PowerMode.h"
#include "TimerInterrupt.h"
#include "RTC.h"
#include "WDG.h"



uint16_t var;
char str[40];


int main(void){
	
	SysClockConfig();
	TIM2config();
	GPIOconfig();
	UartConfig();
	
	//TIM4 Periodic Interrupt
	
	//TIM4config();
	
	RTCinit();
	
	ADC_Config();
	ADC_Enable();
	uint16_t val_ADC[2];
	char a[20];
	
	GPConf();
	InterruptConf();
	
	GoToSleep();
	IWDG_init();
	
	while (1){
		
		
	
	
		
	}
}
