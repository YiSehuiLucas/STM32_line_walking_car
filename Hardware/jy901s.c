#include "stm32f10x.h"                  // Device header
#include "jy901s.h"
#include "jy901s_reg.h"
 
#define JY901S_ADDRESS		0xa0
 
unsigned char yawAngle[2];
int flag = 0;
 
void MyDMA_Transfer(int BufferSize_DMA)
{
	DMA_Cmd(DMA1_Channel5, DISABLE);                
	DMA_SetCurrDataCounter(DMA1_Channel5, BufferSize_DMA);
	DMA_Cmd(DMA1_Channel5, ENABLE);
	
	while (DMA_GetFlagStatus(DMA1_FLAG_TC5) == RESET);
	DMA_ClearFlag(DMA1_FLAG_TC5);
}
 
 
void JY901S_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout;
	Timeout = 1000;
	while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
	{
		Timeout --;
		if (Timeout == 0)
		{
			break;
		}
	}
}
 
void JY901S_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	I2C_GenerateSTART(I2C1, ENABLE);
	JY901S_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);//EV5
	
	I2C_Send7bitAddress(I2C1, JY901S_ADDRESS, I2C_Direction_Transmitter);
	JY901S_WaitEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);//EV6
	
	I2C_SendData(I2C1, RegAddress);
	JY901S_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING);//EV8
	
	I2C_SendData(I2C1, Data);
	JY901S_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);//EV8_2
	
	I2C_GenerateSTOP(I2C1, ENABLE);
}
 
void JY901S_ReadReg(uint8_t RegAddress)
{	
	int8_t length;
	
	I2C_GenerateSTART(I2C1, ENABLE);
	JY901S_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);//EV5
	
	I2C_Send7bitAddress(I2C1, JY901S_ADDRESS, I2C_Direction_Transmitter);
	JY901S_WaitEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);//EV6
	
	I2C_SendData(I2C1, RegAddress);
	JY901S_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);//EV8_2
	
	I2C_GenerateSTART(I2C1, ENABLE);
	JY901S_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);//EV5
	
	I2C_Send7bitAddress(I2C1, JY901S_ADDRESS, I2C_Direction_Receiver);
	JY901S_WaitEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);//EV6
	
	if(flag != 0)MyDMA_Transfer(2);
 
	for(int8_t count=0;count<length;count++){	 
		 if(count!=length-1)
		 {			 
			 JY901S_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED);//EV_7 				
		 }
		 else{
					I2C_AcknowledgeConfig(I2C1, DISABLE);
					I2C_GenerateSTOP(I2C1, ENABLE);					
					JY901S_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED);//EV_7		
			}							
	}
	flag ++;
		
}
 
void JY901S_Init(void)
{
  DMA_InitTypeDef DMA_InitStructure;
  DMA_DeInit(DMA1_Channel5);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(I2C1->DR);
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)yawAngle;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 2; 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;     //DMA_PeripheralDataSize_Byte  ??int_8; DMA_PeripheralDataSize_HalfWord int_16; DMA_PeripheralDataSize_Word  int_32
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;             
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;               
																														
  DMA_Init(DMA1_Channel5, &DMA_InitStructure);
  DMA_Cmd(DMA1_Channel5, ENABLE);
	I2C_DMACmd(I2C1, ENABLE);	
}


