#include "RCC_config.h"


void SysClockConfig(void){
	
	#define PLL_M 8
	#define PLL_N 84
	#define PLL_P 0 //PLLP =2
	
	RCC->CR |= RCC_CR_HSEON;
	while ( !(RCC->CR & RCC_CR_HSERDY));
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_VOS;
	FLASH->ACR |= FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_2WS;
	RCC->PLLCFGR = 1<<22; // HSE as PLL
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1; // AHB prscaler
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; //APB1 prscaler
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1; //APB2 prscaler
	
	RCC->PLLCFGR = (PLL_M << 0) | (PLL_N << 6) | (PLL_P << 16) ;
	
	RCC->CR |= RCC_CR_PLLON;
	while (!(RCC->CR & RCC_CR_PLLRDY));
	
	RCC->CFGR &= ~(RCC_CFGR_SW);//reset
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while (!(RCC->CFGR & RCC_CFGR_SWS_PLL));
	SystemCoreClockUpdate();//update
}


