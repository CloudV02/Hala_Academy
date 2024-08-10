#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_i2c.h"              // Keil::Device:StdPeriph Drivers:I2C
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

#define I2C_SCL			GPIO_Pin_6
#define I2C_SDA			GPIO_Pin_7
#define I2C_GPIO		GPIOB

void RCC_Config(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1 | RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
}

void GPIO_Config(){
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = I2C_SCL|I2C_SDA;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(I2C_GPIO,&GPIO_InitStruct);
}
void TIM_BaseConfig(){
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1; //36mhz
	TIM_InitStruct.TIM_Prescaler = 36000-1;
	TIM_InitStruct.TIM_Period = 0xFFFF;
	TIM_Cmd(TIM2,ENABLE);
	TIM_TimeBaseInit(TIM2,&TIM_InitStruct);
}
void delay_us(uint16_t delays){
	TIM_SetCounter(TIM2,0);
	while(TIM_GetCounter(TIM2) < delays *0.001);
}

void I2C_Config(){
	GPIO_SetBits(I2C_GPIO,I2C_SDA);
	delay_us(3);
	GPIO_SetBits(I2C_GPIO,I2C_SCL);
	delay_us(3);
}
void I2C_Start(){
	GPIO_ResetBits(I2C_GPIO,I2C_SDA);
	delay_us(3);
	GPIO_ResetBits(I2C_GPIO,I2C_SCL);
	delay_us(3);
}

int main()
{
	
}

