#include "stm32f10x.h"                  // Device header


/* ǰ�ú�����ӿ��� */
uint8_t Switch_Num_Ahead(void)
{
	uint8_t num;
	num = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5);
	return num;
}

/* ���ú�����ӿ��� */
uint8_t Switch_Num_Back(void)
{
	uint8_t num;
	num = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4);
	return num;
}
