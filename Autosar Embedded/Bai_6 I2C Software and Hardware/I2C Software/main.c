#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#define I2C_SCL 	GPIO_Pin_6
#define I2C_SDA		GPIO_Pin_7
#define I2C_GPIO	GPIOB

typedef enum{
	NOT_OK,
	OK
}status;

void RCC_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

void GPIO_Config(){
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin |= I2C_SCL | I2C_SDA;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void TIM_Base_Config(){
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1; // 36Mhz
	
	/*	1/36000000	*/
	TIM_InitStruct.TIM_Prescaler = 36000-1; // 0.001
	
	TIM_InitStruct.TIM_Period = 0xFFFF;
	
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
	TIM_Cmd(TIM2,ENABLE);
}
void delay_ms(uint16_t number_delay){ //0.001
	TIM_SetCounter(TIM2,0);
	while(TIM_GetCounter(TIM2) < number_delay );
}
void delay_us(uint16_t number_delay){ //0.001
	TIM_SetCounter(TIM2,0);
	while(TIM_GetCounter(TIM2) < number_delay*0.001 );
}
void I2C_Config(){
	GPIO_WriteBit(I2C_GPIO,I2C_SDA,Bit_SET);
	delay_us(1);
	GPIO_WriteBit(I2C_GPIO,I2C_SCL,Bit_SET);
	delay_us(1);
}
void I2C_Start(){
	GPIO_SetBits(I2C_GPIO,I2C_SDA);
	delay_us(4);
	GPIO_SetBits(I2C_GPIO,I2C_SCL);
	delay_us(4);
	GPIO_ResetBits(I2C_GPIO,I2C_SDA);
	delay_us(4);
	GPIO_ResetBits(I2C_GPIO,I2C_SCL);
	delay_us(4);
}
void I2C_Stop(){
	GPIO_ResetBits(I2C_GPIO,I2C_SDA);
	delay_us(4);
	GPIO_SetBits(I2C_GPIO, I2C_SCL);
	delay_us(4);
	GPIO_SetBits(I2C_GPIO, I2C_SDA);
	delay_us(4);
}
status I2CWrite(uint8_t u8data){
	status st_Write;
	for(int i = 0; i< 8 ;i++){
		if(u8data&0x80){
			GPIO_SetBits(I2C_GPIO,I2C_SDA);
		}
		else{
			GPIO_ResetBits(I2C_GPIO,I2C_SDA);
		}
		delay_us(3);
		GPIO_SetBits(I2C_GPIO,I2C_SCL);
		delay_us(5);
		GPIO_ResetBits(I2C_GPIO,I2C_SCL);
		delay_us(2);
		u8data = u8data << 1;
	}
	GPIO_SetBits(I2C_GPIO,I2C_SDA);
	delay_us(3);
	GPIO_SetBits(I2C_GPIO,I2C_SCL);
	delay_us(3);
	if(GPIO_ReadInputDataBit(I2C_GPIO,I2C_SDA)){
		st_Write = NOT_OK;
	}
	else{
		st_Write = OK;
	}
	delay_us(2);
	GPIO_ResetBits(I2C_GPIO,I2C_SCL);
	delay_us(2);
	return st_Write;
}

uint8_t I2C_Read(status ACK_Bit){
	uint8_t data_recei;
	GPIO_SetBits(I2C_GPIO,I2C_SDA);
	delay_us(3);
	for(int i = 0; i<8;i++){
		data_recei <<= 1;
		GPIO_SetBits(I2C_GPIO, I2C_SCL);
		delay_us(4);
		if(GPIO_ReadInputDataBit(I2C_GPIO,I2C_SDA)){
			data_recei |= 0x01; 
		}
		else{
			data_recei |= 0x00; 
		}
		GPIO_ResetBits(I2C_GPIO, I2C_SCL);
		delay_us(2);
	}
	if(data_recei){
		GPIO_ResetBits(I2C_GPIO, I2C_SDA);
	}
	else{
		GPIO_SetBits(I2C_GPIO,I2C_SDA);
	}
	delay_us(3);
	GPIO_SetBits(I2C_GPIO,I2C_SCL);
	delay_us(5);
	GPIO_ResetBits(I2C_GPIO,I2C_SCL);
	delay_us(2);
	return data_recei;
}

int main()
{
		
}
