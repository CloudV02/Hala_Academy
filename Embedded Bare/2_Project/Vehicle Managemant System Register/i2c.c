#include "i2c.h"

void I2C_LCD_Init(void){
	/*Clock*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
	/*I2C1*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	/*PB6 - PB7*/
	GPIO_InitTypeDef GPIO_InitType;
	GPIO_InitType.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitType.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitType.GPIO_Speed = GPIO_Speed_50MHz;
	
	
	I2C_InitTypeDef I2C_InitType;
	I2C_InitType.I2C_Mode = I2C_Mode_I2C;
	I2C_InitType.I2C_Ack = I2C_Ack_Enable;
	I2C_InitType.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitType.I2C_ClockSpeed = 100000;
	I2C_InitType.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitType.I2C_OwnAddress1 = 0x00;
	
	GPIO_Init(GPIOB,&GPIO_InitType);
	I2C_Init(I2C1,&I2C_InitType);
	I2C_Cmd(I2C1,ENABLE);
	
	
}

void I2C_LCD_Read(uint8_t address, uint8_t *data_buffer)
{
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C1, address << 1, I2C_Direction_Receiver);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    I2C_AcknowledgeConfig(I2C1, DISABLE);   // ACK off cho 1 byte
    I2C_GenerateSTOP(I2C1, ENABLE);         // STOP tru?c khi d?c

    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
    *data_buffer = I2C_ReceiveData(I2C1);

    I2C_AcknowledgeConfig(I2C1, ENABLE);    // b?t l?i ACK
}
ErrorStatus I2C_Write(I2C_TypeDef *I2Cx,
                      uint8_t slaveAddr,
                      uint8_t *buf,
                      uint16_t len)
{
    uint32_t timeout;

    if (len == 0) return SUCCESS;

    /* Wait until bus idle */
    timeout = 100000;
    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)) {
        if (--timeout == 0) return ERROR;
    }

    /* Generate START */
    I2C_GenerateSTART(I2Cx, ENABLE);

    /* EV5 */
    timeout = 100000;
    while (!I2C_CheckEvent(I2Cx,
           I2C_EVENT_MASTER_MODE_SELECT)) {
        if (--timeout == 0) return ERROR;
    }

    /* Send address (write) */
    I2C_Send7bitAddress(I2Cx,
                        slaveAddr<<1,
                        I2C_Direction_Transmitter);

    /* EV6 */
    timeout = 100000;
    while (!I2C_CheckEvent(I2Cx,
           I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if (--timeout == 0) return ERROR;
    }

    /* Send bytes */
    while (len--) {
        I2C_SendData(I2Cx, *buf++);
				DelayUs(50);

        /* EV8 / EV8_2 */
        timeout = 100000;
        while (!I2C_CheckEvent(I2Cx,
               I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
            if (--timeout == 0) return ERROR;
        }
    }

    /* Generate STOP */
    I2C_GenerateSTOP(I2Cx, ENABLE);

    return SUCCESS;
}




