#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

/*GPIO  SPI SS keo xuong truoc, Chan Clock, Chan Truyen -> data*/
#define SPI_SCK_Pin 			GPIO_Pin_1
#define SPI_MOSI_Pin 			GPIO_Pin_2
#define SPI_MISO_Pin 			GPIO_Pin_3
#define SPI_SS_Pin 				GPIO_Pin_4
#define SPI_InitTypeDef 	GPIO_InitTypeDef
#define SPI_GPIO					GPIOC
#define SPI_RCC						RCC_APB2Periph_GPIOC

void RCC_Config(){
	
	RCC_APB2PeriphClockCmd(SPI_RCC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
}

void SPI_Cofnig(){
	SPI_InitTypeDef SPI_InitStruct;
	
	/* 4 pin for GPIO_Pin_1 = SCK, GPIO_Pin_2 = MOSI, GPIO_Pin_3 = MISO, GPIO_Pin_4 = SS*/
	SPI_InitStruct.GPIO_Pin |= SPI_SCK_Pin | SPI_MOSI_Pin | SPI_SS_Pin;
	
	/* Config output for SS, SCK, MOSI*/
	SPI_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	
	SPI_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(SPI_GPIO, &SPI_InitStruct);
	
	
	/*Config input for MISO*/
	SPI_InitStruct.GPIO_Pin |= SPI_MISO_Pin;
	
	SPI_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	
	SPI_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &SPI_InitStruct);
	
}

void TIM_BASE_Config(){
	TIM_TimeBaseInitTypeDef TIM_BASE_InitStruct;
	
	// 36MHz 36000000
	TIM_BASE_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	
	// 36000 * 1/36Mhz -> 1/1000
	TIM_BASE_InitStruct.TIM_Prescaler = 36000 -1;
	
	TIM_BASE_InitStruct.TIM_Period = 0xFFFF;
	
	TIM_BASE_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	
	
	TIM_Cmd(TIM4, ENABLE);
	TIM_TimeBaseInit(TIM2, &TIM_BASE_InitStruct);
}

/*function delay giay*/
void delay_s(uint16_t delays){ // 1 /1000 = 0.001 -> 0.001 * 1000 = 1s
		TIM_SetCounter(TIM2, 0);
		while(TIM_GetCounter(TIM2) < delays*1000);
}
/*function delay m giay*/
void delay_ms(uint16_t delayms){	// 1 /1000 = 0.001
		TIM_SetCounter(TIM2, 0);
		while(TIM_GetCounter(TIM2) < delayms);
}

void SPI_Clock(){
	// #define SPI_SCK_Pin 			GPIO_Pin_1
	GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, 0);
	delay_ms(4);
	GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, 1);
	delay_ms (4);
}

void SPI_Init(){
	GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, 0);
	GPIO_WriteBit(SPI_GPIO, SPI_SS_Pin, 1);
	GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, 0);
	GPIO_WriteBit(SPI_GPIO, SPI_MISO_Pin, 0);
}

void SPI_Transmit(uint8_t data_Transmit){
	// SS = 0 -> Clock -> If clock = 1 -> Truyen data
	uint8_t buffer_trans = 0;
	
	GPIO_WriteBit(SPI_GPIO,SPI_SS_Pin,0);
	delay_ms(1);
	for(uint32_t i = 0; i < sizeof(data_Transmit)*8; i++ ){
		buffer_trans = ((data_Transmit << i)&0x10000000);
		GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, buffer_trans);
		SPI_Clock();
	}
	GPIO_WriteBit(SPI_GPIO,SPI_SS_Pin,1);
	delay_ms(1);
}

uint8_t SPI_Receive(){
	uint8_t recei_data = 0;
	uint8_t recei_buffer = 0;
	while(GPIO_ReadInputDataBit(SPI_GPIO,SPI_SS_Pin)!=0);
	while(GPIO_ReadInputDataBit(SPI_GPIO,SPI_SCK_Pin)==0);
	
	for(int i = 0; i< 8 ; i++){
		if(GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin)!=0){
			while(GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin) != 0)
				recei_buffer = GPIO_ReadInputDataBit(SPI_GPIO, SPI_MOSI_Pin);
			recei_data =  recei_data << i;
			recei_data |= recei_buffer;
		}
		while(GPIO_ReadInputDataBit(SPI_GPIO,SPI_SCK_Pin)==0);
	}
	return recei_data;
}
	
int main(){
	uint8_t data[] = {};
	RCC_Config();
	SPI_Cofnig();
	SPI_Init();
	while(1)
		{	
        while(GPIO_ReadInputDataBit(SPI_GPIO,SPI_SS_Pin)!=0);
        if(GPIO_ReadInputDataBit(SPI_GPIO, SPI_SS_Pin)==0){
            for(int i = 0; i<8; i++){
            data[i] = SPI_Receive();
            }
        }
		}

}
