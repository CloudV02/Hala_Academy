#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC

void RCC_Config(void);
void GPIO_Config(void);
void delay(uint64_t delaytime);
void chaseLed(uint8_t loop);
void Read_Input();
void NhapNhay();
int main(){
	RCC_Config();
	GPIO_Config();
	while(1)
		{
			NhapNhay();
		}
		
}

void RCC_Config(void){
	/*Su dung ham de cap xung*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE); // tuong duong RCC-> APB2 |= GPIOC|GPIOA
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
}
void GPIO_Config(void){
	/*Cau hinh chan GPIOC*/
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	/*Cau hinh chan xong se dua no den dia chi cua chan GPIO tren MCU*/
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	/*Cau hinh chan GPIOA*/
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	
	/*Cau hinh chan xong se dua no den dia chi cua chan GPIO tren MCU*/
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
void delay(uint64_t delaytime){
	for(int i = 0; i < delaytime;delaytime++);
}
void chaseLed(uint8_t loop){
	uint16_t LedVal;
	for(int i = 0; i < loop; i++){
		LedVal = 0x0010;
		for(int j = 0; j<4; j++){
			LedVal = LedVal << 1;
			GPIO_Write(GPIOC, LedVal);
			delay(10000000);
		}
	} 
}
void Read_Input(){
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0){
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0);
		if(GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_13)){
			GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		}
		else{
			GPIO_SetBits(GPIOC, GPIO_Pin_13);
		}
	}
}
void NhapNhay(){
	GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
	delay(5000000);
	GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
	delay(5000000);
}
