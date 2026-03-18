#include "delay.h"


void DelayInit(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	TIM_TimeBaseInitTypeDef Tim_InitStruct;
	
	/*Chia nho xung dau vao, clock cap cho timer */
	Tim_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1; // Clock cap cho tim 2 la 72mhz: 1s tao dc 72m dao dong
	
	/*Gia tri quyet dinh sau bao xung mhz dem len 1 lan, tuc la sau bao nhieu dao dong se dem len 1 lan*/
	// 1 dao dong ton: 1/72m s
	// 1ms = 10^6s
	Tim_InitStruct.TIM_Prescaler = 8 - 1; // 72*1/72m = 1/1000000= 1ms
	
	/*Sau bao nhieu gia tri dem no se tran -> reset lai thanh ghi*/
	Tim_InitStruct.TIM_Period = 0xFFFF; // tuc la nhay len 1000 lan reset ve 0

	/*Mode dem len or dem xuong or mode khac*/
	Tim_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_Cmd(TIM3,ENABLE);
	TIM_TimeBaseInit(TIM3, &Tim_InitStruct);
}

/*1 giay nhay 1 lan*/
void DelayMs(uint32_t ms)
{
    while (ms) {
		DelayUs(1000);
		--ms;
	}
}
void DelayUs(uint32_t us)
{
    TIM_SetCounter(TIM3, 0);
		while(TIM_GetCounter(TIM3) < us);
}
