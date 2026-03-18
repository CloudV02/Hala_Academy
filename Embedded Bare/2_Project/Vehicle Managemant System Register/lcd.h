#ifndef I2C_LCD_H_
#define I2C_LCD_H_

#include "stm32f10x.h"
#include "i2c.h"
#include "delay.h"
#include <stdint.h>

#define ADDRESS_LCD_SLAVE 0x27

#define LCD_EN 2
#define LCD_RW 1
#define LCD_RS 0
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7
#define LCD_BL 3

void LCD_Write(uint8_t *data);
void LCD_Write_4Bits(uint8_t half_data);
void LCD_Write_Char(uint8_t data);
void LCD_Write_Command(uint8_t command);
void LCD_Init(void);
void LCD_Print(char *str);
void LCD_NewLine(void);
void LCD_Clear_Display(void);

#endif
