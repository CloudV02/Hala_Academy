#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

void RCC_Config(){
	/**/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
}

void TIM_Config(){
	TIM_TimeBaseInitTypeDef Tim_InitStruct;
	
	/*Chia nho xung dau vao, clock cap cho timer */
	Tim_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1; // Clock cap cho tim 2 la 72mhz: 1s tao dc 72m dao dong
	
	/*Gia tri quyet dinh sau bao xung mhz dem len 1 lan, tuc la sau bao nhieu dao dong se dem len 1 lan*/
	// 1 dao dong ton: 1/72m s
	// 1ms = 10^6s
	Tim_InitStruct.TIM_Prescaler = 72 - 1; // 72*1/72m = 1/1000000= 1ms
	
	/*Sau bao nhieu gia tri dem no se tran -> reset lai thanh ghi*/
	Tim_InitStruct.TIM_Period = 0xFFFF; // tuc la nhay len 1000 lan reset ve 0

	/*Mode dem len or dem xuong or mode khac*/
	Tim_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_Cmd(TIM2,ENABLE);
	TIM_TimeBaseInit(TIM2, &Tim_InitStruct);
}

void delay(uint32_t timedelay){
	TIM_SetCounter(TIM2,0);
	while(TIM_GetCounter(TIM2)<timedelay * 10);
}

int main(){
	RCC_Config();
	TIM_Config();
	delay(10000);
}

