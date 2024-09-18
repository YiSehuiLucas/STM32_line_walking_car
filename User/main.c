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
u8 SYN_StopCom[] = {0xFD, 0X00, 0X02, 0X02, 0XFD}; //停止合成
u8 SYN_SuspendCom[] = {0XFD, 0X00, 0X02, 0X03, 0XFC}; //暂停合成
u8 SYN_RecoverCom[] = {0XFD, 0X00, 0X02, 0X04, 0XFB}; //恢复合成
u8 SYN_ChackCom[] = {0XFD, 0X00, 0X02, 0X21, 0XDE}; //状态查询
u8 SYN_PowerDownCom[] = {0XFD, 0X00, 0X02, 0X88, 0X77}; //进入POWER DOWN 状态命令
	//选择背景音乐2, (0无背景音乐, 1-15:背景音乐可选)
	//m[0~16]:(0为无背景音乐, 16背景音乐最大)
	//v[0~16]:0朗读音量为静音, 16朗读音量最大
	//t[0~5]:0朗读语速最慢,5朗读语速最快
	// SYN_FrameInfo(2, "[v7][m1][t5]欢迎使用SYN6288语音模块");
	// //YS_SYN_Set(SYN_SuspendCom); 暂停合成

#define GW_GRAY_ANALOG_BASE_ 0xDD
#define GW_GRAY_ANALOG_MODE  (GW_GRAY_ANALOG_BASE_ + 0)
#define GW_GRAY_black_white 160   // 待定
#define GW_GRAY_black_green 14     // 待定

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
		 // 语言：准备完毕
		SYN_FrameInfo(2, (u8*)"准备完毕"); 
		// 开始木板检测
	    while(Switch_Num_Ahead()!=1)
		{
			OLED_ShowNum(1, 1, Switch_Num_Ahead(), 1);
			OLED_ShowNum(2, 1, Switch_Num_Back(), 1);		
		}
		// 离开家
		Ahead();
		Set_Speed(40);
		Delay_ms(1000);
		Set_Speed(0);
//		// 开始巡线
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
		
	// 到达三号平台
		while(Switch_Num_Ahead())
		{
			Ahead();
			Set_Speed(70);
		}
//		
		Set_Speed(0);
		Hand();
		SYN_FrameInfo(2, (u8*)"到达三号平台"); 

		Back();
		Set_Speed(40);
		Delay_ms(10);

		
		Turn_Left(190);
		
		Back();
		Set_Speed(70);
		Delay_ms(100);



		
		// 离开三号平台
		
		
		
		// 未调起始处
		

		
		
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
		
		// 离开三角形
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
		
//	// 到达三号平台
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
		SYN_FrameInfo(2, (u8*)"到达三号平台"); 
		
		
		
		
		
		
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
		
		// 离开三号平台
		Ahead();
		Set_Speed(50);
		Delay_ms(1000);
		
		Ahead_Line_Walking(70);
		
		Ahead();
		Set_Speed(40);
		Delay_ms(40);
		
		Turn_Right(50);
		
		// 进入直立景点
		Back();
		Set_Speed(70);
		Delay_ms(150);
		
		Set_Speed(0);
		
		
		while(!Switch_Num_Back())
		{
			Back_Line_Walking(70);
		}
		SYN_FrameInfo(2, (u8*)"到达直立景点");
		Ahead_Line_Walking(70);
		// 离开直立景点
		
		Ahead();
		Set_Speed(70);
		Delay_ms(20);
		
		Turn_Left(40);
		
		Ahead_Line_Walking(70);
		
		Ahead();
		Set_Speed(70);
		Delay_ms(50);
		
		Ahead_Line_Walking(50);
		
		
		// 进入直立景点
		Turn_Left(30);
		while(!Switch_Num_Ahead())
		{
			Ahead_Line_Walking(70);
		}
		SYN_FrameInfo(2, (u8*)"到达直立景点");
		Turn_Right(50);
		Ahead();
		Set_Speed(70);
		Delay_ms(150);
		Ahead_Line_Walking(70);
		Delay_ms(6000);

}







