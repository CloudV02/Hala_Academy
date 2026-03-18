#include "spi.h"



/*Config SPI*/
void RC522_SPI_Config(void){
	/*RCC*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	/*GPIO*/
	GPIO_InitTypeDef GPIO_InitType;
	GPIO_InitType.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitType.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitType.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitType);
	
	GPIO_InitType.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitType.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitType.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitType);
	
	/*chan CS -> HIGH first after low*/
	GPIO_InitType.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitType.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitType.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitType);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	/*SPI*/
	SPI_InitTypeDef SPI_InitType;
	SPI_InitType.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitType.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitType.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitType.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitType.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitType.SPI_NSS = SPI_NSS_Soft;
	SPI_InitType.SPI_Mode = SPI_Mode_Master;
	SPI_InitType.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitType.SPI_CRCPolynomial = 7;
	
	SPI_Init(SPI1,&SPI_InitType);
	SPI_Cmd(SPI1,ENABLE);
	/*
	* Enable IRQ
	* With 1 means RXMIE has data
	*/
	//SPI1->CR2 |= (1<<6); // bit RXNEIE
	
	/**
	* Set Priotity for SPI1 or RFID 
	*/
	/*NVIC_SetPriority(SPI1_IRQn, 1);
	NVIC_EnableIRQ(SPI1_IRQn);*/
	
}

/*Config SPI*/
/*PB12 - PB15*/
void SDCard_SPI_Config(void){
	/*RCC*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	/*GPIO*/
	GPIO_InitTypeDef GPIO_InitType;
	GPIO_InitType.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitType.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitType.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitType);
	
	GPIO_InitType.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitType.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitType.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitType);
	
	/*chan CS -> HIGH first after low*/
	GPIO_InitType.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitType.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitType.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitType);
		/*SPI*/
	SPI_InitTypeDef SPI_InitType;
	SPI_InitType.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitType.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitType.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitType.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitType.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitType.SPI_NSS = SPI_NSS_Soft;
	SPI_InitType.SPI_Mode = SPI_Mode_Master;
	SPI_InitType.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitType.SPI_CRCPolynomial = 7;
	
	SPI_Init(SPI2,&SPI_InitType);
	SPI_Cmd(SPI2,ENABLE);
	
}

/*send to data and recieve*/
uint8_t SPI_TransmitRecive(SPI_TypeDef *SPIx, uint8_t data){
	
	/*RESET buffer still have data, SET buffer = zezo*/
	while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_TXE)==RESET);
	
	SPI_I2S_SendData(SPIx,data);
	
	/*SET -> still tx and rx, reset -> done tx and rx*/
	while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_RXNE)==RESET);
	
	while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_BSY)==SET);
	return SPI_I2S_ReceiveData(SPIx);

}

/**
* Use for SD Card
*/
uint8_t My_SPI_Exchange(uint8_t u8Data)
{
	SPI_I2S_SendData(SPI2, u8Data);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET) {
	}
	return SPI_I2S_ReceiveData(SPI2);
}

