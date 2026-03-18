#include "rifd.h"
#include "lcd.h"
#include "servo.h"
#include "stdio.h"
#include "ff.h"
#include "rtos.h"
#include <string.h>

/**
* @brief Variable use for RFID
*/
uint8_t CardID[5];
char szBuff[16];
char currentCard[0];
void taskRFID(void);

/**
* @brief Variable use for Servo
*/
static uint32_t semaServo;
void taskServo(void);

/**
* @brief Variable use for LCD
*/
static uint32_t semaLCD;
void taskLCD(void);

void My_GPIO_Init(void);

/**
* @brief Variable use SD Card
*/
FATFS FatFs;		/* FatFs work area needed for each volume */
FIL Fil;			/* File object needed for each open file */
UINT bw;
FRESULT fr;
void registCard(char *idCard);
//static uint32_t semaSDCard;


uint32_t count1, count2, count3 = 0;
/*
void task1(void){
	while(1){
		count1++;
		osSignalSet(&semaServo);
		osSignalSet(&semaLCD);
		osThreadSleep(3);
	}
}
void task2(void){
	while(1){
		osSignalWait(&semaServo);
		count2++;
		osThreadSleep(5);
	}
}
void task3(void){
	while(1){
		osSignalWait(&semaLCD);
		count3++;
	}
}*/


int main(void){
	DelayInit();
	LCD_Init();
	Servo_PWM_Init();
	RC522_Init();
	osKernelInit();
	osKernelAddThread(taskRFID,100,1,0);
	osKernelAddThread(taskServo,100,1,0);
	osKernelAddThread(taskLCD,100,3,0);
	osSemaphoreInit(&semaServo,0);
	osSemaphoreInit(&semaLCD,0);
	/*osKernelAddThread(task1,100,0,0);
	osKernelAddThread(task2,100,1,0);
	osKernelAddThread(task3,100,2,0);*/
	
	LCD_Print("Hello");
	My_GPIO_Init();

	fr = f_mount(&FatFs, "", 1);		/* Give a work area to the default drive */

	registCard("B6C26B2.txt");
	registCard("F71F8BC.txt");
	fr = f_open(&Fil, "Hi.txt", FA_WRITE | FA_OPEN_APPEND);	/* Create a file */
	if (fr == FR_OK) {
		f_write(&Fil, "Welcome", 8, &bw);	/* Write data to the file */
		fr = f_close(&Fil);							/* Close the file */		  
		osKernelLaunch(2000);
	}
}

void taskRFID(void){

	while(1){
		if (RC522_Check(CardID) == RC522_OK) 
		{
			sprintf(szBuff, "ID:%02X%02X%02X%02X%02X", CardID[0], CardID[1], CardID[2], CardID[3], CardID[4]);
			currentCard[0] = szBuff[3];
			osSignalSet(&semaServo);
			osSignalSet(&semaLCD);
			osThreadSleep(3);
		}
	}
}

void taskWriteSDCard(char *dataCard, char *idCard){
	fr = f_open(&Fil,idCard,FA_WRITE | FA_OPEN_APPEND);
	fr = f_write(&Fil,dataCard,10,&bw);		
	fr = f_write(&Fil,"\r\n",2,&bw);		
	f_close(&Fil);

}

uint8_t checkStatusBarrier = 0;

// 2600 - 17000
void taskServo(void){
	while(1){
		if(checkStatusBarrier == 0x00){
			osSignalWait(&semaServo);
			Set_ServoCorner(2600);
      checkStatusBarrier = 1;
			osThreadSleep(5);
		}
		else{
			if(semaServo != 0x00)
			{
				osSignalWait(&semaServo);
				osThreadSleep(5);
				continue;
			}
			Set_ServoCorner(1700);
      checkStatusBarrier = 0;
		}
	}
}

uint32_t countTimeCar1 = 0;
uint32_t countTimeCar2 = 0;
char bufCountTimeCar[16];
void taskLCD(void){
	while(1){
		osSignalWait(&semaLCD);				
		LCD_Clear_Display();
		char *dataCard;
		if(currentCard[0]== 'B'){
			countTimeCar1++;
		  LCD_Print(szBuff);
			LCD_NewLine();
			if(countTimeCar1%2 != 0) sprintf(bufCountTimeCar,"IN: %d",countTimeCar1);
			else 										sprintf(bufCountTimeCar,"OUT: %d", countTimeCar1);				
			LCD_Print(bufCountTimeCar);
			dataCard = bufCountTimeCar;
			taskWriteSDCard(dataCard, "B6C26B2.txt");
		}
	
		if(currentCard[0]=='F'){
			countTimeCar2++;
		  LCD_Print(szBuff);			
			LCD_NewLine();
			if(countTimeCar2%2 != 0) sprintf(bufCountTimeCar,"IN: %d",countTimeCar2);
			else 										sprintf(bufCountTimeCar,"OUT: %d", countTimeCar2);				
			LCD_Print(bufCountTimeCar);
			dataCard = bufCountTimeCar;
			taskWriteSDCard(dataCard, "F71F8BC.txt");
		}
	}
}

void My_GPIO_Init(void) {
	GPIO_InitTypeDef gpioInit;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	gpioInit.GPIO_Mode=GPIO_Mode_Out_PP;
	gpioInit.GPIO_Speed=GPIO_Speed_50MHz;
	gpioInit.GPIO_Pin=GPIO_Pin_13;
	GPIO_Init(GPIOC, &gpioInit);
}

void registCard(char* idCard){
	fr = f_open(&Fil,idCard,FA_WRITE | FA_OPEN_APPEND);
	
	f_close(&Fil);
}


