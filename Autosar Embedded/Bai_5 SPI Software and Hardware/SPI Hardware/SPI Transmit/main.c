#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_spi.h"              // Keil::Device:StdPeriph Drivers:SPI
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC

#define SPI1_NSS_Pin 				GPIO_Pin_4
#define SPI1_SCK_Pin				GPIO_Pin_5
#define SPI1_MISO_Pin				GPIO_Pin_6
#define SPI1_MOSI_Pin				GPIO_Pin_7
#define SPI1_GPIO						GPIOA

void RCC_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
}

void GPIO_Config(){
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = SPI1_NSS_Pin | SPI1_SCK_Pin | SPI1_MISO_Pin | SPI1_MOSI_Pin;
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(SPI1_GPIO, &GPIO_InitStruct);
}
void SPI_Config(){
	SPI_InitTypeDef SPI_InitStruct;
	
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	
	
	SPI_InitStruct.SPI_NSS = ENABLE;
}

int main()
{
	
}



