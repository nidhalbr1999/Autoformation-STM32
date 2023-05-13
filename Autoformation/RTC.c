#include "RTC.h"
#include "UART.h"
#include <stdio.h>

typedef struct{
	
	uint8_t Hours;
	uint8_t Minutes;
	uint8_t Seconds;

}RTC_TimeTypeDef;

typedef struct{
	
	uint8_t Month;
	uint8_t Day;
	uint8_t Year;

}RTC_DateTypeDef;

RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;



void RTCinit(void){

    // Enable the RTC clock and backup domain access
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_DBP;

    // Set the LSE oscillator as the RTC clock source
    RCC->BDCR |= RCC_BDCR_LSEON;
    while(!(RCC->BDCR & RCC_BDCR_LSERDY));
    RCC->BDCR |= RCC_BDCR_RTCSEL_0;

    // Initialize the RTC
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;
    RTC->ISR |= RTC_ISR_INIT;
	RCC->BDCR |= RCC_BDCR_RTCEN;
    while(!(RTC->ISR & RTC_ISR_INITF));
    RTC->PRER = (127 << 16) | 255; // set prescaler for 1Hz clock    PREDIV_A = (LSE frequency / 128) - 1 PREDIV_S = 127 For example, if the LSE frequency is 32.768 kHz, then PREDIV_A = 255 and PREDIV_S = 127.
    RTC->CR &= ~RTC_CR_FMT; // 24-hour format
    RTC->ISR &= ~RTC_ISR_INIT;

    // Enable the RTC interrupts (optional)
    RTC->CR |= RTC_CR_WUTIE;
    NVIC_EnableIRQ(RTC_WKUP_IRQn);

    // Enter low-power mode with RTC wakeup
    //PWR->CR &= ~PWR_CR_PDDS; // enter standby mode
    //PWR->CR |= PWR_CR_CWUF; // clear wakeup flag
    RTC->WUTR =100; // set wakeup time
    RTC->CR |= RTC_CR_WUTE; // enable wakeup
    PWR->CR |= PWR_CR_LPDS; // enter low-power deep sleep
	}

void RTC_get_time(void){
	
	char t[20];
	
	sTime.Seconds=(((RTC->TR & 0x7f) >> 4)*10)+(RTC->TR & 0xf);
	sTime.Minutes = ((RTC->TR & 0x7f00) >> 8);
	
	sTime.Minutes=(((sTime.Minutes & 0x7f) >> 4)*10)+(sTime.Minutes & 0xf);
	sTime.Hours = ((RTC->TR & 0x7f0000) >> 16);
	sTime.Hours = (((sTime.Hours & 0x7f) >> 4)*10)+(sTime.Hours & 0xf);
	
	sprintf(t,"%d:%d:%d\n",sTime.Hours,sTime.Minutes,sTime.Seconds);
	UART2_SendString(t);
	
	
}


void RTC_get_date(void){
	
	char t[20];
	
	sDate.Year=((RTC->DR >>20)*10)+((RTC->DR >> 16)& 0xf);
	sDate.Month = ((RTC->DR >>12) & 1)*10+((RTC->DR >>8) & 0xf);
	sDate.Day = ((RTC->DR >> 4)&3)*10+(RTC->DR & 0xf);
	
	sprintf(t,"%d/%d/%d\n",sDate.Day,sDate.Month,sDate.Year);
	UART2_SendString(t);
}

void RTC_WKUP_IRQHandler(void) {
    RTC->ISR &= ~RTC_ISR_WUTF; // clear wakeup flag
	UART2_SendChar('n');
    // do something
}



