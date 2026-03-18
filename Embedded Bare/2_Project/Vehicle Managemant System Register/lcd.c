#include "lcd.h"

//static uint16_t rs = 0;
uint8_t data_t[3];

void LCD_Write_Command(uint8_t command){
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (command & 0xf0);
	data_l = ((command << 4) & 0xf0);
	data_t[0] = data_u | 0x0C;  //en=1, rs=0
	data_t[1] = data_u | 0x08;  //en=0, rs=0
	data_t[2] = data_l | 0x0C;  //en=1, rs=0
	data_t[3] = data_l | 0x08;  //en=0, rs=0
	
	I2C_Write(I2C1,ADDRESS_LCD_SLAVE, data_t,4);
}

void LCD_Write_Char(uint8_t data){
	volatile ErrorStatus check;
	
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data & 0xf0);
	data_l = ((data << 4) & 0xf0);
	data_t[0] = data_u | 0x0D;  //en=1, rs=0
	data_t[1] = data_u | 0x09;  //en=0, rs=0
	data_t[2] = data_l | 0x0D;  //en=1, rs=0
	data_t[3] = data_l | 0x09;  //en=0, rs=0
	
	check = I2C_Write(I2C1,ADDRESS_LCD_SLAVE, data_t,4);
}

void LCD_Write_4Bits(uint8_t half_data){
	uint8_t data_t[2];
	uint8_t data = (half_data & 0x0F)<<4;
	
	data_t[0] = (data & 0xf0)|0x0C; // E = 1
	data_t[1] = (data & 0xf0)|0x08; // E = 0
	
	I2C_Write(I2C1,ADDRESS_LCD_SLAVE, data_t,2);
}
void LCD_Init(void){
	
	I2C_LCD_Init();
	/*Wait LCD khoi dong*/
	DelayMs(50);
	/*Select 4 bit mode */
	LCD_Write_4Bits(0x03);
	DelayMs(5);
	LCD_Write_4Bits(0x03);
	DelayUs(200);
	LCD_Write_4Bits(0x03);
	DelayUs(50);
	LCD_Write_4Bits(0x02); // DB4 = DL = 0, DB5 = 1
	/*Set function set*/
	LCD_Write_Command(0x28);
	/*return home*/
	LCD_Write_Command(0x80);
	/*Use cursor*/
	LCD_Write_Command(0x0F);
	/*clear display*/
	LCD_Write_Command(0x01);
	DelayMs(2);
}

/*ham truyen chuoi*/
void LCD_Print(char *str)
{
    while(*str)
    {
      LCD_Write_Char(*str++);
			DelayUs(10);
    }
}

void LCD_Clear_Display(void){
	//LCD_Write_Command(0x80);
	LCD_Write_Command(0x01);
	DelayMs(2);
}

void LCD_Write(uint8_t *data){
	I2C_Write(I2C1,ADDRESS_LCD_SLAVE, data,1);
}
void LCD_NewLine(void){
	LCD_Write_Command(0xC0); // 0x1100 0000 <-> 0x40 | bit DB7
}

