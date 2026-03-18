#ifndef __UART_H__
#define __UART_H__

#include "stm32f10x.h"
#include <stdio.h>
#include <stdint.h>

void Terminal_UART_Init(void);

void SendData(uint8_t *data, uint16_t lengthData);
uint8_t RevData(void);
#endif

