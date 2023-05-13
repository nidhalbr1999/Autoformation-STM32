#include "WDG.h"



void IWDG_init(void){
	
	
	// Enable the IWDG clock
  RCC->APB1ENR |= RCC_APB1ENR_WWDGEN;

  // Set the prescaler value to 4
  IWDG->PR = 0x06;

  // Set the reload value to get a 512ms watchdog timeout
  IWDG->RLR = 4096;

  // Enable the IWDG
  IWDG->KR = 0xCCCC;
	
	
}


void IWDG_refresh(void){
	
	IWDG->KR = 0xAAAA;
	
	
}


void WWDG_init(void){
	
	
	// Enable the IWDG clock
  RCC->APB1ENR |= RCC_APB1ENR_WWDGEN;

  // Set the prescaler value and the window watchdog counter value
    WWDG->CFR = (2<<7) | WWDG_CFR_W_6 | WWDG_CFR_W_0; // Prescaler = 8, Window = 0x5F, Counter = 0x7F; // Prescaler = 8, Counter = 0x7F

    // Enable the WWDG and set the early wakeup interrupt
    WWDG->CR = WWDG_CR_WDGA ;
	WWDG->CFR |= WWDG_CFR_EWI;
	
}

