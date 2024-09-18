#include "sys.h"
#include "usart.h"
#include "timer.h"

#if 1
#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
	int handle;

};

FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
	x = x;
}
//�ض���fputc����
int fputc(int ch, FILE *f)
{
	while((USART1->SR & 0X40) == 0); //ѭ������,ֱ���������
	USART1->DR = (u8) ch;
	return ch;
}
#endif

void USART1_SendData(u8 data)
{
	while((USART1->SR & 0X40) == 0);
	USART1->DR = data;
}

void USART1_SendString(u8 *DAT, u8 len)
{
	u8 i;
	for(i = 0; i < len; i++)
	{
		USART1_SendData(*DAT++);
	}
}

#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���
u8 USART1_RX_BUF[USART1_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
u16 USART1_RX_STA = 0;     //����״̬���

void USART1_Init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure6;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	//USART3_TX   GPIOC.10
	GPIO_InitStructure6.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure6.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure6.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure6);//��ʼ��GPIO

	//USART3_RX	  GPIOC.11��ʼ��
	GPIO_InitStructure6.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure6.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure6);//��ʼ��GPIO

	//USART3 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	//USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1

	TIM1_Int_Init(99, 7199);		//10ms�ж�
	USART1_RX_STA = 0;		//����
	TIM_Cmd(TIM1, DISABLE);			//�رն�ʱ��7
}

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		Res = USART_ReceiveData(USART1);	//��ȡ���յ�������
		if((USART1_RX_STA & 0x8000) == 0) //����δ���
		{
			if(USART1_RX_STA < USART1_REC_LEN)	//�����Խ�������
			{
				TIM_SetCounter(TIM1, 0); //���������          				//���������
				if(USART1_RX_STA == 0) 				//ʹ�ܶ�ʱ��7���ж�
				{
					TIM_Cmd(TIM1, ENABLE); //ʹ�ܶ�ʱ��7
				}
				USART1_RX_BUF[USART1_RX_STA++] = Res;	//��¼���յ���ֵ
			}
			else
			{
				USART1_RX_STA |= 1 << 15;				//ǿ�Ʊ�ǽ������
			}
		}
	}
}
#endif


