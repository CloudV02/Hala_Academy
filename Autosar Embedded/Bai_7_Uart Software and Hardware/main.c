#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

#define UART_TX			GPIO_Pin_9
#define UART_RX			GPIO_Pin_10
#define UART_GPIO		GPIOA

void RCC_Config()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE );
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}
void GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = UART_TX | UART_RX;
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(UART_GPIO, &GPIO_InitStruct);
	
}

void UART_Config(){
	USART_InitTypeDef UART_InitStruct;
	
	UART_InitStruct.USART_BaudRate = 9600;
	
	UART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	UART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	UART_InitStruct.USART_Parity = USART_Parity_No;
	
	UART_InitStruct.USART_StopBits = USART_StopBits_1;
	
	UART_InitStruct.USART_WordLength = USART_WordLength_8b;
	
	USART_Init(USART1, &UART_InitStruct);
	USART_Cmd(USART1,ENABLE);
}

char USART_RecieveChar(USART_TypeDef *USARTx){
	USARTx->DR = 0x00;
	uint8_t tmp_data;
	tmp_data = USART_ReceiveData(USARTx);
	
	while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE)!=1);
	
	return tmp_data;
}

void USART_SendChar(USART_TypeDef *USARTx, uint8_t data){
	USARTx->DR = 0x00;
	USART_SendData(USARTx,data);
	
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) != 1);
}
void USART_StringSend(USART_TypeDef *USARTx, uint8_t *data){
	while(*data){
		USART_SendData(USARTx, *data);
		data++;
	}
}

int main(){
	RCC_Config();
	GPIO_Config();
	UART_Config();
	
	while(1){
		
	}
}
