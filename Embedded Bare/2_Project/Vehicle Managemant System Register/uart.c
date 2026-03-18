#include "uart.h"


void Terminal_UART_Init(void)
{
	/*RCC for UART and GPIO of UART*/
	RCC->APB2ENR |= (1<<14);
	RCC->APB2ENR |= (1<<2);
	
	/*UART using PA9 - PA10*/
	GPIOA->CRH &= ~(3<<4);
	GPIOA->CRH |= (3<<4);
	GPIOA->CRH &= ~(3<<6);
	GPIOA->CRH |= (2<<6);
	
	GPIOA->CRH &= ~(3<<8);
	GPIOA->CRH |= (3<<8);
	GPIOA->CRH &= ~(3<<10);
	GPIOA->CRH |= (2<<10);
	
	/*Set baudrate
	* take Clock of system / baudrate 
	* default clock = 8Mhz / 9600 = 833.333 == 0x341;
	*/
	USART1 -> BRR = 0x341;
	
	/*Enable TX and RX*/
	USART1 -> CR1 |= (1<<3); // TE bit
	USART1 -> CR1 |= (1<<2); // RE bit
	
	/*Enable IRQ RX*/
	USART1 -> CR1 |= (1<<5); // RXNEIE bit
	
	/*NVIC*/
	/*NVIC->ISER[1] |= (1 << 5);   // USART1 interrupt if don't use CMSIS*/
	NVIC_EnableIRQ(USART1_IRQn);
	
	/*Set Priority
	* NVIC->IP[USART1_IRQn] = 2 << 4; Cortex-M3 only use 4 bit cao of priority field.
	* |7 6 5 4|3 2 1 0|
		priority   unused
	*/
	NVIC_SetPriority(USART1_IRQn, 2);
	
	/*Enable USART*/
	USART1 -> CR1 |= (1<<13); // UE bit
	
}

void SendData(uint8_t *data, uint16_t lengthData){
	uint16_t counter = 0;
	
	for(counter = 0; counter < lengthData; counter++){
		/*Check buffer transmit data empty
		bit TXE = 1 meaning buffer empty -> while check TXE empty
		*/
		while(!((USART1->SR) & (1<<7))); // check bit 7(TXE) = 0 or 1
		USART1 ->DR = data[counter];
	}
}

uint8_t RevData(void){
		/*Check buffer recive data empty
		bit RXNE = 1 meaning buffer empty -> while check RXNE empty*/
		while(!((USART1->SR) & (1<<5))); // check bit 5(RXNE) = 0 or 1
		return USART1 -> DR;
}

// Using IRQ
uint8_t data;

void USART1_IRQHandler(void){
	/*clear bit*/
	USART1 -> SR &= ~(1<<5);
	
	data = USART1->DR;
}

