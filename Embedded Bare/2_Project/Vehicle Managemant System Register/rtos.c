
#include "rtos.h"

/*
* Should be create a function to input size of thread
*/
#define NUMBER_OF_THREAD_MAX		3
#define SIZE_OF_THREAD_MAX			100

#define FREQ_DEFAULT			8000000

#define ICSR_VAL					(*((volatile uint32_t*)0xE000ED04))

static uint16_t NumberThreadCreate = 0;
static statusRTOS osKerneThreadInit(uint16_t ThreadNumber);
void osSchedularLauch(void);
void osKernelSW(void);
void osSemaphoreInit(uint32_t *semaphore, const uint32_t value);
void osSignalWait(uint32_t *semaphore);
void osSignalSet(uint32_t *semaphore);
static void osInitTimeSleep(void);


/**
* @brief Init all thing relate TCB
*/
struct tcb{
	uint32_t *stackPt;
	struct tcb *nextTCB;
	uint16_t priority;
	uint32_t sleep;
	uint16_t block; // sema = 0 
};
typedef struct tcb tcbType;
tcbType tcbs[NUMBER_OF_THREAD_MAX];
uint32_t stack_thread[NUMBER_OF_THREAD_MAX][SIZE_OF_THREAD_MAX];
tcbType *currentPt = &tcbs[0];

/**
* @brief osKernelInit use for init all timer using for osKernel
*/
uint32_t osTime; 
statusRTOS osKernelInit(void){
	osTime = (FREQ_DEFAULT/1000) - 1; // 1ms
	
	/*Init timer for periodic Thread*/
	osInitTimeSleep();
	
	return RTOS_SUCCES;
}

/**
* @brief osKernelAddThread func when u want to add new thread, and we also init TCB->nextTCB in here
* @param task: function u want to add into thread schedular
* @param Size_Of_Thread: size of stack u want to allocate on memory
* @param priority: set priority
*/
statusRTOS osKernelAddThread(void (*task)(void), int Size_Of_Thread, uint16_t priority, uint32_t sleep){
	
	if(NumberThreadCreate > NUMBER_OF_THREAD_MAX || Size_Of_Thread > SIZE_OF_THREAD_MAX){
		return RTOS_THREADMAX;
	}
	if(NumberThreadCreate != 0){
		tcbs[NumberThreadCreate].nextTCB = tcbs[NumberThreadCreate-1].nextTCB;
		tcbs[NumberThreadCreate-1].nextTCB = &tcbs[NumberThreadCreate];
	}
	else tcbs[NumberThreadCreate].nextTCB = &tcbs[0];
	
	osKerneThreadInit(NumberThreadCreate);
	stack_thread[NumberThreadCreate][SIZE_OF_THREAD_MAX - 2] = (uint32_t)(*task);
	tcbs[NumberThreadCreate].priority = priority;
	tcbs[NumberThreadCreate].sleep = sleep;
	tcbs[NumberThreadCreate].block = 0;
	
	NumberThreadCreate++;
	return RTOS_SUCCES;
}

/**
* @brief osKerneThreadInit in func will init register of new Thread and set stackPt when start run
*/
static statusRTOS osKerneThreadInit(uint16_t ThreadNumber){
	tcbs[ThreadNumber].stackPt = &stack_thread[ThreadNumber][SIZE_OF_THREAD_MAX - 16];
	
	/**
	* Init 8 register R0 - R3 R12 LR PC xPSR
	*/
	// set thumb mode
	stack_thread[ThreadNumber][SIZE_OF_THREAD_MAX - 1] = 0x01000000; // xPSR
	stack_thread[ThreadNumber][SIZE_OF_THREAD_MAX - 3] = 0x14141414; // LR
	stack_thread[ThreadNumber][SIZE_OF_THREAD_MAX - 4] = 0x12121212; // R12
	stack_thread[ThreadNumber][SIZE_OF_THREAD_MAX - 5] = 0x03030303; // R3
	stack_thread[ThreadNumber][SIZE_OF_THREAD_MAX - 6] = 0x02020202; // R2
	stack_thread[ThreadNumber][SIZE_OF_THREAD_MAX - 7] = 0x01010101; // R1
	stack_thread[ThreadNumber][SIZE_OF_THREAD_MAX - 8] = 0x00000000; // R0
	/**
	* Init 8 register R4-R11
	*/
	stack_thread[ThreadNumber][SIZE_OF_THREAD_MAX - 9]  = 0x11111111;  //R11
	stack_thread[ThreadNumber][SIZE_OF_THREAD_MAX - 10] = 0x10101010; //R10
	stack_thread[ThreadNumber][SIZE_OF_THREAD_MAX - 11] = 0x09090909; //R9
	stack_thread[ThreadNumber][SIZE_OF_THREAD_MAX - 12] = 0x08080808; //R8
	stack_thread[ThreadNumber][SIZE_OF_THREAD_MAX - 13] = 0x07070707; //R7
	stack_thread[ThreadNumber][SIZE_OF_THREAD_MAX - 14] = 0x06060606; //R6
	stack_thread[ThreadNumber][SIZE_OF_THREAD_MAX - 15] = 0x05050505; //R5
	stack_thread[ThreadNumber][SIZE_OF_THREAD_MAX - 16] = 0x04040404; //R4
	
	return RTOS_SUCCES;
}

