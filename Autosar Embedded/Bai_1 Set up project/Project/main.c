#include <stm32f10x.h>


/**Function prototype**/
void RCC_Config();
void GPIO_Config();
void Led_Toggle();

/**Function main**/
int main()
{
	RCC_Config();
	GPIO_Config();
	while(1){
		if((GPIOA->IDR & (1<<1))==0){
			while((GPIOA->IDR & (1<<1))==0);
			Led_Toggle();
		}
	}
}

/****/
void RCC_Config()
{
		RCC->APB2ENR |= RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPAEN;
}

void GPIO_Config()
{
	
	/*Input PA0*/
	GPIOA->CRL &= ~(3<<4);
	GPIOA->CRL &= ~(3<<6);
	GPIOA->CRL |= (2<<6);
	GPIOA->ODR |= (1<<1);
	
	/*Output PC13*/
	GPIOC->CRH &= ~(3<<20);
	GPIOC->CRH |= (3<<20);
	GPIOC->CRH &= ~(3<<22); 
}


void Led_Toggle()
{
	if(GPIOC->ODR & 1<<13){
		GPIOC->ODR &= ~(1<<13);
	}
	else{
		GPIOC->ODR |= (1<<13);
	}
}
