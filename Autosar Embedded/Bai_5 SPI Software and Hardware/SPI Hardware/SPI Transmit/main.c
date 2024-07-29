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
	
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_LSB;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	
	SPI_Cmd(SPI1, ENABLE);
	SPI_Init(SPI1,&SPI_InitStruct);
	
}

void SPI1_Send1Byte(uint8_t data){
	GPIO_WriteBit(SPI1_GPIO, SPI1_NSS_Pin, Bit_RESET);
	SPI_I2S_SendData(SPI1,data);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)==0);
	
	GPIO_WriteBit(SPI1_GPIO, SPI1_NSS_Pin, Bit_SET);
}
uint8_t SPI1_Receive1Byte(void){
	uint8_t temp;
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY)==1);
	temp = (uint8_t)SPI_I2S_ReceiveData(SPI1);
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==0);
	return temp;
}
int main()
{
	
}



