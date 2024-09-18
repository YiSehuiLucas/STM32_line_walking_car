#include "stm32f10x.h"                  // Device header
#include "GPIO_Init.h"

void GPIOInit(void)
{
	// I2C1和I2C2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_ClockSpeed = 100000;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_Init(I2C1, &I2C_InitStructure);
	I2C_Init(I2C2, &I2C_InitStructure);
	
	I2C_Cmd(I2C1, ENABLE);
	I2C_Cmd(I2C2, ENABLE);
	
	
	// TIM2	
	TIM_InternalClockConfig(TIM2);
	
	GPIO_InitTypeDef GPIO_InitStructure1;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure1);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;	
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 100 - 1;		// 自动重装寄存器ARR
	TIM_TimeBaseInitStruct.TIM_Prescaler = 36 - 1;	// 预分频值, 内部时钟频率为72Mhz, 这里7200分频, 就成为了1whz
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;		// 重复计数器, 高级定时器用的, 这里不用, 置为0
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);		//时基单元初始化
	
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCStructInit(&TIM_OCInitStruct);		// 结构体变量赋初始值, 因为这里有些结构体成员不需要赋值, 所以通过这个给全部成员赋初始值, 以后再修改就行
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;		// 输出使能
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;		// 输出极性, 这里为高级性, 就是极性不翻转, REF直接输出, 还有一个低级性, REF波形取反
	TIM_OCInitStruct.TIM_Pulse = 0;		// 设置CCR(捕获/比较寄存器的值)
	TIM_OC1Init(TIM2, &TIM_OCInitStruct);
	TIM_OC2Init(TIM2, &TIM_OCInitStruct);
	TIM_OC3Init(TIM2, &TIM_OCInitStruct);
	TIM_OC4Init(TIM2, &TIM_OCInitStruct);
	
	TIM_Cmd(TIM2, ENABLE);
	
	// TIM3
	TIM_InternalClockConfig(TIM3);
	GPIO_InitTypeDef GPIO_InitStructure2;
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct1;
	TIM_TimeBaseInitStruct1.TIM_ClockDivision = TIM_CKD_DIV1;	
	TIM_TimeBaseInitStruct1.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct1.TIM_Period = 20000 - 1;		// 自动重装寄存器ARR
	TIM_TimeBaseInitStruct1.TIM_Prescaler = 72 - 1;	// 预分频值, 内部时钟频率为72Mhz, 这里7200分频, 就成为了1whz
	TIM_TimeBaseInitStruct1.TIM_RepetitionCounter = 0;		// 重复计数器, 高级定时器用的, 这里不用, 置为0
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct1);		//时基单元初始化
	
	TIM_OCInitTypeDef TIM_OCInitStruct1;
	TIM_OCStructInit(&TIM_OCInitStruct1);		// 结构体变量赋初始值, 因为这里有些结构体成员不需要赋值, 所以通过这个给全部成员赋初始值, 以后再修改就行
	TIM_OCInitStruct1.TIM_OutputState = TIM_OutputState_Enable;		// 输出使能
	TIM_OCInitStruct1.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct1.TIM_OCPolarity = TIM_OCPolarity_High;		// 输出极性, 这里为高级性, 就是极性不翻转, REF直接输出, 还有一个低级性, REF波形取反
	TIM_OCInitStruct1.TIM_Pulse = 0;		// 设置CCR(捕获/比较寄存器的值)
	
	TIM_OC1Init(TIM3, &TIM_OCInitStruct1);
	TIM_OC2Init(TIM3, &TIM_OCInitStruct1);
	TIM_OC3Init(TIM3, &TIM_OCInitStruct1);
	
	TIM_Cmd(TIM3, ENABLE);
	
	// 四个电机控制转向的逻辑电平
	// PA11和PA12
	GPIO_InitTypeDef GPIO_InitStructure3;
	GPIO_InitStructure3.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure3.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure3.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure3);
	
	//PB1和PB5
	GPIO_InitTypeDef GPIO_InitStructure4;
	GPIO_InitStructure4.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure4.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_5 | GPIO_Pin_12;
	GPIO_InitStructure4.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure4);
	
	// 红外电子开关
	// PA4和PA5
	GPIO_InitTypeDef GPIO_InitStructure5;
	GPIO_InitStructure5.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure5.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure5.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure5);
	
	// SPI通信
	GPIO_InitTypeDef GPIO_InitStructure6;
	GPIO_InitStructure6.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure6.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure6.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure6);
	
	SPI_InitTypeDef SPI_InitStructure;					
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;			
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;		
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;	
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;	
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;	
	SPI_InitStructure.SPI_CRCPolynomial = 7;			
	SPI_Init(SPI2, &SPI_InitStructure);
	
	SPI_Cmd(SPI2, ENABLE);
	
}


