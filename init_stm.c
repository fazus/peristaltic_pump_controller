#include "init_stm.h"


void init_stm(void){

	if (!(RCC->CR2 & (1 << 17))) {
			/* Enable HSI */
		RCC->CR2 |= 1 << 16;
			/* Wait for HSI to be ready */
			while (!(RCC->CR2 & (1 << 17)));

	}
	RCC->CFGR = 0x00000003;
		/* wait until the HSI48 is the clock source */
		while ((RCC->CFGR & 0xc) != 0xc);

		tim3_cen();


}

void tim3_cen(void)
{   RCC->APB1ENR |= 0x00000002;
    TIM3->PSC         = 10;
    TIM3->ARR         = 50;
    TIM3->CR1     = 0x0280;
    TIM3->DIER        = 0x0001;
    NVIC_EnableIRQ(TIM3_IRQn);


    TIM3->CR1        |= 0x0001;}
