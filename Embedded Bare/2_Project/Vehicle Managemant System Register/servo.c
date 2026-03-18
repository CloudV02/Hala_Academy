#include "servo.h"

void Servo_PWM_Init(void)
{
	// Initialization struct
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// Step 3: Initialize GPIOA (PA0)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	// Initialize PA0 as push-pull alternate function (PWM output) for LED
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	// Step 1: Initialize TIM2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	/*TIM2 default use 8Mhz of HSI -> 8/8 000 000 = 0.000001*/
	/*xem RCC reset value cua thanh ghi RCC_CR = 0x83 -> HSI luon ON, con nhung c	ai khac off*/
	TIM_TimeBaseInitStruct.TIM_Prescaler = 8-1;
	
	/*0.000001*20 000 = 0.02s -> 0.02*1000 = 20ms*/
	TIM_TimeBaseInitStruct.TIM_Period = 19999;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	// Start TIM2
	TIM_Cmd(TIM2, ENABLE);
	
	// Step 2: Initialize PWM
	// Common PWM settings
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_Pulse = 0;
	TIM_OC1Init(TIM2, &TIM_OCInitStruct);
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
}

void Set_ServoCorner(uint16_t setCorner){
	/*Set tu  100 - 200 tuong ung 1ms-2ms tren 20ms */
	/*150 = 90 do, 200 = 180 do*/
	TIM2->CCR1 =  setCorner;
}

