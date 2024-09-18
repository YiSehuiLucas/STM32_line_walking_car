#ifndef __JY901S_H
#define __JY901S_H
 
extern unsigned char yawAngle[2];
 
void MyDMA_Transfer(int BufferSize_DMA);
void JY901S_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT);
void JY901S_WriteReg(uint8_t RegAddress, uint8_t Data);
void JY901S_ReadReg(uint8_t RegAddress);
void JY901S_Init(void);
 
#endif

