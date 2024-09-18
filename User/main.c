#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "GPIO_Init.h"
#include "Motor.h"
#include "I2C.h"
#include "timer.h"
#include "SPI.h"
#include "syn6288.h"
#include "W25Q64.h"
#include "usart.h"
#include "sys.h"
#include "stdio.h"
#include "Switch.h"
#include "Servo.h"

/**************syn6288*********************/
u8 SYN_StopCom[] = {0xFD, 0X00, 0X02, 0X02, 0XFD}; //ֹͣ�ϳ�
u8 SYN_SuspendCom[] = {0XFD, 0X00, 0X02, 0X03, 0XFC}; //��ͣ�ϳ�
u8 SYN_RecoverCom[] = {0XFD, 0X00, 0X02, 0X04, 0XFB}; //�ָ��ϳ�
u8 SYN_ChackCom[] = {0XFD, 0X00, 0X02, 0X21, 0XDE}; //״̬��ѯ
u8 SYN_PowerDownCom[] = {0XFD, 0X00, 0X02, 0X88, 0X77}; //����POWER DOWN ״̬����
	//ѡ�񱳾�����2, (0�ޱ�������, 1-15:�������ֿ�ѡ)
	//m[0~16]:(0Ϊ�ޱ�������, 16�����������)
	//v[0~16]:0�ʶ�����Ϊ����, 16�ʶ��������
	//t[0~5]:0�ʶ���������,5�ʶ��������
	// SYN_FrameInfo(2, "[v7][m1][t5]��ӭʹ��SYN6288����ģ��");
	// //YS_SYN_Set(SYN_SuspendCom); ��ͣ�ϳ�

#define GW_GRAY_ANALOG_BASE_ 0xDD
#define GW_GRAY_ANALOG_MODE  (GW_GRAY_ANALOG_BASE_ + 0)
#define GW_GRAY_black_white 160   // ����
#define GW_GRAY_black_green 14     // ����

uint8_t Gray_Data_Ahead[8];
uint8_t Gray_Data_Back[8];
uint8_t Color[3]; 
uint8_t Angle[2];


int main(void)
{
	OLED_Init();
	GPIOInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	USART1_Init(9600);

//		while(1)
//		{
	    Hand(); 
		 // ���ԣ�׼�����
		SYN_FrameInfo(2, (u8*)"׼�����"); 
		// ��ʼľ����
	    while(Switch_Num_Ahead()!=1)
		{
			OLED_ShowNum(1, 1, Switch_Num_Ahead(), 1);
			OLED_ShowNum(2, 1, Switch_Num_Back(), 1);		
		}
		// �뿪��
		Ahead();
		Set_Speed(40);
		Delay_ms(1000);
		Set_Speed(0);
//		// ��ʼѲ��
		Ahead_Line_Walking(50);
		
		Ahead();
		Set_Speed(50);
		Delay_ms(80);
		
		Turn_Left(40);

		
		Ahead();
		Set_Speed(50);
		Delay_ms(250);
		
		Ahead_Line_Walking(70);
		
		Ahead();
		Set_Speed(50);
		Delay_ms(150);
		Ahead_Line_Walking(70);
		Ahead();
		Set_Speed(70);
		Delay_ms(600);
		
		
		Ahead_Line_Walking(70);
		
		Ahead();
		Set_Speed(50);
		Delay_ms(250);
		
		Turn_Right(40);
		
		Ahead();
		Set_Speed(70);
		Delay_ms(250);
		
		Ahead_Line_Walking(70);
		
		Ahead();
		Set_Speed(70);
		Delay_ms(400);
		
		Ahead_Line_Walking(70);
		
	// ��������ƽ̨
		while(Switch_Num_Ahead())
		{
			Ahead();
			Set_Speed(70);
		}
//		
		Set_Speed(0);
		Hand();
		SYN_FrameInfo(2, (u8*)"��������ƽ̨"); 

		Back();
		Set_Speed(40);
		Delay_ms(10);

		
		Turn_Left(190);
		
		Back();
		Set_Speed(70);
		Delay_ms(100);



		
		// �뿪����ƽ̨
		
		
		
		// δ����ʼ��
		

		
		
		Ahead();
		Set_Speed(50);
		Delay_ms(250);

		
		Ahead_Line_Walking(50);
		
		Ahead();
		Set_Speed(80);
		Delay_ms(600);
		
		Ahead();
		Set_Speed(50);
		Delay_ms(300);
		
		Ahead_Line_Walking(70);
		
;
		
		// �뿪������
		Ahead();
		Set_Speed(50);
		Delay_ms(100);
		Turn_Right(40);
//		
		Ahead();
		Set_Speed(70);
		Delay_ms(35);
		
		Ahead_Line_Walking(70);
		Ahead();
		Set_Speed(70);
		Delay_ms(100);
		
		Ahead_Line_Walking(70);
		
//	// ��������ƽ̨
		while(!Switch_Num_Ahead())
		{
			Ahead();
			Set_Speed(70);
			Delay_ms(100);
			OLED_ShowNum(1, 1, Switch_Num_Ahead(), 1);
		}
		Set_Speed(0);
//		
		Hand();
		SYN_FrameInfo(2, (u8*)"��������ƽ̨"); 
		
		
		
		
		
		
		Back();
		Set_Speed(40);
		Delay_ms(10);
		Set_Speed(0);
		
		Turn_Left(190);
		
		Back();
		Set_Speed(70);
		Delay_ms(100);
		Set_Speed(0);

		Set_Speed(0);
		
		// �뿪����ƽ̨
		Ahead();
		Set_Speed(50);
		Delay_ms(1000);
		
		Ahead_Line_Walking(70);
		
		Ahead();
		Set_Speed(40);
		Delay_ms(40);
		
		Turn_Right(50);
		
		// ����ֱ������
		Back();
		Set_Speed(70);
		Delay_ms(150);
		
		Set_Speed(0);
		
		
		while(!Switch_Num_Back())
		{
			Back_Line_Walking(70);
		}
		SYN_FrameInfo(2, (u8*)"����ֱ������");
		Ahead_Line_Walking(70);
		// �뿪ֱ������
		
		Ahead();
		Set_Speed(70);
		Delay_ms(20);
		
		Turn_Left(40);
		
		Ahead_Line_Walking(70);
		
		Ahead();
		Set_Speed(70);
		Delay_ms(50);
		
		Ahead_Line_Walking(50);
		
		
		// ����ֱ������
		Turn_Left(30);
		while(!Switch_Num_Ahead())
		{
			Ahead_Line_Walking(70);
		}
		SYN_FrameInfo(2, (u8*)"����ֱ������");
		Turn_Right(50);
		Ahead();
		Set_Speed(70);
		Delay_ms(150);
		Ahead_Line_Walking(70);
		Delay_ms(6000);

}







