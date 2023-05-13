#include "PowerMode.h"
#include "UART.h"


void CheckSleep(void){
	
	if (PWR->CSR & PWR_CSR_SBF) {
		PWR->CR |=PWR_CR_CWUF;
		PWR->CR |= PWR_CR_CSBF;
		UART2_SendString("I have awaken from standby \n");
	}else{
			UART2_SendString("I have awaken from power cycle");
	}
	
}

void GoToSleep(void){
	
	RCC->APB1ENR |= (1<<28) ; // PWREN
	// Set the Standby mode
    SCB->SCR |= SCB_SCR_SEVONPEND_Msk;  //SLEEPONEXIT  /  SLEEPDEEP  / SEVONPEND (this)
	
	// Set the Standby mode bit in the Power Control Register
    PWR->CR |= PWR_CR_PDDS;
	
	// Clear the Wakeup Pin flag
    PWR->CR |= PWR_CR_CWUF;

	
	//WKUPin
	PWR->CSR |= (1<<8);
	
	

    // Set the Sleep mode
    __WFI();
	
}
