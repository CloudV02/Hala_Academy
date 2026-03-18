/*PB6 - PB7*/
#ifndef __I2C_H__
#define __I2C_H__

#include "stm32f10x.h"
#include "delay.h"


#define I2C_SCL_HIGH	GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define I2C_SCL_LOW		GPIO_ResetBits(GPIOB, GPIO_Pin_6)
#define I2C_SDA_HIGH	GPIO_SetBits(GPIOB, GPIO_Pin_7)
#define I2C_SDA_LOW		GPIO_ResetBits(GPIOB, GPIO_Pin_7)

void I2C_LCD_Init(void);
void I2C_LCD_Write(uint8_t address, const uint8_t *data);
void I2C_LCD_Read(uint8_t address, uint8_t *data_buffer);
ErrorStatus I2C_Write(I2C_TypeDef *I2Cx,
                      uint8_t slaveAddr,
                      uint8_t *buf,
                      uint16_t len);

#endif