/**
* @brief osKernelLaunch Use when want to run thread application, in func will set time quanta for Systick to start run thread schedular
*/
statusRTOS osKernelLaunch(uint32_t quanta){
	SysTick->CTRL = 0;
	SysTick->VAL = 0;
	
	/*?ms*/
	SysTick->LOAD = (quanta * osTime)-1;
	
	/*Set priority Systick and PendSV*/
	NVIC_SetPriority(SysTick_IRQn, 14);
	NVIC_SetPriority(PendSV_IRQn,15);
	
	/*start run Systick*/
	SysTick->CTRL = 0x00000007;
	
	/*start context switching with frist task in tcbs*/
	osSchedularLauch();
	
	return RTOS_SUCCES;
}

/**
* @brief osKernelSW asssign context sw here
*/
void osKernelSW(void){
	
	tcbType *_currentPt = currentPt;
	tcbType *_nextThreadRun = _currentPt;
	uint16_t highestPriority = 255;
	
	do{
		_currentPt = _currentPt->nextTCB;
		if(_currentPt->sleep==0 && _currentPt->block == 0 && _currentPt ->priority < highestPriority){
			highestPriority = _currentPt ->priority;
			_nextThreadRun = _currentPt;
		}
	}while(_currentPt != currentPt);
	
	
	currentPt = _nextThreadRun;
	
}


/**
* @brief SysTick_Handler activate by time quanta and inside will be activate PendSV
*/
void SysTick_Handler(void){
	/*
	* activate IRQ PendSV
	*/
	SCB->ICSR |= (1<<28);
}

/**
* @brief osSemaphoreInit use to init semphore and user will be manage this semaphore
* @param semaphore: point to a variable that user seem it is semaphore variable
* @param value: value for semphore
*/

void osSemaphoreInit(uint32_t *semaphore, const uint32_t value){
	__disable_irq();
	*semaphore = value;
	__enable_irq();
}


void osSignalWait(uint32_t *semaphore){
	while( *semaphore <= 0){
		/*
		* activate IRQ PendSV to context sw
		*/
		SysTick->VAL =0;
		SCB->ICSR |= (1 << 26); // set pending SysTick
	}
	*semaphore = *semaphore -1;;
}


void osSignalSet(uint32_t *semaphore){
	__disable_irq();
	*semaphore += 1;
	__enable_irq();
}

/**
* @brief osThreadSleep: thread want to sleep
*/

void osThreadSleep(uint32_t sleep_time){
	__disable_irq();
	currentPt -> sleep = sleep_time;
	currentPt -> block = 1;
	__enable_irq();
	SysTick->VAL =0;
	SCB->ICSR |= (1 << 28); // set pending SysTick	
}

/**
* @brief Timer for control sleep time
* Why need to timer for sleep time? Because Systick can be activate by SCB->ICSR |= (1 << 26); -> time sleep will be wrong
* 
*/
static void osInitTimeSleep(void){
	RCC->APB1ENR |= (1<<2);
	TIM4->PSC = 8000-1;
	TIM4->ARR = 1000-1;
	
	TIM4->DIER |= (1<<0); // enable IRQ for TIM4
	TIM4->CR1  |= (1<<0); // start count
	
	
	NVIC_SetPriority(TIM4_IRQn,1);
	NVIC_EnableIRQ(TIM4_IRQn);
	
}

void TIM4_IRQHandler(void){
	TIM4 -> SR &= ~(1<<0);
	uint16_t i;
	for(i =0;i<NumberThreadCreate;i++){
	   if(tcbs[i].sleep >0){
		  tcbs[i].sleep--;
			 if(tcbs[i].sleep == 0){
				 tcbs[i].block = 0;
			}
		}
	}
}
