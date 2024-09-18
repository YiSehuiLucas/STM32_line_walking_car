#include "stm32f10x.h"                  // Device header

void MySPI_W_SS(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, (BitAction)BitValue);
}

void MySPI_Init(void)
{
	MySPI_W_SS(1);											
}

void MySPI_Start(void)
{
	MySPI_W_SS(0);				
}

void MySPI_Stop(void)
{
	MySPI_W_SS(1);				
}

uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) != SET);
	
	SPI_I2S_SendData(SPI2, ByteSend);								
	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) != SET);
	
	return SPI_I2S_ReceiveData(SPI2);								
}


