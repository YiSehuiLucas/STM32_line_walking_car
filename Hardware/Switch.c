#include "stm32f10x.h"                  // Device header


/* 前置红外电子开关 */
uint8_t Switch_Num_Ahead(void)
{
	uint8_t num;
	num = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5);
	return num;
}

/* 后置红外电子开关 */
uint8_t Switch_Num_Back(void)
{
	uint8_t num;
	num = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4);
	return num;
}
