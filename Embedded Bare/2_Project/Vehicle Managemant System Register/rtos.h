#ifndef __RTOS_H__
#define __RTOS_H__

#include "stm32f10x.h"
#include <stdio.h>

/**
* @brief enum for RTOS
*/
enum status{
	RTOS_ERROR = 0,
	RTOS_SUCCES,
	RTOS_THREADMAX
};
typedef enum status statusRTOS;

/*Prototype*/
statusRTOS osKernelInit(void);
statusRTOS osKernelAddThread(void (*task)(void), int Size_Of_Thread, uint16_t priority, uint32_t sleep);

/**
*@brief osKernelLaunch: Use for the first time into RTOS, have to addThread before
*@param quanta: Set Time Quanta for each thread
*/
statusRTOS osKernelLaunch(uint32_t quanta);

/**
* @brief all function use for Semaphore
*/
void osSemaphoreInit(uint32_t *semaphore, const uint32_t value);
void osSignalWait(uint32_t *semaphore);
void osSignalSet(uint32_t *semaphore);


void osThreadSleep(uint32_t sleep_time);

#endif

